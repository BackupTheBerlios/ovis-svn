#ifndef OVIS_VIDEO_INDEXSTREAM_HH_INCLUDED
#define OVIS_VIDEO_INDEXSTREAM_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/itypes.hh"
#include "geometrystream.hh"
#include "indexiterator.hh"

namespace ovis {
namespace video {

class OVIS_API Indexstream:public Geometrystream
{
friend class Indexiterator;
public:
	virtual ~Indexstream();
	inline Indexformat indexformat() const { return m_Format; }

	inline void transferFrom(const Indexstream& from) { transferFrom(from,from.capacity()); }
	void transferFrom(const Indexstream& from,const ovis_uint32 numIndices);

	virtual ovis_uint32 index(const ovis_uint32 position)=0;
	virtual void index(const ovis_uint32 position,const ovis_uint32 newindex)=0;
protected:
	Indexstream(const Indexformat format,const ovis_uint32 _capacity);

	Indexformat m_Format;
};

}
}

#endif
