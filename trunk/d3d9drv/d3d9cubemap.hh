#ifndef OVIS_D3D9DRV_D3D9CUBEMAP_HH_INCLUDED
#define OVIS_D3D9DRV_D3D9CUBEMAP_HH_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION 0x0900
#endif

#include <windows.h>
#include <d3d9.h>

#include "../video/mempool.hh"
#include "../video/pixelformat.hh"
#include "../video/cubemap.hh"

namespace ovis {
namespace d3d9drv {

	class D3D9Device;

	class OVIS_API D3D9Cubemap:public video::Cubemap
	{
	public:
		D3D9Cubemap(D3D9Device& d3d9device,const base::String& identifier,
			const ovis_uint32 edgelength,const ovis_uint32 levels,
			const ovis_uint32 flags,const video::Pixelformat format,const video::Memorypool pool);
		~D3D9Cubemap();

		video::Pixelformat actualpixelformat() const;
		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();
		ovis_uint32 edgelength() const;
		ovis_uint32 numMipmaplevels() const;
		void transferData(const ovis_uint32 level,const base::Datablock& datablock,const ovis_uint32 pitch,
			const video::RGBALayouts rgbalayout,const video::Channeldatatype channeldatatype);
		LPDIRECT3DCUBETEXTURE9 d3d9cubetexture() const;
		void d3d9releaseBeforeReset();
		void d3d9restoreAfterReset();

	protected:
		D3D9Device& m_rD3D9Device;
		LPDIRECT3DCUBETEXTURE9 m_pCubemap;
		bool m_IsValid,m_IsDataOK;

		video::Pixelformat m_ActualPixelformat;
		ovis_uint32 m_Edgelength,m_NumMipmaplevels;
	};

}
}

#endif
