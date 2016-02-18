struct DataHeader_t {

	/// 4 bytes of a magic string
	int32_t mMagicString;

	/// size of the struct
	int32_t mStructSize;

	/// header version, bookkeeping in the software */
	int32_t mHeaderVersion

	/// flags, one of the bits to indicate that a sub header is following */
	int32_t mFlags;

	/// size of payload in memory */
	int64_t mPayloadSize;

	/// payload serialization method for transfer */
	char mPayloadSerializationMethod[7+1];

	/// data payload meta data: type, origin, specification */
	//PayloadMetaData_t mMetaData;
};