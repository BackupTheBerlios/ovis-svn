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

		inline void blocksize(int size) { m_Blocksize100k=(size<=1) ? 1 : ((size>9) ? 9 : size); }
		inline int blocksize() const { return m_Blocksize100k; }
	protected:
		int m_Blocksize100k;
	};

}
}

#endif
