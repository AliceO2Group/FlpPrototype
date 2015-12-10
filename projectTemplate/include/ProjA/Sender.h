///
/// @file    Sender.h
/// @author  Barthelemy von Haller
///

#ifndef PROJECT_TEMPLATE_HELLO_SENDER_H
#define PROJECT_TEMPLATE_HELLO_SENDER_H

#include "FairMQDevice.h"
#include "TH1.h"


/// @brief    Here you put a short description of the namespace
/// Extended documentation for this namespace
/// @author  	Barthelemy von Haller
namespace AliceO2 {
namespace ProjectTemplate {
namespace ProjA {

/// @brief   Here you put a short description of the class
/// Extended documentation for this class.
/// @author 	Barthelemy von Haller
class Sender : public FairMQDevice
{
  public:
    Sender();
    virtual ~Sender();

    void go();
    void myStop();

  protected:
    static void CustomCleanup(void *data, void *object);
    static void CustomCleanupTMessage(void *data, void *object);
    virtual void Run() override;

  private:
    std::string mText;
    bool mRun;
    TH1* mHisto;
};

} // namespace Hello
} // namespace ProjectTemplate
} // namespace AliceO2

#endif // PROJECT_TEMPLATE_HELLO_SENDER_H
