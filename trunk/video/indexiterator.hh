#ifndef OVIS_VIDEO_INDEXITERATOR_HH_INCLUDED
#define OVIS_VIDEO_INDEXITERATOR_HH_INCLUDED

#include "../base/itypes.hh"
#include "../base/dll.hh"
#include "indexformat.hh"

namespace ovis {
namespace video {

class Indexstream;

class OVIS_API Indexiterator {
friend class Indexstream;
public:
	Indexiterator(Indexstream& rIndexstream);
	~Indexiterator();

	inline ovis_uint32 positionInStream() const { return m_CurrentPosition; }
	void positionInStream(const unsigned int index);

	friend bool operator ==(const Indexiterator &a,const Indexiterator &b) { return a.positionInStream()==b.positionInStream(); }
	friend bool operator !=(const Indexiterator &a,const Indexiterator &b) { return a.positionInStream()!=b.positionInStream(); }

	Indexiterator& operator ++();
	Indexiterator& operator ++(const int);
	Indexiterator& operator --();
	Indexiterator& operator --(const int);
	Indexiterator& operator +=(const int offset);
	Indexiterator& operator -=(const int offset);

	inline Indexiterator& operator =(const ovis_uint32 newindex) { index(newindex); return *this; }
	inline operator ovis_uint32() const { return index(); }

	void index(const ovis_uint32 newindex);
	ovis_uint32 index() const;
protected:
	Indexiterator(Indexstream& rIndexstream,const ovis_uint32 position);

	mutable bool m_CurrentindexValid;
	mutable ovis_uint32 m_CurrentIndex;
	ovis_uint32 m_CurrentPosition;
	
	Indexstream &m_rIndexstream;
};

}
}

#endif
