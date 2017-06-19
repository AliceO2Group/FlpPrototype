/// \file Daemon.h
/// \brief Helper class to create/run a daemon
///
/// \author Sylvain Chapeland, CERN

#include "SimpleLog.h"
#include <string>

#include <Common/Configuration.h>


// class to define parameters for daemon runtime behavior.
// default values are defined.
class DaemonConfigParameters {
public:
  int isInteractive=0;           // flag set when process to be kept in foreground
  int idleSleepTime=100000;     // sleep time in microseconds when doLoop() idle
  std::string userName;          // user name under which should run the process. Not changed if empty.
  int redirectOutput=0;          // flag set to redirect stdout/stderr to /dev/null
  std::string logFile;           // log file (leave empty to keep stdout/stderr)
};


// a class to initialize and run a daemon process
// - user doLoop() called iteratively
// - exit when doLoop() returns Completed or Error status,
// - exit when termination signal received: SIGINT (CTRL+C), SIGQUIT, SIGTERM
// This class should be derived with a custom doLoop() periodic main loop.
class Daemon {
  public:

  // constructor
  // takes as input main(argc,argv) command line parameters, and custom configuration options.
  // accepted command line parameters:
  // -z configFileURI : load provided config file (e.g. file:../.../daemon.cfg. See Configuration.h for URI specs)
  // -okey=value : set a given key/value pair parameter.
  //
  // Configuration options may are overwritten by those provided in [daemon] section of the config file, if any.
  // There is a 1-to-1 match between key names and members of the DaemonConfigParameters class.
  // i.e. user name running the daemon can be set by passing a pointer to a custom DaemonConfigParameters object,
  // but it is overwritten by value of [daemon] userName key in configuration file, if defined.
  Daemon(int argc=0, char * argv[]=nullptr, DaemonConfigParameters * =nullptr);
  
  // destructor
  virtual ~Daemon();

  // Entry point to start periodic loop. This is the main function.
  // doLoop() method is thereafter called continuously until conditions described below.
  // returns -1 (daemon failed to initialize), 0 (loop exits with success) or 1 (loop exists with error).
  int run();

  // main periodic loop return status.
  // doLoop() recalled immediatetly on Ok or with short delay on Idle (delay set in idleSleepTime parameter).
  // run() will return when doLoop() returns Error or Completed. Corresponding error codes are 1 and 0.
  // run() will also be interrupted (with success return code) on external exit requests.
  enum LoopStatus {Ok, Idle, Error, Completed}; 

  // Function called periodically, until Completed/Error returned or external exit request.
  // If returns Idle, a sleep time is inserted between consecutive calls.
  // If returns Ok, called again immediately.
  virtual LoopStatus doLoop();                

  
  protected:
  
  SimpleLog log;      // object for output logging.
  ConfigFile config;  // input configuration file, if any. Loaded if path provided on command line.

  // check daemon status (e.g. after constructor, before starting main loop by calling run(), to know if init success)
  bool isOk();

  
  private:

  DaemonConfigParameters params;    // configuration parameters
  bool isInitialized;               // init status. 1 when daemon ok, 0 when error (failed to initialize, etc)
};



/* TODO

configure process name, user name, log file (or system),
possibly load config from given name

or from config object (all keys like daemon.userName, daemon.isInteractive, etc)

*/
