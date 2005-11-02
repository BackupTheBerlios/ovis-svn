#include "indexformat.hh"
#include "memindexstream.hh"

namespace ovis {
namespace video {

	MemIndexstream::MemIndexstream(const ovis_uint32 size):Indexstream(Indexformat_32bit,size)
	{
		m_Indices.resize(sizeof(ovis_uint32)*size);
		memset(m_Indices.data(),0,m_Indices.size());
	}

	bool MemIndexstream::isValid() const
	{
		return true;
	}

	bool MemIndexstream::isDataOK() const
	{
		return true;
	}

}
}
