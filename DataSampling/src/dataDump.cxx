///
/// \file    dataDump.cxx
/// \author  Barthelemy von Haller
///

// std
#include <iostream>
#include <algorithm>
// boost
#include <boost/program_options.hpp>
// datasampling
#include "DataSampling/Version.h"
#include <DataSampling/SamplerFactory.h>
#include <Common/signalUtilities.h>
#include <boost/exception/diagnostic_information.hpp>

using namespace std;
namespace po = boost::program_options;
using namespace AliceO2::DataSampling;

int main(int argc, char *argv[])
{
  // Arguments parsing
  po::variables_map vm;
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Produce help message.")("version,v", "Show program name/version banner and exit.")(
    "rev", "Print the SVN revision number")("source,s", po::value<string>(), "fair://xxxx file://xxxx mock");
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

  string samplerName;
  string samplerAddress;
  if (vm.count("source")) {
    string source = vm["source"].as<string>();
    string prefix = "fair://";
    if (!source.compare(0, prefix.size(), prefix)) {
      samplerName = "FairSampler";
      samplerAddress = source.substr(prefix.size());
    }
    prefix = "file://";
    if (!source.compare(0, prefix.size(), prefix)) {
      samplerName = "FileSampler";
      samplerAddress = source.substr(prefix.size());
    }
    prefix = "mock";
    if (!source.compare(0, prefix.size(), prefix)) {
      samplerName = "MockSampler";
      samplerAddress = "";
    }
  } else {
    cerr << "Location must be specified." << endl;
    cout << desc << endl;
    return EXIT_FAILURE;
  }

  try {
    unique_ptr<SamplerInterface> sampler = SamplerFactory::create(samplerName);

    handler_interruption_message = "";
    signal(SIGINT, handler_interruption);
    unsigned int i = 0;
    cout << "\n\n\n\n\n" << endl; // shift for the output
    while (keepRunning) {
      std::vector<std::shared_ptr<DataBlockContainer>> *blocks = sampler->getData();

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

      sampler->releaseData();
    }
    cout << "\033[4B"; // down 4 lines
  } catch (std::string const &e) {
    cerr << e << endl;
  } catch (...) {
    string diagnostic = boost::current_exception_diagnostic_information();
    std::cerr << "Unexpected exception, diagnostic information follows:\n" << diagnostic << endl;
    if (diagnostic == "No diagnostic information available.") {
      throw;
    }
  }
  return EXIT_SUCCESS;
}
