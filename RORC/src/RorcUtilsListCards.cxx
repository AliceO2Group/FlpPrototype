///
/// \file RorcUtilsReadRegister.cxx
/// \author Pascal Boeschoten
///
/// Utility that lists the RORC devices on the system
///

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <boost/exception/diagnostic_information.hpp>
#include "RorcDevice.h"
#include "RorcUtilsOptions.h"
#include "RorcUtilsCommon.h"
#include "RorcUtilsProgram.h"
#include <boost/format.hpp>

using namespace AliceO2::Rorc::Util;
using std::cout;
using std::endl;

class ProgramListCards: public RorcUtilsProgram
{
  public:
    virtual ~ProgramListCards()
    {
    }

    virtual UtilsDescription getDescription()
    {
      return UtilsDescription("List Cards", "Lists installed RORC cards", "./rorc-list-cards");
    }

    virtual void addOptions(boost::program_options::options_description&)
    {
    }

    virtual void mainFunction(boost::program_options::variables_map&)
    {
      auto cardsFound = AliceO2::Rorc::RorcDevice::enumerateDevices();

      cout << "Found " << cardsFound.size() << " card(s)\n";

      auto formatHeader = "  %-3s %-12s %-12s %-12s %-12s \n";
      auto formatRow = "  %-3s %-12s 0x%-10s 0x%-10s %-12s \n";
      auto header = boost::str(boost::format(formatHeader) % "#" % "Card Type" % "Vendor ID" % "Device ID" % "Serial Nr.");
      auto lineFat = std::string(header.length(), '=') + '\n';
      auto lineThin = std::string(header.length(), '-') + '\n';

      cout << lineFat << header << lineThin;

      int i = 0;
      for (auto& card : cardsFound) {
        auto cardType = AliceO2::Rorc::CardType::toString(card.cardType);
        cout << boost::str(boost::format(formatRow) % i % cardType % card.vendorId % card.deviceId % card.serialNumber);
        i++;
      }

      cout << lineFat;
    }
};

int main(int argc, char** argv)
{
  return ProgramListCards().execute(argc, argv);
}
