#include "SimpleLog.h"
#include <string>

#include <Common/Configuration.h>


class Daemon {
  public:
  Daemon(int argc=0, char * argv[]=nullptr);
  virtual ~Daemon();
 
  enum LoopStatus {Ok, Idle, Error, Completed}; 

  // Function called periodically, until Error returned or external exit request.
  // If Idle, a sleep time is inserted between consecutive calls.
  virtual LoopStatus doLoop();                
  
  
  int run();
  

  protected:
  
  SimpleLog log;
  ConfigFile config;

  bool isOk();

  private:

  void setDefaultConfiguration();

  // configuration parameters
  bool isInteractive;
  int idleSleepTime;     // sleep time in microseconds when doLoop() idle
  std::string userName;  // user name under which should run the process
  
  // status
  bool isInitialized;  // 1 when daemon ok, 0 when error (failed to initialize, etc)
};



/* TODO

configure process name, user name, log file (or system),
possibly load config from given name

or from config object (all keys like daemon.userName, daemon.isInteractive, etc)

*/
