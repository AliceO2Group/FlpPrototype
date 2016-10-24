///
/// @file    Thread.h
/// @author  Sylvain Chapeland
///

#ifndef COMMON_THREAD_H
#define COMMON_THREAD_H


#include <chrono>
#include <string>
#include <atomic>
#include <thread>

namespace AliceO2 {
namespace Common {

/// @brief   Class to implement controllable looping threads
/// User just needs to overload the "doLoop()" method  or to provide a callback function to the base class constructor
/// @author   Sylvain Chapeland
class Thread {
  public:
     enum CallbackResult {Ok,Idle,Done,Error};
     
    /// Constructor
    /// @param[in]   vLoopCallback    Pointer to user-defined function called periodically. (optional)
    /// @param[in]   vLoopArg         Pointer to argument passed to user-defined function, if any.
    /// @param[in]   vThreadName      Name to be used to identify this thread (for debug printouts)
    /// @param[in]   loopSleepTime    Idle sleep time (in microseconds), when loop callback function/method returns idle. This is the maximum time between 2 calls.  
    Thread(Thread::CallbackResult (*vLoopCallback)(void *) = NULL , void *vLoopArg = NULL, std::string vThreadName = "", int loopSleepTime=1000);

    /// Destructor
    ~Thread();

    /// start thread loop
    void start();
    /// request thread termination
    void stop();
    /// wait thread termination (blocking call)
    void join();

    /// get thread name
    // @returns   name of the thread, as defined at construct time.
    std::string getName();
 
    
  private:
    std::atomic<int> shutdown;    // flag set to 1 to request thread termination
    std::atomic<int> running;     // flag set to 1 when thread running
    std::thread *theThread;

    std::string name;   // name of the thread, used in debug printouts
    int loopSleepTime;  // sleep time between 2 loop calls
    
    CallbackResult (*loopCallback)(void *);  // callback provided at create time
    void *loopArg;                // arg to be passed to callback function
   
    CallbackResult doLoop();   // function called at each thread iteration. Returns a result code.
    static void threadMain(Thread *e); // this is the (internal) thread entry point
    
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> t0; // time of reset
    double tmax; // duration between reset and timeout condition, in seconds
};

} // namespace Common
} // namespace AliceO2

#endif // COMMON_THREAD_H
