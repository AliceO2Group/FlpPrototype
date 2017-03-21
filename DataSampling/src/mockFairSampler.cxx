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
  cout << "Blocks received so far : 0\n\n\n\n\n" << endl; // prepare output

  signal(SIGINT, handler_interruption);
  while (keepRunning) {
    std::vector<std::shared_ptr<DataBlockContainer>> *blocks = sampler.getData();

    if (blocks == nullptr) {
      continue;
    }
    cout << "\033[6A"; // up 6 lines
    cout << "Blocks received so far : \033[K" << ++i << endl;
    cout << "--> last block received : " << endl;
    if (blocks->size() > 0) {
      if (blocks->at(0) != nullptr) {
        cout << "    id : \033[K" << blocks->at(0)->getData()->header.id << endl;
        cout << "    blockType : \033[K" << std::hex << blocks->at(0)->getData()->header.blockType << endl;
        cout << "    headerSize : \033[K" << std::dec << blocks->at(0)->getData()->header.headerSize << endl;
        cout << "    payload size : \033[K" << std::dec << blocks->at(0)->getData()->header.dataSize << endl;
      } else {
        cout << "    Container pointer invalid" << endl;
      }
    } else {
      cout << "    Empty vector!" << endl;
    }

    sampler.releaseData();
  }

  return EXIT_SUCCESS;
}
