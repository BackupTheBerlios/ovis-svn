#include "../base/itypes.hh"
#include "indexiterator.hh"
#include "indexstream.hh"

namespace ovis {
namespace video {

	Indexstream::Indexstream(const Indexformat format,const ovis_uint32 _capacity):
	Geometrystream(_capacity),m_Format(format)
	{
	}

	Indexstream::~Indexstream()
	{
	}

	void Indexstream::transferFrom(const Indexstream& from,const ovis_uint32 numIndices)
	{
		Indexiterator ifrom((Indexstream&)from),ito(*this);

		for (ovis_uint32 idx=0;idx<numIndices;++idx) {
			ito.index(ifrom.index());
			++ifrom; ++ito;
		}
	}

}
}
