///
/// \file    dataFileProducerMain.cxx
/// \author  Barthelemy von Haller
///

// std
#include <iostream>
#include <algorithm>
// boost
#include <boost/program_options.hpp>
// datasampling
#include "DataSampling/FileSampler.h"
#include "DataSampling/Version.h"
#include "DataSampling/DataBlockProducer.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char *argv[])
{
  // Arguments parsing
  po::variables_map vm;
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Produce help message.")
    ("version,v", "Show program name/version banner and exit.")
    ("rev", "Print the SVN revision number")
    ("size,s", po::value<int>(), "The size of the payload in bytes (default: 1024).")
    ("file,f", po::value<string>(), "Path and name to the output file (required).")
    ("blocks,b", po::value<int>(), "Number of blocks to store in the file (default: 10).");
  po::store(parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }
  if (vm.count("version")) {
    std::cout << "DataSampling version " << AliceO2::DataSampling::Version::getString() << std::endl;
    return EXIT_SUCCESS;
  }
  if (vm.count("rev")) {
    std::cout << "Git revision : " << AliceO2::DataSampling::Version::getRevision() << std::endl;
    return EXIT_SUCCESS;
  }
  string outputFile;
  if (vm.count("file")) {
    outputFile = vm["file"].as<string>();
  } else {
    cerr << "File must be specified." << endl;
    cout << desc << endl;
    return EXIT_FAILURE;
  }
  int size = 1024;
  if (vm.count("size")) {
    size = vm["size"].as<int>();
  }
  unsigned int blocks = 10;
  if (vm.count("blocks")) {
    blocks = vm["blocks"].as<unsigned int>();
  }

  AliceO2::DataSampling::DataBlockProducer producer;
  producer.saveToFile(outputFile, false);
  for (int i = 0; i < blocks-1; i++) {
    producer.regenerate();
    producer.saveToFile(outputFile, true);
  }

  return EXIT_SUCCESS;
}

