/// \file DataBlock.h
/// \brief C interface for a general data format.
///
/// Data are organized in a succession of continuous HEADER + PAYLOAD in memory.
/// Each PAYLOAD is also a succession of headers and payloads, up to a certain point (i.e. when payload is not a concatenation of things).
///
/// A base header is defined, and common to all data block types.
/// Each data block type may have a specialized header based on DataBlockHeaderBase.
/// It should be a concatenation of DataBlockHeaderBase and of additionnal fields.
///
/// \author Sylvain Chapeland, CERN

#ifndef DATAFORMAT_DATABLOCK
#define DATAFORMAT_DATABLOCK


#include <stdint.h>


/// Definition of data block types and their associated header.
typedef enum {
  H_BASE = 0xBB,               ///< base header type
} DataBlockType;



/// Data structure is based on standard data types with specified width.
/// headerPtr + headerSize = payloadPtr
/// headerPtr + headerSize + dataSize = nextHeaderPtr (if not toplevel block header)
typedef struct {
  uint32_t   blockType;     ///< ID to identify structure type
  uint32_t   headerSize;     ///< header size in bytes
  uint32_t   dataSize;     ///< data size following this structure (until next header, if this is not a toplevel block header)
} DataBlockHeaderBase;


/// Add extra types below, e.g.
///
/// typedef struct {
///   DataBlockHeaderBase header;   ///< Base common data header
///   int numberOfSubTimeframes;    ///< number of subtimeframes in payload
/// } DataBlockHeaderTimeframe;

typedef struct
{
    DataBlockHeaderBase header;   ///< Base common data header
    char* data;
} DataBlock;

#endif /* DATAFORMAT_DATABLOCK */
