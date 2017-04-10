#include "ControlStateMachine/StateMachine.h"
#include <Common/Fifo.h>
#include <Common/MakeUnique.h>

#include <vector>
#include <thread>
#include <atomic>
#include <stdio.h>
#include <unistd.h>
#include <boost/thread/mutex.hpp>

#include "ControlStateMachine/BaseControl.h"

typedef Directory* t_ControlSystemHandle ;







class RuntimeControlledObjectPrivate {
  RuntimeControlledObjectPrivate(const std::string objectName) {
    mCurrentState=t_State::undefined;
    mName=objectName;
  }
  
  ~RuntimeControlledObjectPrivate() {
  }
  
  friend class RuntimeControlledObject;
  friend class ControlStateMachine;

  private:
     t_State mCurrentState;
     std::string mName;
     int getState(t_State &currentState) {
       currentState=mCurrentState;
       return 0;
     }
     void setState(t_State newState) {
       mCurrentState=newState;
     }
 
};

RuntimeControlledObject::RuntimeControlledObject(const std::string objectName){
  dPtr=new RuntimeControlledObjectPrivate(objectName);  
  if (dPtr==NULL) {
    throw __LINE__;
  }
}

RuntimeControlledObject::~RuntimeControlledObject(){
  if (dPtr!=NULL) {
    delete dPtr;
    dPtr=NULL;
  }

}




t_State RuntimeControlledObject::getState() {
  return dPtr->mCurrentState;
}


const std::string RuntimeControlledObject::getName() {
  return dPtr->mName;
}


int RuntimeControlledObject::executePrepare() {
  return 0;
}

int RuntimeControlledObject::executeReset() {
  return 0;
}

int RuntimeControlledObject::executeStart() {
  return 0;
}

int RuntimeControlledObject::executeStop() {
  return 0;
}

int RuntimeControlledObject::executePause() {
  return 0;
}

int RuntimeControlledObject::executeResume() {
  return 0;
}

int RuntimeControlledObject::iterateRunning() {
  printf("step run\n");
  sleep(1);
  return 0;
}

int RuntimeControlledObject::iterateCheck() {
  //printf("check\n");
  return 0;
}


t_State getStateFromString(const std::string s) {
  if (s=="STANDBY") {
    return t_State::standby;
  } else if (s=="READY") {
    return t_State::ready;
  } else if (s=="RUNNING") {
    return t_State::running;
  } else if (s=="PAUSED") {
    return t_State::paused;
  } else if (s=="ERROR") {
    return t_State::error;
  }
  return t_State::undefined;
}
const std::string getStringFromState(t_State s) {
  if (s==t_State::standby) {
    return "STANDBY";
  } else if (s==t_State::ready) {
    return "READY";
  } else if (s==t_State::running) {
    return "RUNNING";
  } else if (s==t_State::paused) {
    return "PAUSED";
  } else if (s==t_State::error) {
    return "ERROR";
  }
  return "UNDEFINED";
}



// structure to hold a command execution request
class CommandRequest {
  public:
  ControlStateMachine *stateMachine;
  std::string command;
};

// todo: handle exceptions (what types?) of RuntimeControlledObject

class ControlStateMachine:public ControlObject {
  public:

  ControlStateMachine(RuntimeControlledObject *objectPtr, Directory * dir ):ControlObject(objectPtr->getName(),dir) {
    mObject=objectPtr;
    updateState(t_State::standby);
    
    pendingCommands=std::make_unique<AliceO2::Common::Fifo<CommandRequest*>>(1);
  };
  
  ~ControlStateMachine() {
  };
  
