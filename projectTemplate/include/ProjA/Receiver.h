///
/// @file    Receiver.h
/// @author  Barthelemy von Haller
///

#ifndef PROJECT_TEMPLATE_HELLO_RECEIVER_H
#define PROJECT_TEMPLATE_HELLO_RECEIVER_H

#include <FairMQDevice.h>
#include <TMessage.h>

namespace AliceO2 {
namespace ProjectTemplate {
namespace ProjA {

class TestTMessage : public TMessage
{
  public:
    TestTMessage(void *buf, Int_t len)
      : TMessage(buf, len)
    {
      ResetBit(kIsOwner);
    }
};

class Receiver : public FairMQDevice
{
  public:
    Receiver();

    virtual ~Receiver();

  protected:
    virtual void Run() override;

};

} // namespace Hello
} // namespace ProjectTemplate
} // namespace AliceO2

#endif // PROJECT_TEMPLATE_HELLO_RECEIVER_H
