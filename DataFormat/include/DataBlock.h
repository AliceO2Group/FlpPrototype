#include <stdint.h>

/// Definition of data block types and their associated header.
/// Each data block type may have a specialized header based on DataBlockHeaderBase.
/// It should be a concatenation of DataBlockHeaderBase and of additionnal fields.
typedef enum {
  H_BASE = 0xBB,               /// base header type
} DataBlockType;


/// Data are organized in a succession of continuous HEADER + PAYLOAD in memory.
/// Up to a certain point, each PAYLOAD is also a succession of headers and payloads.
/// headerPtr + headerSize = payloadPtr
/// headerPtr + headerSize + dataSize = nextHeaderPtr (if not toplevel block header)
/// Data structure is based on standard data types with specified width.
typedef struct {
  uint8_t   blockType;     // ID to identify structure type
  uint32_t headerSize;     // header size in bytes
  uint32_t   dataSize;     // data size following this structure (until next header, if this is not a toplevel block header)
} DataBlockHeaderBase;


/// Add extra types below, e.g.
///
/// typedef struct {
///   DataBlockHeaderBase header;   // Base common data header
///   int numberOfSubTimeframes;    // number of subtimeframes in payload
/// } DataBlockHeaderTimeframe;   
 