  // this is the callback triggered by the ControlObject
  void executeCommand(const std::string command){
    printf("command received: %s\n",command.c_str());
    //processStateTransition(command);
    
    //push to command queue
    CommandRequest *newCommand=new CommandRequest();
    newCommand->stateMachine=this;
    newCommand->command=command;
    if (pendingCommands->push(newCommand)) {
      printf("%s - already busy with %d pending command(s)",mObject->getName().c_str(),pendingCommands->getNumberOfUsedSlots());
      CommandRequest *currentCommand=nullptr;
      pendingCommands->front(currentCommand);
      if (currentCommand!=nullptr) {
        printf("= %s",currentCommand->command.c_str());
      }
      printf("\n");
      delete newCommand;
    }
  }
  
  // this is the actual method for command execution,
  // which is called later by a dedicated execution thread
  // todo: make sure another command not being executed already  
  // todo: push command to a pool of "execute threads" to parallelize actions
  void processStateTransition(const std::string command){
    int err=0;
    int invalidCommand=0;
    
    t_State currentState=mObject->getState();
    t_State newState=currentState;
    
    printf("Object: %s - executing command %s in state %s\n",mObject->getName().c_str(),command.c_str(),getStringFromState(currentState).c_str());
    
    // STANDBY
    if (currentState==t_State::standby) {
      if (command=="prepare") {
        err=mObject->executePrepare();
        if (!err) {
          newState=t_State::ready;
        } else {
          newState=t_State::error;
        }
      } else {
        invalidCommand=1;
      }        

    // READY
    } else if (currentState==t_State::ready) {
      if (command=="start") {
        err=mObject->executeStart();
        if (!err) {
          newState=t_State::running;
        } else {
          newState=t_State::error;
        }
      } else if (command=="reset") {
        err=mObject->executeReset();
        if (!err) {
          newState=t_State::standby;
        } else {
          newState=t_State::error;
        }
      } else {
        invalidCommand=1;
      }                
    
    // RUNNING
    } else if (currentState==t_State::running) {
      if (command=="stop") {
        err=mObject->executeStop();
        if (!err) {
          newState=t_State::ready;
        } else {
          newState=t_State::error;
        }
      } else if (command=="pause") {
        err=mObject->executePause();
        if (!err) {
          newState=t_State::paused;
        } else {
          newState=t_State::error;
        }
      } else {
        invalidCommand=1;
      }                

    // PAUSED
    } else if (currentState==t_State::paused) {
      if (command=="resume") {
        err=mObject->executeResume();
        if (!err) {
          newState=t_State::running;
        } else {
          newState=t_State::error;
        }
      } else if (command=="stop") {
        err=mObject->executeStop();
        if (!err) {
          newState=t_State::ready;
        } else {
          newState=t_State::error;
        }
      } else {
        invalidCommand=1;
      }                

    // ERROR
    } else if (currentState==t_State::error) {
      if (command=="reset") {
        err=mObject->executeReset();
        if (!err) {
          newState=t_State::standby;
        } else {
          newState=t_State::error;
        }
      } else {
        invalidCommand=1;
      }
    
    // other
    } else {
      invalidCommand=1;
    }
    
    if (invalidCommand) {
      printf("Object: %s - invalid command %s received in state %s\n",mObject->getName().c_str(),command.c_str(),getStringFromState(currentState).c_str());
    } else {
      printf("Object: %s - command %s executed in state %s. New state: %s\n",mObject->getName().c_str(),command.c_str(),getStringFromState(currentState).c_str(),getStringFromState(newState).c_str());
      updateState(newState);
    }
  };
  
  private:
    RuntimeControlledObject *mObject;  // stores pointer to instance of object implementing the commands
    
    void updateState(t_State s) {
      setState(getStringFromState(s));
      // todo: check if error
      mObject->dPtr->setState(s);
      printf("Object: %s - updating state = %s\n",mObject->getName().c_str(),getStringFromState(s).c_str());
    }
    
    std::unique_ptr<AliceO2::Common::Fifo<CommandRequest*>> pendingCommands;

  friend class RuntimeControlEngine;
};



// A state machine engine to control/activate the RuntimeControlledObject instances at runtime
// instead of having a single 'control thread' per object, there is a single engine to control them in
// parallel: trigger action on commands and advertise state
// Can be implemented by a pool of threads.

