///
/// \file   InjectorDevice.h
/// \author Barthelemy von Haller
///

#ifndef PROJECT_INJECTORDEVICE_H
#define PROJECT_INJECTORDEVICE_H

#include <FairMQDevice.h>
#include <vector>

namespace AliceO2 {
namespace DataSampling {

class InjectorDevice : public FairMQDevice
{
  public:
    InjectorDevice();
    virtual ~InjectorDevice();

    void sendData(std::string /*std::vector<DataBlockContainer*>*/ data);

  protected:
    virtual void Run();

//    std::vector<DataBlockContainer *> mDataBlocks;
    std::string *mData;
    bool mAvailableData; // to make sure that we don't try to send something that is not there
};

}
}

#endif //PROJECT_INJECTORDEVICE_H
