#ifndef OVIS_BASE_DATABLOCK_HH_INCLUDED
#define OVIS_BASE_DATABLOCK_HH_INCLUDED

//#include <iostream>
#include "dll.hh"
#include "itypes.hh"
#include "localstreamable.hh"

namespace ovis {
namespace base {
	
	//! A streamable memory block suitable for buffering.
	/*!
	A Datablock can be seen as a file in memory (similar to MemIO). It is very
	useful when buffering or modifying a batch of data.
	*/

	class OVIS_API Datablock:public LocalStreamable
	{
	protected:
		ovis_uint8 *m_pData;
		unsigned long m_Size,m_ActualSize,m_Pos;

		unsigned char m_BitBuffer;
		unsigned char m_BitCount;
	public:
		//! The block's "name".
		/*! A Datablock does not have a name. This method returns a description
		instead, formatted this way: "Datablock <pointer> size <size> actualsize <actual size>".
		This makes it useful for debugging purposes.
		*/
		const std::string& name() const;

		// Bit set/get methods

		//! Adds bits (will be cached in a 8-bit buffer)
		/*! This method is part of a 8-bit LIFO bit cache. The bits added will not
		immediately be present in the buffer. To force this, call flush().
		@param bits The bits to be added. Endianness is irrelevant here. The MSB will
		get inserted first, the LSB last.
		@param size The number of the bits to be added.
		*/
		void addBits(const unsigned long bits,const unsigned short size);
		//! gets a bit. Bits in the cache will be ignored.
		bool getBit();
		//! gets up to 32 bits. Bits in the cache will be ignored.
		unsigned long getBits(const unsigned long size);
		//! flushes the 8-bit-cache.
		void flushBitCache() { while(m_BitCount!=0) addBit(false); }
		//! adds one bit.
		void addBit(const bool bit );

		//! Retrieves the hash value of this string.
		/*!
		A hash of a datablock is often useful for speeding up the comparison of
		datablocks. The hash value will be calculated using lazy evaluation, e.g. if
		the block gets modified, the hash value is marked invalid (if there was a hash
		value before). The next time hash() is called, the hash value automatically
		gets recalculated. The next hash() call does not recalculate anything, but just
		returns the hash value - assuming it's valid, of course.
		*/
		unsigned long hash() const;

		//! Clones the memory block;  this memblock becomes an exact duplicate of rDatablockFrom.
		/*! @param rDatablockFrom The datablock to be cloned.
		\note This method duplicates the specified block, including its size, position etc.
		No information of the old block remains.
		\see copyTo()
		*/
		void clone(const Datablock &rDatablockFrom);
		//! Writes this memory block's contents to rDatablock.
		/*! @param rDatablock The datablock receiving the data from this block.
		\note This method sends a regular write() request to the specified datablock.
		This block's data will be written at the specified block's writing position.
		This method does NOT clone the block.
		\see clone()
		*/
		void copyTo(Datablock &rDatablock) const;

		//! Compares two datablocks.
		/*! First, the hash values are compared. If they do not match, false is
		returned. Otherwise, the blocks are compared byte by byte.
		*/
		const Datablock& operator =(const Datablock& x) { clone(x); return *this; }

		//! Writes some data into the block at the current position.
		/*! Resizes the memory block if necessary.
		Return value: amount of bytes written. Shouldn't deviate from "size".
		@param src The data to be written.
		@param wsize The size of the data to be written, in bytes.
		*/
		unsigned long write(const void *src,const unsigned long wsize);
		//! Reads some data into "dest".
		/*! Return value: amount of bytes read. Can deviate from "size", for example
		if read() is near the end of the datablock and thus reads fewer bytes. Should NOT
		be greater than "size".
		@param dest Pointer to a memory location where the data will be uploaded to.
		@param rsize The amount of data to be read, in bytes.
		*/
		unsigned long read(void *dest,const unsigned long rsize);

		//! Resizes the memory block.
		/*! @param newsize The new, desired size of the block. */
		void resize(const unsigned long newsize);
		//! Expands the memory block. Does the same like resize(size()+addsize)
		/*!
		@param addsize Amount of bytes the block is to be expanded.
		@return Pointer to the previous limit of the buffer.
		For example, if the buffer was 32 bytes large, and gets expanded, then this
		points to byte nr. 32 in the buffer.
		*/
		ovis_uint8* expand(const unsigned long addsize);
		//! Contracts the memory block. Does the same like resize(size()-addsize)
		/*! @param subsize Amount of bytes the block is to be contracted. */
		void contract(const unsigned long subsize) { resize(m_Size-subsize); }

		//! Creates a datablock with 0 byte.
		Datablock();
		//! Copy constructor. Calls clone().
		/*! @param DatablockFrom The datablock to be copied. */
		Datablock(const Datablock& rDatablockFrom);
		
		//! Destroys the datablock.
		virtual ~Datablock();

		//! Retrieves a pointer to the data.
		inline const ovis_uint8 *data() const { return m_pData; }
		//! Retrieves a pointer to the data.
		inline ovis_uint8 *data() { return m_pData; }
		//! Operator [] for byte retrieval.
		inline ovis_uint8 operator[](const unsigned long pos) const { return m_pData[pos]; }
		//! Operator [] for byte retrieval.
		inline ovis_uint8& operator[](const unsigned long pos) { return m_pData[pos]; }
		//! Size of the Datablock, in bytes.
		inline unsigned long size() const { return m_Size; }
		//! Current I/O position in the buffer, in bytes.
		inline unsigned long tell() const { return m_Pos; }

		void seek(const long Pos,const Seekmode seekmode);

		//! Tells whether the current I/O position is at the end of the buffer.
		inline bool eof() const { return (m_Pos>=m_Size); }		

		// Stream operators
		
		friend inline const Datablock& operator >> (const Datablock& src,Datablock& dest)	{ src.copyTo(dest); return src; }
		friend inline Datablock& operator << (Datablock& dest,const Datablock& src)	{ src.copyTo(dest); return dest; }
	};

/*	OVIS_API std::ostream& operator << (std::ostream& stream,const Datablock& datablock);
	OVIS_API std::istream& operator >> (std::istream& stream,Datablock& datablock);
*/
}
}

#endif
