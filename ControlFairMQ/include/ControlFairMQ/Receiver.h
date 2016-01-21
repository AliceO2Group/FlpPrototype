/// @file    Receiver.h
/// \brief   Definition of Receiver class
///
/// @author  Vasco Barroso

#ifndef ALICEO2_CONTROLFAIRMQ_CORE_RECEIVER_H
#define ALICEO2_CONTROLFAIRMQ_CORE_RECEIVER_H

#include <FairMQDevice.h>
#include "InfoLogger/InfoLogger.hxx"

namespace AliceO2 {
/// ALICE O2 Control system tests with FairMQ
namespace ControlFairMQ {
/// Core features
namespace Core {
/// Test receiver with FairMQ
///
/// Simple test receiver to play with FairMQ
class Receiver : public FairMQDevice
{
  public:
    /// Standard constructor
    Receiver();

    /// Destructor
    virtual ~Receiver();

  protected:
    /// Running state
    virtual void Run() override;

  private:
    /// Get logger object
    InfoLogger::InfoLogger& getLogger();

    InfoLogger::InfoLogger mLogger;       ///< Logger object
};

} // namespace Hello
} // namespace ControlFairMQ
} // namespace Core

#endif // ALICEO2_CONTROLFAIRMQ_CORE_RECEIVER_H
