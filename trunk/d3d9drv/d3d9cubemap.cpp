#include <d3dx9.h>

#include "../base/log.hh"
#include "d3d9device.hh"
#include "d3d9cubemap.hh"
#include "tod3d9convs.hh"

namespace ovis {
namespace d3d9drv {

	D3D9Cubemap::D3D9Cubemap(D3D9Device& d3d9device,const base::String&identifier,
		const ovis_uint32 edgelength,const ovis_uint32 levels,
		const ovis_uint32 flags,const video::Pixelformat format,const video::Memorypool pool):
		Cubemap(format,identifier),m_rD3D9Device(d3d9device),m_pCubemap(0),m_IsValid(true),m_IsDataOK(true)
	{
		UINT actualedgelength=edgelength,actualnumlevels=levels;
		D3DFORMAT fmt=d3dpixelformat(format);
		D3DPOOL mempool=d3dmempool(pool);

		if (fmt==D3DFMT_UNKNOWN) {
			m_IsValid=false;
			base::log("D3D9Cubemap()",base::Error) << "Unknown/unsupported pixel format\n";

			return;
		}

		DWORD usage=0;

		if (video::isDepthformat(format)) usage|=D3DUSAGE_DEPTHSTENCIL;
		if (flags&video::Textureflag_IsDynamic) usage|=D3DUSAGE_DYNAMIC;
		if (flags&video::Textureflag_IsRendertaget) usage|=D3DUSAGE_RENDERTARGET;

		HRESULT hr=D3DXCheckCubeTextureRequirements(
			d3d9device.d3d9deviceinterface(),&actualedgelength,&actualnumlevels,
			(usage&D3DUSAGE_RENDERTARGET), // masks out everything else - this function only accepts this one flag
			&fmt,mempool);

		if (FAILED(hr)) {
			m_IsValid=false;
			base::log("D3D9Cubemap()",base::Error) << "D3DXCheckCubeTextureRequirements() failed\n";

			return;
		}

		hr=d3d9device.d3d9deviceinterface()->CreateCubeTexture(
			actualedgelength,actualnumlevels,usage,fmt,mempool,&m_pCubemap,0);

		if (FAILED(hr)) {
			m_IsValid=false;
			base::log("D3D9Cubemap()",base::Error) << "CreateCubeTexture() failed\n";
		}

		m_Edgelength=actualedgelength;
		m_ActualPixelformat=fromd3dpixelformat(fmt);
		m_NumMipmaplevels=actualnumlevels;

		if (m_ActualPixelformat==video::Pixelformat_Unknown) {
			base::log("D3D9Cubemap()",base::Warning) << "Actual pixelformat unknown\n";
		}
	}

	D3D9Cubemap::~D3D9Cubemap()
	{
		if (m_pCubemap) m_pCubemap->Release();
	}

	video::Pixelformat D3D9Cubemap::actualpixelformat() const
	{
		return m_ActualPixelformat;
	}

	bool D3D9Cubemap::isValid() const
	{
		return m_IsValid;
	}

	bool D3D9Cubemap::isDataOK() const
	{
		return m_IsDataOK;
	}

	void D3D9Cubemap::dataIsOK() { m_IsDataOK=true; }

	LPDIRECT3DCUBETEXTURE9 D3D9Cubemap::d3d9cubetexture() const
	{
		return m_pCubemap;
	}

	ovis_uint32 D3D9Cubemap::edgelength() const
	{
		return m_Edgelength;
	}

	ovis_uint32 D3D9Cubemap::numMipmaplevels() const
	{
		return m_NumMipmaplevels;
	}

	void D3D9Cubemap::transferData(const ovis_uint32 level,const base::Datablock& datablock,const ovis_uint32 pitch,
		const video::RGBALayouts rgbalayout,const video::Channeldatatype channeldatatype)
	{		
		D3DCUBEMAP_FACES face;
		switch (m_CurrentFace) {
			case FrontFace:face=D3DCUBEMAP_FACE_POSITIVE_Z; break;
			case BackFace:face=D3DCUBEMAP_FACE_POSITIVE_Z; break;
			case LeftFace:face=D3DCUBEMAP_FACE_POSITIVE_X; break;
			case RightFace:face=D3DCUBEMAP_FACE_POSITIVE_X; break;
			case TopFace:face=D3DCUBEMAP_FACE_POSITIVE_Y; break;
			case BottomFace:face=D3DCUBEMAP_FACE_POSITIVE_Y; break;
		}
		D3DLOCKED_RECT lockedrect;
		m_pCubemap->LockRect(face,level,&lockedrect,0,0);
		BYTE *pDest=(BYTE *)(lockedrect.pBits);
		const BYTE *pSrc=datablock.data();
		UINT y=0;
		UINT bpl=(pitch<(UINT)(lockedrect.Pitch)) ? pitch : (UINT)(lockedrect.Pitch);
		for (;y<m_Edgelength;pSrc+=pitch,pDest+=lockedrect.Pitch,++y) {
			memcpy(pDest,pSrc,bpl);
		}
		m_pCubemap->UnlockRect(face,level);
	}

	void D3D9Cubemap::d3d9releaseBeforeReset()
	{
		m_IsDataOK=false;
	}

	void D3D9Cubemap::d3d9restoreAfterReset()
	{
		m_IsDataOK=false;
	}



}
}
