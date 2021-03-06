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
    bool HandleData(FairMQMessagePtr &, int);

  private:
    InfoLogger::InfoLogger mLogger;       ///< Logger object
};

} // namespace Core
} // namespace ControlFairMQ
} // namespace AliceO2

#endif // ALICEO2_CONTROLFAIRMQ_CORE_RECEIVER_H
