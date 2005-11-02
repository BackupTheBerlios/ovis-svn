#ifndef OVIS_VIDEO_MEMINDEXSTREAM_HH_INCLUDED
#define OVIS_VIDEO_MEMINDEXSTREAM_HH_INCLUDED

#include "../base/datablock.hh"
#include "../base/dll.hh"
#include "../base/itypes.hh"
#include "indexstream.hh"

namespace ovis {
namespace video {

	class OVIS_API MemIndexstream:public Indexstream
	{
	public:
		MemIndexstream(const ovis_uint32 size);

		inline void bind() {}
		inline void map(const ovis_uint32 flags) {}
		inline void unmap() {}

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK() {}

		inline void* mappedPointer() { return m_Indices.data(); }
		inline const void* mappedPointer() const { return m_Indices.data(); }
		inline bool isMapped() const { return true; }

		inline ovis_uint32 index(const ovis_uint32 position) { return ((ovis_uint32*)m_Indices.data())[position]; }
		inline void index(const ovis_uint32 position,const ovis_uint32 newindex) { ((ovis_uint32*)m_Indices.data())[position]=newindex; }

	protected:
		base::Datablock m_Indices;
	};

}
}

#endif
