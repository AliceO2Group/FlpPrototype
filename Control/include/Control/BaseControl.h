#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <memory>

class RuntimeControlledObjectPrivate;
class ControlStateMachine;
class Directory;


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


class RuntimeControlEngine {
  public:
  RuntimeControlEngine();
  ~RuntimeControlEngine();

  int registerObject(RuntimeControlledObject *o);
  
  private:
  class Impl;
  std::unique_ptr<Impl> pImpl;
};
