/// \file BarAccessor.h
/// \brief Definition of the CruBarAccessor class.
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#pragma once

#include <cstddef>
#include <boost/optional/optional.hpp>
#include "Cru/Constants.h"
#include "Pda/PdaBar.h"
#include "Utilities/Util.h"

namespace AliceO2 {
namespace Rorc {
namespace Cru {

/// A simple wrapper object for accessing the CRU BAR
class BarAccessor
{
  public:
    BarAccessor(Pda::PdaBar* pdaBar)
        : mPdaBar(pdaBar)
    {
    }

    enum class BufferStatus
    {
        AVAILABLE,
        BUSY
    };

    /// Set the registers of a descriptor entry
    /// \param index FIFO index
    /// \param pages Amount of 8 kiB pages in superpage
    /// \param address Superpage PCI bus address
    void setSuperpageDescriptor(uint32_t index, uint32_t pages, uintptr_t busAddress)
    {
      assert(index < SUPERPAGE_DESCRIPTORS);

      // Set superpage address
      mPdaBar->writeRegister(Registers::SUPERPAGE_ADDRESS_HIGH, Utilities::getUpper32Bits(busAddress));
      mPdaBar->writeRegister(Registers::SUPERPAGE_ADDRESS_LOW, Utilities::getLower32Bits(busAddress));

      // Set superpage size and FIFO index
      const uint32_t addressShift = 5;
      const uint32_t indexMask = 0b11111;
      uint32_t pagesAvailableAndIndex = index & indexMask;
      pagesAvailableAndIndex |= (pages << addressShift) & (~indexMask);
      mPdaBar->writeRegister(Registers::SUPERPAGE_PAGES_AVAILABLE_AND_INDEX, pagesAvailableAndIndex);

      // Set superpage enabled
      mPdaBar->writeRegister(Registers::SUPERPAGE_STATUS, index);
    }

    uint32_t getSuperpagePushedPages(uint32_t index)
    {
      assert(index < SUPERPAGE_DESCRIPTORS);
      return mPdaBar->readRegister(Registers::SUPERPAGE_PUSHED_PAGES + index);
    }

    BufferStatus getSuperpageBufferStatus(uint32_t index)
    {
      uint32_t status = mPdaBar->readRegister(Registers::SUPERPAGE_STATUS);
      uint32_t bit = status & (0b1 << index);
      if (bit == 0) {
        return BufferStatus::BUSY;
      } else {
        return BufferStatus::AVAILABLE;
      }
    }

    void setDataEmulatorEnabled(bool enabled) const
    {
      mPdaBar->writeRegister(Registers::DATA_EMULATOR_CONTROL, enabled ? 0x3 : 0x0);
    }

    void resetDataGeneratorCounter() const
    {
      mPdaBar->writeRegister(Registers::RESET_CONTROL, 0x2);
    }

    void resetCard() const
    {
      mPdaBar->writeRegister(Registers::RESET_CONTROL, 0x1);
    }

    void setDataGeneratorPattern(GeneratorPattern::type pattern)
    {
      constexpr uint32_t INCREMENTAL = 0b01;
      constexpr uint32_t ALTERNATING = 0b10;
      constexpr uint32_t CONSTANT = 0b11;

      auto value = [&pattern]() -> uint32_t { switch (pattern) {
          case GeneratorPattern::Incremental: return INCREMENTAL;
          case GeneratorPattern::Alternating: return ALTERNATING;
          case GeneratorPattern::Constant:    return CONSTANT;
          default: BOOST_THROW_EXCEPTION(Exception()
              << ErrorInfo::Message("Unsupported generator pattern for CRU")
              << ErrorInfo::GeneratorPattern(pattern)); }
      };

      mPdaBar->writeRegister(Registers::DMA_CONFIGURATION, value());
    }

    uint32_t getSerialNumber() const
    {
      if (mPdaBar->getBarNumber() != 2) {
        BOOST_THROW_EXCEPTION(Exception()
            << ErrorInfo::Message("Can only get serial number from BAR 2")
            << ErrorInfo::BarIndex(mPdaBar->getBarNumber()));
      }

      return mPdaBar->readRegister(Registers::SERIAL_NUMBER);
    }

    /// Get raw data from the temperature register
    uint32_t getTemperatureRaw() const
    {
      if (mPdaBar->getBarNumber() != 2) {
        BOOST_THROW_EXCEPTION(Exception()
            << ErrorInfo::Message("Can only get temperature from BAR 2")
            << ErrorInfo::BarIndex(mPdaBar->getBarNumber()));
      }

      // Only use lower 10 bits
      return mPdaBar->readRegister(Registers::TEMPERATURE) & 0x3ff;
    }

    /// Converts a value from the CRU's temperature register and converts it to a °C double value.
    /// \param registerValue Value of the temperature register to convert
    /// \return Temperature value in °C or nothing if the registerValue was invalid
    boost::optional<float> convertTemperatureRaw(uint32_t registerValue) const
    {
      /// It's a 10 bit register, so: 2^10 - 1
      constexpr int REGISTER_MAX_VALUE = 1023;

      // Conversion formula from: https://documentation.altera.com/#/00045071-AA$AA00044865
      if (registerValue == 0 || registerValue > REGISTER_MAX_VALUE) {
        return boost::none;
      } else {
        float A = 693.0;
        float B = 265.0;
        float C = float(registerValue);
        return ((A * C) / 1024.0f) - B;
      }
    }

    /// Gets the temperature in °C, or nothing if the temperature value was invalid.
    /// \return Temperature value in °C or nothing if the temperature value was invalid
    boost::optional<float> getTemperatureCelsius() const
    {
      return convertTemperatureRaw(getTemperatureRaw());
    }

    uint32_t getIdleCounterLower()
    {
      return mPdaBar->readRegister(Registers::IDLE_COUNTER_LOWER);
    }

    uint32_t getIdleCounterUpper()
    {
      return mPdaBar->readRegister(Registers::IDLE_COUNTER_UPPER);
    }

    uint64_t getIdleCounter()
    {
      return (uint64_t(getIdleCounterUpper()) << 32) + uint64_t(getIdleCounterLower());
    }

    uint32_t getIdleMaxValue()
    {
      return mPdaBar->readRegister(Registers::MAX_IDLE_VALUE);
    }

    uint32_t getFirmwareCompileInfo()
    {
      return mPdaBar->readRegister(Registers::FIRMWARE_COMPILE_INFO);
    }

    uint8_t getDebugReadWriteRegister()
    {
      return mPdaBar->getRegister<uint8_t>(toByteAddress(Registers::DEBUG_READ_WRITE));
    }

    void setDebugReadWriteRegister(uint8_t value)
    {
      return mPdaBar->setRegister<uint8_t>(toByteAddress(Registers::DEBUG_READ_WRITE), value);
    }

    void setLedState(bool onOrOff)
    {
      int on = 0x00; // Yes, a 0 represents the on state
      int off = 0xff;
      mPdaBar->writeRegister(Registers::LED_STATUS, onOrOff ? on : off);
    }

  private:
    /// Convert an index to a byte address
    size_t toByteAddress(size_t address32)
    {
      return address32 * 4;
    }

    Pda::PdaBar* mPdaBar;
//    uintptr_t bar;
};

} // namespace Cru
} // namespace Rorc
} // namespace AliceO2