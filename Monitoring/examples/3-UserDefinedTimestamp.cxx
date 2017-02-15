///
/// \file 3-UserDefinedTimestamp.cxx
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include <iostream>
#include <chrono>
#include "Monitoring/Collector.h"

using Monitoring = AliceO2::Monitoring::MonitoringFactory;
using AliceO2::Monitoring::Metric;

int main() {
  // configure monitoring (only once per process), pass configuration path as parameter
  Monitoring::Configure("file:///home/awegrzyn/hackathon/Monitoring/examples/SampleConfig.ini");

  // current timestamp
  std::chrono::time_point<std::chrono::system_clock> timestamp = std::chrono::system_clock::now();
	
  // now send an application specific metric
  // 10 is the value timestamp
  // myCrazyMetric is the name of the metric
  //
  // 1. by copying all parameters
  Monitoring::Get().sendTimed(10, "myCrazyMetric", timestamp);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  Monitoring::Get().sendTimed(40, "myCrazyMetric", timestamp);
  
  // 2. by moving
  Monitoring::Get().send(Metric{10, "myCrazyMetric"}.setTimestamp(timestamp));
  std::this_thread::sleep_for(std::chrono::seconds(1));
  Monitoring::Get().send(Metric{40, "myCrazyMetric"}.setTimestamp(timestamp));
}
