#ifndef OVIS_BASE_BZ2PKHANDLER_HH_INCLUDED
#define OVIS_BASE_BZ2PKHANDLER_HH_INCLUDED

#include "dll.hh"
#include "pkdatablock.hh"

namespace ovis {
namespace base {
	
	//! PackHandler for bzip2-compressed data.

	class OVIS_API BZ2PackHandler:public PackHandler
	{
	public:
		BZ2PackHandler():m_Blocksize100k(9) {}

		void pack(const Datablock &rDatablockSrc,PackedDatablock &rPackedBlockDest);
		void unpack(const PackedDatablock &rPackedBlockSrc,Datablock &rDatablockDest);

		//! Sets the bzip2 block vssize.
		/**
		* One of bzip2's crucial components is the Burrows-Wheeler Transform, in short BWT.
		* For this transform, a buffer is needed. Roughly speaking, the bigger the buffer, the better
		* the compression (not always, but mostly), however compression takes more time.
		* Minimum setting is 1 (=100 KByte buffer), maximum is 9 (=900 KByte buffer). Default is 9.
		* This value should be set before packing. During unpacking, this value is ignored.
		* @param size New size for the buffer. Minimum setting is 1 (=100 KByte buffer), maximum is 9 (=900 KByte buffer).
		*/
		inline void blocksize(int size) { m_Blocksize100k=(size<=1) ? 1 : ((size>9) ? 9 : size); }
		//! Returns the block buffer size.
		/**
		* See blocksize(int size) for details.
		* @return The buffer size.
		*/
		inline int blocksize() const { return m_Blocksize100k; }
	protected:
		int m_Blocksize100k;
	};

}
}

#endif
