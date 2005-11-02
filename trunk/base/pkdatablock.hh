#ifndef OVIS_BASE_PACKED_DATABLOCK_HH_INCLUDED
#define OVIS_BASE_PACKED_DATABLOCK_HH_INCLUDED

#include "dll.hh"
#include "datablock.hh"

namespace ovis {
namespace base {

	class PackedDatablock;
		
	class OVIS_API PackHandler
	{
	protected:
		PackHandler() {}
	public:
		virtual ~PackHandler() {}

		virtual void pack(const Datablock &rDatablockSrc,PackedDatablock &rPackedBlockDest)=0;
		virtual void unpack(const PackedDatablock &rPackedBlockSrc,Datablock &rDatablockDest)=0;
	};

	class OVIS_API PackedDatablock:public Datablock
	{
	protected:
		PackHandler *m_pPackHandler;

	public:
		PackedDatablock(PackHandler *pPackHandler=0):Datablock(),m_pPackHandler(pPackHandler) {}
		PackedDatablock(PackedDatablock &rFrom):Datablock(rFrom),m_pPackHandler(rFrom.getPackHandler()) {}
			
		inline PackHandler* getPackHandler() { return m_pPackHandler; }
		inline void setPackHandler(PackHandler *pPackHandler) { m_pPackHandler=pPackHandler; }
		
		void pack(const Datablock &rDatablockSrc);
		void unpack(Datablock &rDatablockDest);
	};

	// Dummy pkhandler that only copies without packing.
	class NullPKHandler {
	public:
		NullPKHandler() {}
		
		void pack(const Datablock &rDatablockSrc,PackedDatablock &rPackedBlockDest) { rPackedBlockDest.clone(rDatablockSrc); }
		void unpack(const PackedDatablock &rPackedBlockSrc,Datablock &rDatablockDest) { rDatablockDest.clone(rPackedBlockSrc); }
	};

}
}

#endif
