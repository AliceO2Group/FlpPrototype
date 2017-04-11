/// \file BasicThread.h
/// \brief Definition of BasicThread class
///
/// \author Pascal Boeschoten (pascal.boeschoten@cern.ch)

#ifndef ALICEO2_FLPPROTOTYPE_COMMON_BASICTHREAD_H_
#define ALICEO2_FLPPROTOTYPE_COMMON_BASICTHREAD_H_

#include <atomic>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <string>
#include <thread>

namespace AliceO2
{
namespace Common
{

/// Simple thread class that executes a function in a new thread. The function takes an atomic flag which indicates
/// the function should stop its work.
class BasicThread
{
  public:

    /// Start thread
    /// \param stopFlag Pointer to flag indicating the function should stop
    void start(std::function<void(std::atomic<bool>* stopFlag)> function)
    {
      join();
      mStopFlag = false;
      mThread = std::thread(function, &mStopFlag);
    }

    void stop()
    {
      mStopFlag = true;
    }

    void join()
    {
      stop();
      if (mThread.joinable()) {
        mThread.join();
      }
    }

    ~BasicThread()
    {
      try {
        join();
      }
      catch (const std::system_error& e) {
        std::cout << "Failed to join thread: " << e.what() << '\n';
      }
      catch (const std::exception& e) {
        std::cout << "Unexpected exception while joining thread: " << e.what() << '\n';
      }
    }

  private:
    /// Thread object
    std::thread mThread;

    /// Flag to stop the thread
    std::atomic<bool> mStopFlag;
};

} // namespace Common
} // namespace AliceO2

#endif // ALICEO2_FLPPROTOTYPE_COMMON_BASICTHREAD_H_