// Can itself advertise a "master" control object through which one can control all sub-objects




class RuntimeControlEngine::Impl {
  friend class RuntimeControlEngine;
  private:
  
  std::vector<std::unique_ptr<ControlStateMachine>> mStateMachines;
  std::unique_ptr<std::thread> mEngineThread;

  std::atomic<int> exitRequest;
  static void startThread(void *arg);
  
  Directory *mDirectory;
  
  boost::mutex mMutex;
  
};








RuntimeControlEngine::RuntimeControlEngine() {
  pImpl=std::make_unique<RuntimeControlEngine::Impl>();
  
  pImpl->mEngineThread=nullptr;
//  pImpl->pendingCommands=nullptr;
  pImpl->mDirectory=nullptr;
  
  
  pImpl->exitRequest=0;
  pImpl->mEngineThread=std::make_unique<std::thread>(RuntimeControlEngine::Impl::startThread,this);

  DirectoryConfig dCfg("localhost:2181");
  pImpl->mDirectory=new Directory(dCfg);
}

RuntimeControlEngine::~RuntimeControlEngine() {
  pImpl->exitRequest=1;
  pImpl->mEngineThread->join();
  delete pImpl->mDirectory;
}

int RuntimeControlEngine::registerObject(RuntimeControlledObject *o) {

  // TODO: lock ! can not register before running
  // or allow registerObj only when not running, before starting engine
  
  pImpl->mMutex.lock();
  pImpl->mStateMachines.push_back(std::make_unique<ControlStateMachine>(o, pImpl->mDirectory));
  pImpl->mMutex.unlock();
  
  // TODO: one thread per object?
  
  
  return 0;
}


void RuntimeControlEngine::Impl::startThread(void *arg) {
  
  RuntimeControlEngine *dPtr;
  dPtr=(RuntimeControlEngine *)arg;
  
  int isActive=0;
  
  for(;;) {
    isActive=0;
    
    dPtr->pImpl->mMutex.lock();
    
    // process pending commands    
    for (auto& ctrlObj : dPtr->pImpl->mStateMachines) {

      // check for commands in queue
      CommandRequest *newCommand=nullptr;
      ctrlObj->pendingCommands->front(newCommand);
      if (newCommand!=nullptr) {
        printf("starting processing %s : %s\n",ctrlObj->mObject->getName().c_str(),newCommand->command.c_str());        
        newCommand->stateMachine->processStateTransition(newCommand->command);
        printf("done processing %s : %s\n",ctrlObj->mObject->getName().c_str(),newCommand->command.c_str());        
        ctrlObj->pendingCommands->pop(newCommand);
        delete newCommand;
        isActive=1;
      }
      
      // execute periodic actions, as defined for corresponding state
      if (ctrlObj->mObject->getState()==t_State::running) {
        int err=ctrlObj->mObject->iterateRunning();
        if (err) {
          ctrlObj->updateState(t_State::error);
        }
      }
      
      
      // execute object periodic check, in any state
      int err=ctrlObj->mObject->iterateCheck();
      if (err) {
        ctrlObj->updateState(t_State::error);
      }
      
    }

    dPtr->pImpl->mMutex.unlock();

   
    if (!isActive) {
      usleep(1000);
    }
    if (dPtr->pImpl->exitRequest) {break;}    
  }
}



// share a thread for doLoop and executeCommands?
// or run standalone ?




/*

int main(int argc, char **argv) {
  RuntimeControlledObject o("test");
//  RuntimeControlEngine *e;
  
//  e=new RuntimeControlEngine();
  
//  sleep(3);
//  delete e;

 
  RuntimeControlEngine e;
  e.registerObject(&o);
  sleep(1000);
  return 0;
}
*/
/*
// TODO

specify if controlled object uses self-thread or can use a pool of thread

*/
