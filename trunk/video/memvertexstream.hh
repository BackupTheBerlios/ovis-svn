#ifndef OVIS_VIDEO_MEMVERTEXSTREAM_HH_INCLUDED
#define OVIS_VIDEO_MEMVERTEXSTREAM_HH_INCLUDED

#include "../base/datablock.hh"
#include "../base/dll.hh"
#include "../base/itypes.hh"
#include "../math/vectors.hh"
#include "vertexstream.hh"

namespace ovis {
namespace video {

	class OVIS_API MemVertexstream:public Vertexstream
	{
	public:
		inline void bind() {}
		inline void map(const ovis_uint32 flags) {}
		inline void unmap() {}

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK() {}
	
		inline void* mappedPointer() { return (void*)(m_Vertices.data()); }
		inline const void* mappedPointer() const { return (void*)(m_Vertices.data()); }
		inline bool isMapped() const { return true; }

		MemVertexstream(const Vertexformat& format,const ovis_uint32 numVertices,Vertexsource *pSource=0);

		void calcMinmax(math::Vector3f& minxyz,math::Vector3f& maxxyz);

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

		void resize(const ovis_uint32 numIndices);
	protected:
		void calculatePointers(VertexiteratorPointers& vitp);

		base::Datablock m_Vertices;
		VertexiteratorPointers m_Pointers;
	};

}
}

#endif
