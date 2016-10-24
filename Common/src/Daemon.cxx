#include <Common/Daemon.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>



static int DaemonsShutdownRequest=0;      // set to 1 to request termination, e.g. on SIGTERM/SIGQUIT signals
static void signalHandler(int){
  DaemonsShutdownRequest=1;
}


// todo: boost exceptions?

Daemon::Daemon(int argc, char * argv[]) {
  isInitialized=0;

  try {
    // set default configuration
    setDefaultConfiguration();

    // parse command line parameters
    int option;
    while((option = getopt(argc, argv, "z:o:")) != -1){
        switch(option) {

        case 'z': 
          log.info("Using configuration %s",optarg);
          try {
            config.load(optarg);
          }
          catch (std::string err) {
            log.error("Failed to load configuration: %s",err.c_str());
            throw __LINE__;
          }
          break;
        
        case 'o':
          // pass-through option in the format key=value
          log.info("option %s",optarg);
          break;
        
        default:
          throw __LINE__;
        }

    }
 

    // load configuration

    //myLog.setLogFile(... from log config);

    log.info("Daemon starting");

    // set daemon user name
    if (userName.length()>0) {
      int success=0;
      struct passwd *passent;
      passent=getpwnam(userName.c_str());
      if (passent!=NULL) {
        setuid(passent->pw_uid);
        // check setuid worked as expected
        if (getuid()==passent->pw_uid) {
          success=1;
        }
      }
      if (!success) {
        log.error("Failed to set user = %s",userName.c_str());
        throw __LINE__;
      } else {
        log.info("Changed user to %s",userName.c_str());
      }
    }

    // become a daemon, if configured to do so
    if (!isInteractive) {
      if(daemon(0,0) == -1){
        log.error("Could not become daemon: %s",strerror(errno));
        throw __LINE__;
      }
      log.info("Now running in the background");
    }

    // configure signal handlers for clean exit
    struct sigaction signalSettings;
    bzero(&signalSettings,sizeof(signalSettings));
    signalSettings.sa_handler=signalHandler;
    sigaction(SIGTERM,&signalSettings,NULL);
    sigaction(SIGQUIT,&signalSettings,NULL);
    sigaction(SIGINT,&signalSettings,NULL);

    isInitialized=1;
    log.info("Daemon initialized");
  }
  catch (int errLine) {
    log.error("Failed to initialize daemon - error %d",errLine);
  }

}

Daemon::~Daemon() {
  log.info("Daemon exiting");
}

void Daemon::setDefaultConfiguration() {
  isInteractive=1;
  idleSleepTime=1000000;
  userName="";
}

// This is the main daemon loop
// The doLoop() [custom] method is called continuously until one of the following conditions occur:
//   - global flag DaemonsShutdownRequest is set (e.g. by signal handler)
//   - doLoop() returns with status Error or Completed
// A short pause is done between 2 consecutive calls if status Idle is received.

int Daemon::run() {
  if (!isInitialized) return -1;
  
  for (;;) {
    LoopStatus status=doLoop();
    if (DaemonsShutdownRequest) {
      log.info("Exit requested");
      break;      
    }
    if (status == LoopStatus::Idle) {
      usleep(idleSleepTime);
    } else if (status == LoopStatus::Error) {
      log.error("Error detected");
      break;
    } else if (status == LoopStatus::Completed) {
      log.info("Work completed");
      break;
    }    
  }
  return 0;
}

Daemon::LoopStatus Daemon::doLoop() {
  printf(".");
  fflush(stdout);
  return LoopStatus::Idle;
}

bool Daemon::isOk() {
  return isInitialized;
}
