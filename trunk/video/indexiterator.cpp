#include <assert.h>
#include "../base/itypes.hh"
#include "indexiterator.hh"
#include "indexstream.hh"

namespace ovis {
namespace video {

	OVIS_API unsigned long indexformatSizeLookup(const Indexformat format)
	{
		switch (format) {
			case Indexformat_16bit:return sizeof(ovis_uint16); break;
			case Indexformat_32bit:return sizeof(ovis_uint32); break;
		};

		return 0;
	}

	Indexiterator::Indexiterator(Indexstream& rIndexstream,const ovis_uint32 position):
	m_CurrentindexValid(false),m_CurrentPosition(position),m_rIndexstream(rIndexstream)
	{
	}

	Indexiterator::Indexiterator(Indexstream& rIndexstream):
	m_CurrentindexValid(false),m_CurrentPosition(0),m_rIndexstream(rIndexstream)
	{
	}

	Indexiterator::~Indexiterator()
	{
	}

	void Indexiterator::positionInStream(const unsigned int index)
	{
		m_CurrentPosition=index;
		if (index>=m_rIndexstream.capacity()) m_CurrentPosition=m_rIndexstream.capacity();
		m_CurrentindexValid=false;
	}

	void Indexiterator::index(const ovis_uint32 newindex)
	{
		if (newindex>m_rIndexstream.capacity()) return;
		m_CurrentIndex=newindex;
		m_rIndexstream.index(m_CurrentPosition,newindex);
		m_CurrentindexValid=true;
	}

	ovis_uint32 Indexiterator::index() const
	{
		if (!m_CurrentindexValid && (m_CurrentPosition<m_rIndexstream.capacity())) {
			m_CurrentIndex=m_rIndexstream.index(m_CurrentPosition);
			m_CurrentindexValid=true;
		}
		return m_CurrentIndex;
	}


	Indexiterator& Indexiterator::operator ++()
	{
		++m_CurrentPosition;
		if (m_CurrentPosition>=m_rIndexstream.capacity()) m_CurrentPosition=m_rIndexstream.capacity();
		m_CurrentindexValid=false;

		return *this;
	}

	Indexiterator& Indexiterator::operator ++(const int)
	{
		++m_CurrentPosition;
		if (m_CurrentPosition>=m_rIndexstream.capacity()) m_CurrentPosition=m_rIndexstream.capacity();
		m_CurrentindexValid=false;

		return *this;
	}

	Indexiterator& Indexiterator::operator --()
	{
		--m_CurrentPosition;
		if (m_CurrentPosition>=m_rIndexstream.capacity()) m_CurrentPosition=m_rIndexstream.capacity();
		m_CurrentindexValid=false;

		return *this;
	}

	Indexiterator& Indexiterator::operator --(const int)
	{
		--m_CurrentPosition;
		if (m_CurrentPosition>=m_rIndexstream.capacity()) m_CurrentPosition=m_rIndexstream.capacity();
		m_CurrentindexValid=false;

		return *this;
	}

	Indexiterator& Indexiterator::operator +=(const int offset)
	{
		m_CurrentPosition=(ovis_uint32)(((int)m_CurrentPosition)+offset);
		if (m_CurrentPosition>=m_rIndexstream.capacity()) m_CurrentPosition=m_rIndexstream.capacity();
		m_CurrentindexValid=false;

		return *this;
	}

	Indexiterator& Indexiterator::operator -=(const int offset)
	{
		m_CurrentPosition=(ovis_uint32)(((int)m_CurrentPosition)-offset);
		if (m_CurrentPosition>=m_rIndexstream.capacity()) m_CurrentPosition=m_rIndexstream.capacity();
		m_CurrentindexValid=false;

		return *this;
	}

}
}
