///
/// \file   FileSampler.cxx
/// \author Barthelemy von Haller
///

#include "FileSampler.h"

#include <fstream>
#include <memory.h>

using namespace std;

namespace AliceO2 {
namespace DataSampling {

FileSampler::FileSampler()
{
  // TODO Auto-generated constructor stub

}

FileSampler::~FileSampler()
{
  // TODO Auto-generated destructor stub
}

std::string FileSampler::getFilePath()
{
  return getLocation().substr(5);
}

DataBlock* FileSampler::getData(int timeout)
{
  cout << "DataSampling::getData " << endl;
  string filePath = getFilePath();
  cout << "filePath : " << filePath << endl;
  DataBlock *result = new DataBlock();
  ifstream file(filePath, ios::in | ios::binary);
  file.read((char*) &result->header, sizeof(DataBlockHeaderBase));
  cout << "blockType : " << std::hex << result->header.blockType << endl;
  cout << "headerSize : " << std::hex << result->header.headerSize << endl;
  cout << "payload size : " << std::hex << result->header.dataSize << endl;

  int dataSizeBytes = result->header.dataSize / 8;
  cout << "dataSizeBytes : " << dataSizeBytes << endl;
  char buffer[dataSizeBytes];
  file.read(buffer, dataSizeBytes);
  cout << "buffer : " << buffer << endl;
  file.close();
  result->data = new char[dataSizeBytes];
  memcpy (result->data, buffer, dataSizeBytes);
  cout << "data : " << result->data << endl;
  return result;
}
void FileSampler::releaseData()
{
  cout << "DataSampling::releaseData : " << endl;
}

} /* namespace Utilities */
} /* namespace AliceO2 */
