#ifndef OVIS_VIDEO_GEOMETRYSTREAM_HH_INCLUDED
#define OVIS_VIDEO_GEOMETRYSTREAM_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../base/itypes.hh"
#include "mempool.hh"
#include <vector>

namespace ovis {
namespace video {

	class OVIS_API Geometrystream:public base::BaseMemObj
	{
	public:
		virtual void bind()=0;
		virtual void map(const ovis_uint32 flags)=0;
		virtual void unmap()=0;
		
		virtual void* mappedPointer()=0;
		virtual bool isMapped() const=0;

		virtual bool isDataOK() const=0;
		virtual void dataIsOK()=0;

		inline ovis_uint32 capacity() const { return m_Capacity; }

		virtual ~Geometrystream() {}
	protected:
		Geometrystream(const ovis_uint32 capacity):m_Capacity(capacity) {}

		ovis_uint32 m_Capacity;
	};

}
}

#endif
