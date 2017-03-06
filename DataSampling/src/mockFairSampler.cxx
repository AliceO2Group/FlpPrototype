///
/// \file    mockFairSampler.cxx
/// \author  Barthelemy von Haller
///

// std
#include <iostream>
#include <algorithm>
// boost
#include <boost/program_options.hpp>
#include <DataFormat/DataBlock.h>
// o2
#include "Common/signalUtilities.h"
// datasampling
#include "DataSampling/Version.h"
#include "DataSampling/SamplerFactory.h"
#include "DataSampling/FairSampler.h"

using namespace std;
namespace po = boost::program_options;

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

  unsigned int i = 0;
  AliceO2::DataSampling::FairSampler sampler;

  while (keepRunning /*&& i < 100*/) {

    std::vector<std::shared_ptr<DataBlockContainer>> *blocks = sampler.getData();

    if (blocks == nullptr) {
      continue;
    }
    cout << "--> block received : " << endl;
    if (blocks->size() > 0) {
      if (blocks->at(0) != nullptr) {
        cout << "    id : " << blocks->at(0)->getData()->header.id << endl;
        cout << "    blockType : " << std::hex << blocks->at(0)->getData()->header.blockType << endl;
        cout << "    headerSize : " << std::dec << blocks->at(0)->getData()->header.headerSize << endl;
        cout << "    payload size : " << std::dec << blocks->at(0)->getData()->header.dataSize << endl;
      } else {
        cout << "     Container pointer invalid" << endl;
      }
    } else {
      cout << "    Empty vector!" << endl;
    }

    sampler.releaseData();
    i++;
  }

  return EXIT_SUCCESS;
}
