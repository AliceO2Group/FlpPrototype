///
/// \file    mockFairSampler.cxx
/// \author  Barthelemy von Haller
///

// std
#include <iostream>
#include <algorithm>
// boost
#include <boost/program_options.hpp>
// o2
#include "Common/signalUtilities.h"
// datasampling
#include "DataSampling/Version.h"
#include "DataSampling/SamplerFactory.h"
#include "DataSampling/FairSampler.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char* argv[])
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

  cout << "asdf" << endl;
//  DataBlock *block;
//std::string s;
//
//  while (keepRunning) {
//    s = sampler.getData();
////    block = sampler.getData();
//
//    if (!block) {
//      BOOST_ERROR("pointer is null");
//      return;
//    }
//    cout << "blockType : " << std::hex << block->header.blockType << endl;
//    cout << "headerSize : " << std::hex << block->header.headerSize << endl;
//    cout << "payload size : " << std::dec << block->header.dataSize << endl;
//    int dataSizeBytes = block->header.dataSize / 8;
//    BOOST_CHECK_GT(dataSizeBytes, 0);
//
//    sampler.releaseData();
//  }


  return EXIT_SUCCESS;
}
