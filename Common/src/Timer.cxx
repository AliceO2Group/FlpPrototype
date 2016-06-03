///
/// @file    Timer.cxx
/// @author  Sylvain Chapeland
///

#include "Common/Timer.h"


namespace AliceO2 {
namespace Common {

Timer::Timer() {
  reset(0);
}

Timer::~Timer(){
}

void Timer::reset(int timeout) {
   t0 = std::chrono::high_resolution_clock::now();
   tmax=timeout/1000000.0; 
}

void Timer::increment() {
  t0+=std::chrono::microseconds((int)(tmax*1000000.0));
}

int Timer::isTimeout() {
    std::chrono::duration<double> tdiff= std::chrono::high_resolution_clock::now() - t0;
    if (tdiff.count()>=tmax) {
      return 1;
    }
    return 0;
}

double Timer::getTime() {
    std::chrono::duration<double> tdiff= std::chrono::high_resolution_clock::now() - t0;
    return tdiff.count();
} 

} // namespace Common
} // namespace AliceO2
