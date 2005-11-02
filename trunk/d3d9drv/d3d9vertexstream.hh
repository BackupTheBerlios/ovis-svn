#ifndef OVIS_D3D9DRV_D3D9VERTEXSTREAM_HH_INCLUDED
#define OVIS_D3D9DRV_D3D9VERTEXSTREAM_HH_INCLUDED

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define DIRECT3D_VERSION 0x0900
#include <d3d9.h>

#include "../video/vertexiterator.hh"
#include "../video/vertexstream.hh"

namespace ovis {
namespace d3d9drv {

	class D3D9Device;

	class D3D9Vertexstream :public video::Vertexstream
	{
	public:
		D3D9Vertexstream(D3D9Device& d3d9device,const video::Vertexformat& format,
			const ovis_uint32 flags,const video::Memorypool mempool,
			const ovis_uint32 _capacity,video::Vertexstream::Vertexsource *pSource);
		~D3D9Vertexstream();

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void bind();
		void map(const ovis_uint32 flags);
		void unmap();

		inline void* mappedPointer() { return m_pMappedPointer; }
		inline bool isMapped() const { return (m_pMappedPointer!=0); }

		void d3d9releaseBeforeReset();
		void d3d9restoreAfterReset();

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

		D3D9Device& m_rD3D9Device;
		LPDIRECT3DVERTEXBUFFER9 m_pVB;
		LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
		ovis_uint8 *m_pMappedPointer;
		bool m_IsDataOK;

		video::VertexiteratorPointers m_Pointers;

		DWORD m_FVF;
	};

}
}

#endif
