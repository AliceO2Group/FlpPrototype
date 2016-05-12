#include <iostream>
#include <chrono>
#include "Monitoring/Collector.h"

namespace Monitoring = AliceO2::Monitoring;

int main() {
       	
	// create monitoring object
	// pass filepath to ApMon configuration as argument
	Monitoring::Core::Collector *collector = new Monitoring::Core::Collector("../config/apmon.conf");

	// generate current timestamp
	std::chrono::time_point<std::chrono::system_clock> timestamp = std::chrono::system_clock::now();
	
	// now send an application specific metric
	// 10 is the value 
	// myCrazyMetric is the name of the metric
	collector->send(10, "myCrazyMetric", timestamp);
}