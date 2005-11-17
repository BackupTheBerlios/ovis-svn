#include "indexformat.hh"
#include "memindexstream.hh"

namespace ovis {
namespace video {

	MemIndexstream::MemIndexstream(const ovis_uint32 numIndices):Indexstream(Indexformat_32bit,numIndices)
	{
		m_Indices.resize(sizeof(ovis_uint32)*numIndices);
		if (numIndices>0) memset(m_Indices.data(),0,m_Indices.size());
	}

	bool MemIndexstream::isValid() const
	{
		return true;
	}

	bool MemIndexstream::isDataOK() const
	{
		return true;
	}

	void MemIndexstream::resize(const ovis_uint32 numIndices)
	{
		m_Indices.resize(sizeof(ovis_uint32)*numIndices);
	}

}
}
