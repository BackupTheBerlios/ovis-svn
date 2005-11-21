#ifndef OVIS_OPENGLDRV_OGLINDEXSTREAM_HH_INCLUDED
#define OVIS_OPENGLDRV_OGLINDEXSTREAM_HH_INCLUDED

#include "../base/dll.hh"
#include "../video/indexiterator.hh"
#include "../video/indexstream.hh"

namespace ovis {
namespace opengldrv {

	class BufferObject;

	class OVIS_API OGLIndexstream:public video::Indexstream
	{
	public:
		OGLIndexstream(const ovis_uint32 numIndices,const video::Indexformat format,const ovis_uint32 flags);
		~OGLIndexstream();

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void bind();
		void map(const ovis_uint32 flags);
		void unmap();

		void* mappedPointer();
		bool isMapped() const;

		ovis_uint32 index(const ovis_uint32 position);
		void index(const ovis_uint32 position,const ovis_uint32 newindex);
	protected:

		BufferObject *m_pBuffer;
		bool m_IsDataOK;
	};

}
}


#endif
