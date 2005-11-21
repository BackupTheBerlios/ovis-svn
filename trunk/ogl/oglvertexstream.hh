#ifndef OVIS_OPENGLDRV_OGLVERTEXSTREAM_HH_INCLUDED
#define OVIS_OPENGLDRV_OGLVERTEXSTREAM_HH_INCLUDED

#include "../base/dll.hh"
#include "../video/vertexformat.hh"
#include "../video/vertexiterator.hh"
#include "../video/vertexstream.hh"

namespace ovis {
namespace opengldrv {

	class BufferObject;

	class OVIS_API OGLVertexstream:public video::Vertexstream
	{
	public:
		OGLVertexstream(const ovis_uint32 numVertices,
			const video::Vertexformat& format,const ovis_uint32 flags,video::Vertexstream::Vertexsource *pSource=0);
		~OGLVertexstream();

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void bind();
		void map(const ovis_uint32 flags);
		void unmap();

		void* mappedPointer();
		bool isMapped() const;

		const math::Vector3f& position(const ovis_uint32 vtxindex) const;
		const math::Vector3f& normal(const ovis_uint32 vtxindex) const;
		ovis_uint32 diffuseColor(const ovis_uint32 vtxindex) const;
		ovis_uint32 specularColor(const ovis_uint32 vtxindex) const;
		float texcoord1D(const ovis_uint32 vtxindex,const ovis_uint32 texstage) const;
		const math::Vector2f& texcoord2D(const ovis_uint32 vtxindex,const ovis_uint32 texstage) const;
		const math::Vector3f& texcoord3D(const ovis_uint32 vtxindex,const ovis_uint32 texstage) const;

		void position(const ovis_uint32 vtxindex,const float x,const float y,const float z);
		void position(const ovis_uint32 vtxindex,const math::Vector3f& newPosition);
		void normal(const ovis_uint32 vtxindex,const float x,const float y,const float z);
		void normal(const ovis_uint32 vtxindex,const math::Vector3f& newNormal);
		void diffuseColor(const ovis_uint32 vtxindex,const ovis_uint8 a,const ovis_uint8 r,const ovis_uint8 g,const ovis_uint8 b);
		void diffuseColor(const ovis_uint32 vtxindex,const ovis_uint32 color);
		void specularColor(const ovis_uint32 vtxindex,const ovis_uint8 a,const ovis_uint8 r,const ovis_uint8 g,const ovis_uint8 b);
		void specularColor(const ovis_uint32 vtxindex,const ovis_uint32 color);
		void texcoord1D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const float newTexcoord1D);
		void texcoord2D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const float u,const float v);
		void texcoord2D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const math::Vector2f& newTexcoord2D);
		void texcoord3D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const float u,const float v,const float w);
		void texcoord3D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const math::Vector3f& newTexcoord3D);

	protected:
		void calculatePointers(video::VertexiteratorPointers& vitp);

		BufferObject *m_pBuffer;
		bool m_IsDataOK;

		video::VertexiteratorPointers m_Pointers;
	};

}
}

#endif
