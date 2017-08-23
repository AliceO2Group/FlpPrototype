///
/// \file    dataFileProducerMain.cxx
/// \author  Barthelemy von Haller
///

// std
#include <iostream>
#include <algorithm>
// boost
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <Common/DataBlock.h>
// datasampling
#include "DataSampling/FileSampler.h"
#include "DataSampling/Version.h"
#include "DataSampling/DataBlockProducer.h"

using namespace std;
namespace po = boost::program_options;

/**
 * Writes a dataset with a number of blocks to file.
 * @param argc
 * @param argv
 * @return
 */
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
  int blocks = 10;
  if (vm.count("blocks")) {
    blocks = vm["blocks"].as<int>();
  }

  if (boost::filesystem::exists(outputFile)) {
    std::cout << "File already exists" << std::endl;
    return EXIT_FAILURE;
  }

  AliceO2::DataSampling::DataBlockProducer producer;
  std::fstream file(outputFile, std::ios::out | std::ios::binary | std::ios::trunc);
  auto set = producer.getDataSet((unsigned int) blocks);
  for (int i = 0; i < blocks; i++) {
    auto block = set->at(i);
    file.write((char *) &block->getData()->header, sizeof(DataBlockHeaderBase));
    file.write(block->getData()->data, block->getData()->header.dataSize);
    file.close();
  }
  cout << "Done" << endl;

  return EXIT_SUCCESS;
}

