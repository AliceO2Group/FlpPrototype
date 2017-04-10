/// @file    Sender.h
/// \brief   Definition of Sender class
///
/// @author  Vasco Barroso

#ifndef ALICEO2_CONTROLFAIRMQ_CORE_SENDER_H
#define ALICEO2_CONTROLFAIRMQ_CORE_SENDER_H

#include <FairMQDevice.h>
#include "InfoLogger/InfoLogger.hxx"

namespace AliceO2 {
/// ALICE O2 Control system tests with FairMQ
namespace ControlFairMQ {
/// Core features
namespace Core {
/// Test sender with FairMQ
///
/// Simple test sender to play with FairMQ
class Sender : public FairMQDevice
{
  public:
    /// Standard constructor
    Sender();

    /// Destructor
    virtual ~Sender();

    static void CustomCleanup(void* data, void* hint);

  protected:
    /// Cleanup method
    //static void CustomCleanup(void *data, void *object);
    /// Running state
    virtual void Run() override;

  private:
    /// Get logger object
    InfoLogger::InfoLogger& getLogger();

    std::string mText;
    bool mRun;
    InfoLogger::InfoLogger mLogger;       ///< Logger object
};

} // namespace Core
} // namespace ControlFairMQ
} // namespace AliceO2

#endif // ALICEO2_CONTROLFAIRMQ_CORE_SENDER_H
