#include <StateMachine.h>

#include <vector>
#include <thread>
#include <atomic>
#include <stdio.h>
#include <unistd.h>

typedef Directory* t_ControlSystemHandle ;

class RuntimeControlledObjectPrivate;
class ControlStateMachine;

// Implementation of a typical O2 control state machine
// This is a class meant to be inherited for all objects to be controlled.


// - t_states defines the possible sates: standby, ready runnning paused error
// - The execute*() methods are called when corresponding command is received.
//   If an exception is thrown, the end state is error. Otherwise, it is the state defined in the command/state transition diagram.
// - iterateRunning() is called continuously in a loop in the state 'running'. It should return briefly (<1second).
// - iterateCheck() is called periodically in all states, in order to allow
//   self-triggered state change to error. If an exception is thrown, the object goes to 'error'.
// - getState / setState allow to retrieve and change the current object state.
//   The new state is actually propagated only when the execute* or iterate* method returns.
// - getName: get the name of the object. Needed to uniquely identify the instance, so that it can
//   configure accordingly.


// passive class, to be activated in a state machine engine
// this is a placeholder for user-defined actions

typedef enum {undefined,standby,ready,running,paused,error} t_State;
 

class RuntimeControlledObject {
  public:
  RuntimeControlledObject(const std::string objectName);
  ~RuntimeControlledObject();  
  
  
  const std::string getName();
  
  t_State getState();
   
  virtual int executePrepare(); // to go from standby to ready
  virtual int executeReset();   // to go from ready/error to standby
  virtual int executeStart();   // to go from ready to running
  virtual int executeStop();    // to go from running/paused to ready
  virtual int executePause();   // to go from running to paused
  virtual int executeResume();  // to go from paused to running
  
  virtual int iterateRunning();  // called continuously in state 'running'
  virtual int iterateCheck();    // called periodically in any state
  
  private:
  RuntimeControlledObjectPrivate *dPtr;
  
  friend class ControlStateMachine;
};







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
  return 0;
}

int RuntimeControlledObject::iterateCheck() {
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
  ControlStateMachine *stateMachine;
  std::string command;
}

// todo: handle exceptions (what types?) of RuntimeControlledObject

class ControlStateMachine:public ControlObject {
  public:

  ControlStateMachine(RuntimeControlledObject *objectPtr, Directory * dir ):ControlObject(objectPtr->getName(),dir) {
    mObject=objectPtr;
    updateState(t_State::standby);
  };
  
  ~ControlStateMachine() {
  };
  
  // this is the callback triggered by the ControlObject
  void executeCommand(const std::string command){
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
};



// A state machine engine to control/activate the RuntimeControlledObject instances at runtime
// instead of having a single 'control thread' per object, there is a single engine to control them in
// parallel: trigger action on commands and advertise state
// Can be implemented by a pool of threads.

// Can itself advertise a "master" control object through which one can control all sub-objects

class RuntimeControlEngine {
  public:
  RuntimeControlEngine();
  ~RuntimeControlEngine();

  int registerObject(RuntimeControlledObject *o);
  
  private:
  std::vector<ControlStateMachine *> mStateMachines;
  std::thread *mEngineThread;

  std::atomic<int> exitRequest;
  static void startThread(void *arg);
  
  Directory *mDirectory;
};




RuntimeControlEngine::RuntimeControlEngine() {
  exitRequest=0;
  mEngineThread=new std::thread(startThread,this);

  DirectoryConfig dCfg("localhost:2181");
  mDirectory=new Directory(dCfg);
}

RuntimeControlEngine::~RuntimeControlEngine() {
  exitRequest=1;
  mEngineThread->join();
  delete mEngineThread;
  delete mDirectory;
}

int RuntimeControlEngine::registerObject(RuntimeControlledObject *o) {

  ControlStateMachine *newSM= new ControlStateMachine(o, mDirectory);

  mStateMachines.push_back(newSM);
  return 0;
}


void RuntimeControlEngine::startThread(void *arg) {
  
  RuntimeControlEngine *dPtr;
  dPtr=(RuntimeControlEngine *)arg;
  
  for(;;) {
    sleep(1);
    if (dPtr->exitRequest) {break;}    
  }
}



// share a thread for doLoop and executeCommands?
// or run standalone ?






int main(int argc, char **argv) {
  RuntimeControlledObject o("test");
/*  RuntimeControlEngine *e;
  
  e=new RuntimeControlEngine();
  
  sleep(3);
  delete e;
*/
 
  RuntimeControlEngine e;
  e.registerObject(&o);
  sleep(1000);
  return 0;
}
