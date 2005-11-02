#include "pkdatablock.hh"

namespace ovis {
namespace base {

	void PackedDatablock::pack(const Datablock &rDatablockSrc)
	{
		if (m_pPackHandler) m_pPackHandler->pack(rDatablockSrc,*this);
		else clone(rDatablockSrc);
	}

	void PackedDatablock::unpack(Datablock &rDatablockDest)
	{
		if (m_pPackHandler) m_pPackHandler->unpack(*this,rDatablockDest);
		else rDatablockDest.clone(*this);
	}

}
}
