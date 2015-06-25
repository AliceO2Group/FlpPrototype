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
#include "datasampling/FileSampler.h"
#include "datasampling/Version.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char* argv[])
{
  // Arguments parsing
  po::variables_map vm;
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Produce help message.")("version,v", "Show program name/version banner and exit.")(
      "rev", "Print the SVN revision number")("location,l", po::value<string>(), "The URI of the data location")("format,f", po::value<string>(), "");
  po::store(parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  // help
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }
  // version
  if (vm.count("version")) {
    std::cout << "HelloRunner version " << AliceO2::DataSampling::Version::getString() << std::endl;
    return EXIT_SUCCESS;
  }
  // rev
  if (vm.count("rev")) {
    std::cout << "SVN revision : " << AliceO2::DataSampling::Version::getRevision() << std::endl;
    return EXIT_SUCCESS;
  }

  AliceO2::DataSampling::FileSampler sampling; // TODO we should not hardcode the fact that it is a file

  // location
  if (vm.count("location")) {
    sampling.setLocation(vm["location"].as<string>());
  } else {
    cerr << "Location must be specified." << endl;
    cout << desc << endl;
    return EXIT_FAILURE;
  }

  // format
  vector<string> allowedFormat {"Raw", "STF", "TF", "CTF", "AOD", "ESD"}; // TODO how to avoid duplicating information with the enum ?
  if(vm.count("format")) {
    if(find(allowedFormat.begin(), allowedFormat.end(), vm["format"].as<string>()) == allowedFormat.end()) {
      cerr << "Format must be one of those : Raw, STF, TF, CTF, AOD, ESD" << endl;
      return EXIT_FAILURE;
    }
    sampling.setDataFormat(vm["format"].as<string>());
  }

  return EXIT_SUCCESS;
}
