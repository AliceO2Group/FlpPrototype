#include "Common/BasicThread.h"
#include <chrono>
#include <thread>

#define BOOST_TEST_MODULE BasicThread
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestBasicThread)
{
  AliceO2::Common::BasicThread thread;
  std::atomic<int64_t> number;
  thread.start([&](std::atomic<bool>* stopFlag) {
    while (!stopFlag->load(std::memory_order_relaxed)) {
      number++;
    }
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  thread.join();
  BOOST_CHECK(number > 0);
}
