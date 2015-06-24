///
/// \file    dataDump.cxx
/// \author  Barthelemy von Haller
///

// std
#include <iostream>
// boost
#include <boost/program_options.hpp>
// datasampling
#include "datasampling/Sampler.h"
#include "datasampling/Version.h"

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
    std::cout << "HelloRunner version " << AliceO2::DataSampling::Version::getString() << std::endl;
    return EXIT_SUCCESS;
  }
  // rev
  if (vm.count("rev")) {
    std::cout << "SVN revision : " << AliceO2::DataSampling::Version::getRevision() << std::endl;
    return EXIT_SUCCESS;
  }

  // Actual "work"
  AliceO2::DataSampling::Sampler sampling;
  sampling.setLocation("file:test.data");
  sampling.setDataFormat(AliceO2::DataSampling::DataFormat::Raw);
  cout << "data format : " << sampling.getDataFormat() << endl;

  return EXIT_SUCCESS;
}
