#include <d3dx9.h>

#include "../base/log.hh"
#include "d3d9device.hh"
#include "d3d9texture2d.hh"
#include "tod3d9convs.hh"

namespace ovis {
namespace d3d9drv {

	D3D9Texture2D::D3D9Texture2D(D3D9Device& d3d9device,const base::String&identifier,
		const ovis_uint32 width,const ovis_uint32 height,
		const ovis_uint32 levels,const ovis_uint32 flags,const video::Pixelformat format,const video::Memorypool pool):
		video::Texture2D(format,identifier),m_rD3D9Device(d3d9device),m_pTex(0),m_IsValid(true),m_IsDataOK(true)
	{
		UINT actualwidth=width,actualheight=height,actualnumlevels=levels;
		D3DFORMAT fmt=d3dpixelformat(format);
		D3DPOOL mempool=d3dmempool(pool);

		if (fmt==D3DFMT_UNKNOWN) {
			m_IsValid=false;
			base::log("D3D9Texture2D()",base::Error) << "Unknown/unsupported pixel format\n";

			return;
		}

		DWORD usage=0;

		if (video::isDepthformat(format)) usage|=D3DUSAGE_DEPTHSTENCIL;
		if (flags&video::Textureflag_IsDisplacementMap) usage|=D3DUSAGE_DMAP;
		if (flags&video::Textureflag_IsDynamic) usage|=D3DUSAGE_DYNAMIC;
		if (flags&video::Textureflag_IsRendertaget) usage|=D3DUSAGE_RENDERTARGET;

		HRESULT hr=D3DXCheckTextureRequirements(
			d3d9device.d3d9deviceinterface(),&actualwidth,&actualheight,&actualnumlevels,
			(usage&D3DUSAGE_RENDERTARGET), // masks out everything else - this function only accepts this one flag
			&fmt,mempool);

		if (FAILED(hr)) {
			m_IsValid=false;
			base::log("D3D9Texture2D()",base::Error) << "D3DXCheckTextureRequirements() failed\n";

			return;
		}
			
		hr=d3d9device.d3d9deviceinterface()->CreateTexture(
			actualwidth,actualheight,actualnumlevels,usage,fmt,mempool,&m_pTex,0);

		if (FAILED(hr)) {
			m_IsValid=false;
			base::log("D3D9Texture2D()",base::Error) << "CreateTexture() failed\n";
		}

		m_Width=actualwidth;
		m_Height=actualheight;
		m_ActualPixelformat=fromd3dpixelformat(fmt);
		m_NumMipmaplevels=actualnumlevels;

		if (m_ActualPixelformat==video::Pixelformat_Unknown) {
			base::log("D3D9Texture2D()",base::Warning) << "Actual pixelformat unknown\n";
		}
	}

	D3D9Texture2D::~D3D9Texture2D()
	{
		if (m_pTex) m_pTex->Release();
	}

	video::Pixelformat D3D9Texture2D::actualpixelformat() const
	{
		return m_ActualPixelformat;
	}

	void D3D9Texture2D::transferData(const ovis_uint32 level,const base::Datablock& datablock,const ovis_uint32 pitch,
		const ovis_uint32 height,const video::RGBALayouts rgbalayout,const video::Channeldatatype channeldatatype)
	{
		D3DLOCKED_RECT lockedrect;
		m_pTex->LockRect(level,&lockedrect,0,0);
		BYTE *pDest=(BYTE *)lockedrect.pBits;
		const BYTE *pSrc=datablock.data();
		UINT y=0;
		UINT bpl=(pitch<(UINT)(lockedrect.Pitch)) ? pitch : (UINT)(lockedrect.Pitch);
		for (;y<height;pSrc+=pitch,pDest+=lockedrect.Pitch,++y) {
			memcpy(pDest,pSrc,bpl);
		}
		m_pTex->UnlockRect(level);
	}

	ovis_uint32 D3D9Texture2D::width() const
	{
		return m_Width;
	}

	ovis_uint32 D3D9Texture2D::height() const
	{
		return m_Height;
	}

	ovis_uint32 D3D9Texture2D::numMipmaplevels() const
	{
		return m_NumMipmaplevels;
	}

	void D3D9Texture2D::copyFromFramebuffer(const ovis_uint32 x,const ovis_uint32 y)
	{
	}

	bool D3D9Texture2D::isValid() const
	{
		return m_IsValid;
	}

	bool D3D9Texture2D::isDataOK() const
	{
		return m_IsDataOK;
	}

	void D3D9Texture2D::dataIsOK() { m_IsDataOK=true; }

	LPDIRECT3DTEXTURE9 D3D9Texture2D::d3d9texture() const
	{
		return m_pTex;
	}

	void D3D9Texture2D::d3d9releaseBeforeReset()
	{
		m_IsDataOK=false;
	}

	void D3D9Texture2D::d3d9restoreAfterReset()
	{
		m_IsDataOK=false;
	}

}
}
