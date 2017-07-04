///
/// \file    mockFairInjector.cxx
/// \author  Barthelemy von Haller
///

// std
#include <iostream>
#include <algorithm>
#include <vector>
// boost
#include <boost/program_options.hpp>
// o2
#include "Common/signalUtilities.h"
// datasampling
#include "DataSampling/Version.h"
#include "DataSampling/InjectorFactory.h"
//#include "DataSampling/InjectorDevice.h"
#include "DataSampling/DataBlockProducer.h"
#include "DataSampling/FairInjector.h"

using namespace std;
namespace po = boost::program_options;
using namespace AliceO2::DataSampling;

int main(int argc, char *argv[])
{
  // Arguments parsing
  po::variables_map vm;
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Produce help message.")("version,v", "Show program name/version banner and exit.")(
    "rev", "Print the SVN revision number");
  po::store(parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  // help
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }
  // version
  if (vm.count("version")) {
    std::cout << "DataSampling version " << AliceO2::DataSampling::Version::getString() << std::endl;
    return EXIT_SUCCESS;
  }
  // rev
  if (vm.count("rev")) {
    std::cout << "Git revision : " << AliceO2::DataSampling::Version::getRevision() << std::endl;
    return EXIT_SUCCESS;
  }

//  AliceO2::DataSampling::InjectorInterface *device = AliceO2::DataSampling::InjectorFactory::create("FairInjector");
  AliceO2::DataSampling::InjectorInterface *dataSamplingInjector = AliceO2::DataSampling::InjectorFactory::create("FairInjector");

  unsigned int i = 0;
  DataBlockProducer producer(true, 1024, false /*we take ownership*/);
  signal(SIGINT, handler_interruption);
  while (keepRunning) {
    std::vector<std::shared_ptr<DataBlockContainer>> blocks;

    producer.regenerate();
    DataBlock *block = producer.get();
    std::shared_ptr<DataBlockContainer> containerPtr = std::make_shared<DataBlockContainer>(block);
    blocks.push_back(containerPtr);

    producer.regenerate();
    DataBlock *block2 = producer.get();
    std::shared_ptr<DataBlockContainer> containerPtr2 = std::make_shared<DataBlockContainer>(block2);
    blocks.push_back(containerPtr2);

    dataSamplingInjector->injectSamples(blocks);

//    for (std::shared_ptr<DataBlockContainer> block_ptr : blocks) {
//      if (block_ptr->getData()->data) {
//        delete[] block_ptr->getData()->data;
//      }
//      delete block_ptr->getData(); // REALLY ??
//    }

    if ((i % 10) == 0) {
      std::cout << "\rNumber of blocks produced so far : " << i;
    }
    i++;
  }
  cout << "\nexiting" << endl;
  delete dataSamplingInjector;

  return EXIT_SUCCESS;
}
