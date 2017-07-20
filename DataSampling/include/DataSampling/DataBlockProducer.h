///
/// \file    DataBlockProducer.h
/// \author  Barthelemy von Haller
///

#ifndef DATA_SAMPLING_DATABLOCKPRODUCER_H
#define DATA_SAMPLING_DATABLOCKPRODUCER_H

#include <DataFormat/DataBlock.h>

#include <iostream>
#include <random>
#include <DataFormat/DataSet.h>

namespace AliceO2 {
namespace DataSampling {

/**
 * DataBlockContainer that takes ownership of the payload.
 */
class MockDataBlockContainer : public DataBlockContainer
{

  public:
    MockDataBlockContainer()
    {
      data = new DataBlock();
      data->data = 0;
    }

    ~MockDataBlockContainer()
    {
      if (data->data != nullptr) {
        delete[] data->data;
      }
      delete data;
    }
};

/// \brief This class produces DataBlocks containing random data.
///
/// It can return the block in memory or save it to file. In the latter case, it can
/// append or truncate the file. The content of the payload is just the alphabet.
/// If the flag random is set to true in the constructor, the payload size follows a normal distribution
/// around 1024 bytes with a standard deviation of 256.
/// Usage :
/// \code
///  DataBlockProducer producer(false /*not random*/, payloadSize); // creates a first block
///  producer.get(); // get a the block but it is const. Copy if needed.
///  producer.regenerate(); // generate a new block
///  producer.saveToFile(path); // save the current block to a file
/// \endcode
/// \todo move to DataFormat package
///
/// \author Barthelemy von Haller
class DataBlockProducer
{
  public:
    /** \brief Create a data block producer and an initial data block.
     *
     * @param random use a random payloadSize or not
     * @param payloadSize Size of payload. Ignored if random=true.
     * @param leaveOwnership Whether the caller wants to leave ownership or take it.
     */
    DataBlockProducer(bool random = true, uint32_t payloadSize = 1024 /*bytes*/);
    /// \brief Destructor
    virtual ~DataBlockProducer();

    /**
     * \brief Get a new DataSetReference.
     * Payload size is either random or fixed depending on parameters passed to the constructor.
     * @param numberBlocks Number of blocks to include in this data set.
     * @return A DataSetReference. No need to destroy it, it uses shared_ptr.
     */
    DataSetReference getDataSet(unsigned int numberBlocks = 1);
    /**
     * \brief Get a new DataBlockContainer.
     * Payload size is either random or fixed depending on parameters passed to the constructor.
     * @return A DataSetReference. No need to destroy it, it uses shared_ptr.
     */
    DataBlockContainerReference getDataBlockContainer();

  private:
    uint32_t mCurrentPayloadSize;
    bool mIsRandom;
    std::default_random_engine mGenerator;
    uint32_t mCurrentId;
};

}
}

#endif //DATA_SAMPLING_DATABLOCKPRODUCER_H
