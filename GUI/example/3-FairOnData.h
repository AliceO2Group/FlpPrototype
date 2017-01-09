#include "FairMQDevice.h"

class FairOnData : public FairMQDevice
{
  public:
    FairOnData();
    virtual ~FairOnData();

  protected:
    bool HandleNotification(FairMQMessagePtr&, int);
};
