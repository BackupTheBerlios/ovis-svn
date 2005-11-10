#ifndef OVIS_BASE_PACKED_DATABLOCK_HH_INCLUDED
#define OVIS_BASE_PACKED_DATABLOCK_HH_INCLUDED

#include "dll.hh"
#include "datablock.hh"

namespace ovis {
namespace base {

	class PackedDatablock;

	// Handles data packing/unpacking
	/**
	* This class contains functionality for packing/unpacking data from/to PackedDatablock objects.
	* Derived classes exist for null packing (i.e. simple copies), zlib- and bzip2-packing.
	* For more information about how to use packing functionality, see the PackedDatablock description.
	*/
	class OVIS_API PackHandler
	{
	protected:
		PackHandler() {}
	public:
		virtual ~PackHandler() {}

		//! Packs data from rDatablockSrc into rPackedBlockDest.
		/**
		* @param rDatablockSrc Source data to be packed
		* @param rPackedBlockDest Destination where the packed data shall be stored
		*/
		virtual void pack(const Datablock &rDatablockSrc,PackedDatablock &rPackedBlockDest)=0;
		//! Unpacks data from rPackedBlockSrc into rDatablockDest.
		/**
		* @param rPackedBlockSrc Packed source data to be unpacked
		* @param rDatablockDest Destination where the unpacked data shall be stored
		*/
		virtual void unpack(const PackedDatablock &rPackedBlockSrc,Datablock &rDatablockDest)=0;
	};

	//! Container for packed data.
	/**
	* The PackedDatablock and PackHandler classes form the packing system. The PackHandler
	* takes a datablock, compresses/encodes it, and stores the result into a PackedDatablock.
	* Unpacking works exactly the other way round: PackedDatablock source, packhandler unpacks
	* into a datablock. Note that the source for packing and the destination for unpacking
	* can be any datablock, and do NOT have to be a PackedDatablock instance. In other words:
	* Packed data has to be in a PackedDatablock instance, but unpacked data can be in any
	* Datablock or Datablock-derived class instance.
	*
	* Example for usage:
	*
	* @code
	* base::ZPackHandler zpkhandler
	* base::Datablock sourcedata;
	* base::PackedDatablock dest;
	*
	* readSomeData(sourcedata);
	* zpkhandler.pack(sourcedata,dest);
	* @endcode
	*
	* The last line can also be replaced with
	*
	* @code
	* dest.packhandler(&zpkhandler);
	* dest.pack(sourcedata);
	* @endcode
	*
	* PackedDatablocks can have a default pack handler, which is used when PackedDatablock::pack()
	* and PackedDatablock::unpack() are called (do not confuse them with the PackHandler equivalents).
	* Internally, a pointer to a packhandler is stored. If this pointer is 0, the pack/unpack methods
	* simply call clone().
	*/
	class OVIS_API PackedDatablock:public Datablock
	{
	protected:
		PackHandler *m_pPackHandler;

	public:
		//! Constructor that optionally sets a default packhandler.
		/**
		* @param pPackHandler Pointer to a default packhandler. A nullpointer means no default packhandler used.
		*/
		PackedDatablock(PackHandler *pPackHandler=0):Datablock(),m_pPackHandler(pPackHandler) {}
		//! Copy constructor.
		PackedDatablock(PackedDatablock &rFrom):Datablock(rFrom),m_pPackHandler(rFrom.getPackHandler()) {}

		//! Retrieves the default packhandler.
		/**
		* @return Pointer to the default packhandler, or 0 if no default packhandler is set.
		*/
		inline PackHandler* packhandler() { return m_pPackHandler; }
		//! Sets the default packhandler.
		/**
		* @param pPackHandler Pointer to a default packhandler, or 0 if no default packhandler shall be set.
		*/
		inline void packhandler(PackHandler *pPackHandler) { m_pPackHandler=pPackHandler; }
		
		//! Packs data using the default handler.
		/**
		* If no default packhandler is set, this calls clone(rDatablockSrc).
		* Else, it equals to packhandler()->pack(rDatablockSrc,*this);
		* @param rDatablockSrc Source datablock to be packed
		*/
		void pack(const Datablock &rDatablockSrc);

		//! Unpacks data using the default handler.
		/**
		* If no default packhandler is set, this calls rDatablockDest.clone(*this).
		* Else, it equals to packhandler()->unpack(*this,rDatablockSrc);
		* @param rDatablockDest Datablock where the unpacked data shall be stored
		*/
		void unpack(Datablock &rDatablockDest);
	};

	//! Dummy pkhandler that only copies without packing.
	class NullPKHandler:public PackHandler {
	public:
		NullPKHandler() {}
		
		void pack(const Datablock &rDatablockSrc,PackedDatablock &rPackedBlockDest) { rPackedBlockDest.clone(rDatablockSrc); }
		void unpack(const PackedDatablock &rPackedBlockSrc,Datablock &rDatablockDest) { rDatablockDest.clone(rPackedBlockSrc); }
	};

}
}

#endif
