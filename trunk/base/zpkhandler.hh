#ifndef OVIS_BASE_ZPKHANDLER_HH_INCLUDED
#define OVIS_BASE_ZPKHANDLER_HH_INCLUDED

#include "dll.hh"
#include "pkdatablock.hh"

namespace ovis {
namespace base {

	//! PackHandler for zlib-compressed data.

	class OVIS_API ZPackHandler:public PackHandler
	{
	protected:
		bool m_ZlibHeader;
		unsigned long m_CompressedSize,m_UncompressedSize;

		inline void uncompressedSize(unsigned long size) { m_UncompressedSize=size; }
		inline void compressedSize(unsigned long size) { m_CompressedSize=size; }
	public:
		ZPackHandler():m_ZlibHeader(true),m_CompressedSize(0),m_UncompressedSize(0) {}
		
		inline void zlibHeader(bool state) { m_ZlibHeader=state; }
		inline bool zlibHeader() const { return m_ZlibHeader; }
		inline unsigned long compressedSize() const { return m_CompressedSize; }
		inline unsigned long uncompressedSize() const { return m_UncompressedSize; }

		void pack(const Datablock &rDatablockSrc,PackedDatablock &rPackedBlockDest);
		void unpack(const PackedDatablock &rPackedBlockSrc,Datablock &rDatablockDest);
	};

}
}

#endif
