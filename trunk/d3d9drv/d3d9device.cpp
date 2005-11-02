#include <d3dx9.h>
#include <list>

#include "../base/log.hh"
#include "d3d9device.hh"
#include "d3d9cubemap.hh"
#include "d3d9fragmentprogram.hh"
#include "d3d9indexstream.hh"
#include "d3d9texture2d.hh"
#include "d3d9vertexstream.hh"
#include "d3d9vertexprogram.hh"
#include "tod3d9convs.hh"

namespace ovis {
namespace d3d9drv {

	struct D3D9Device::Resources
	{
		std::list<D3D9Texture2D* > m_2DTextures;
		std::list<D3D9Cubemap* > m_Cubemaps;
		std::list<D3D9Vertexstream* > m_Vertexstreams;
		std::list<D3D9Indexstream* > m_Indexstreams;
	};

	D3D9Device::D3D9Device():m_pD3D9(0),m_pD3DDev9(0),m_pCurrentVertexProgram(0),
		m_pCurrentFragmentProgram(0),m_Cullmode(D3DCULL_CCW),m_CullingEnabled(true),
		m_IsValid(true)
	{
		m_pResources=new Resources;

		memset(&m_D3D9FFMaterial,0,sizeof(m_D3D9FFMaterial));
		memset(&m_D3D9FFLight,0,sizeof(m_D3D9FFLight));

		m_pD3D9=Direct3DCreate9(D3D_SDK_VERSION);

		if (!m_pD3D9) {
			base::log("D3D9Device::D3D9Device()",base::Error) << "could not create the D3D9 interface\n";

			m_IsValid=false;
		}

		m_Texcoordindexmodes=new DWORD[8];
		m_Texcoordgenmodes=new DWORD[8];
		for (int i=0;i<8;++i) {
			m_Texcoordindexmodes[i]=0;
			m_Texcoordgenmodes[i]=0;
		}
	}

	D3D9Device::~D3D9Device()
	{
		if (m_pD3DDev9) m_pD3DDev9->Release();
		if (m_pD3D9) m_pD3D9->Release();

		delete[] m_Texcoordgenmodes;
		delete[] m_Texcoordindexmodes;

		delete m_pResources;
	}

	bool D3D9Device::isValid() const
	{
		return m_IsValid;
	}






	const video::Caps& D3D9Device::caps() const
	{
		return m_Caps;
	}

	ovis_uint32 D3D9Device::numVideoadapters() const
	{
		return m_pD3D9->GetAdapterCount();
	}

	ovis_uint32 D3D9Device::numVideomodes(const ovis_uint32 adapterindex,
		const video::Pixelformat pixelformat) const
	{
		D3DFORMAT fmt=d3dpixelformat(pixelformat);
		return m_pD3D9->GetAdapterModeCount(adapterindex,fmt);
	}

	bool D3D9Device::fetchVideomode(const ovis_uint32 adapter,
		const ovis_uint32 modeindex,video::Videomode& mode,
		const video::Pixelformat pixelformat) const
	{
		D3DFORMAT fmt=d3dpixelformat(pixelformat);
		D3DDISPLAYMODE d3ddisplaymode;
		HRESULT hr=m_pD3D9->EnumAdapterModes(adapter,fmt,modeindex,&d3ddisplaymode);

		if (hr!=D3D_OK) return false;

		mode.m_Width=d3ddisplaymode.Width;
		mode.m_Height=d3ddisplaymode.Height;
		mode.m_Hz=d3ddisplaymode.RefreshRate;
		mode.m_Pixelformat=pixelformat; // TODO: correct this (use the format from d3ddisplaymode)

		return true;
	}

	void D3D9Device::init(base::Window* pWindow,const ovis_uint32 adapter,
		const video::Pixelformat colorbufferformat,
		const video::Pixelformat depthstencilformat,const ovis_uint32 Hz)
	{
		if (!m_IsValid) {
			base::log("D3D9Device::init()",base::Warning) <<
				"Initializing an invalid device\n";
		}

		bool windowed=!(pWindow->fullscreenMode());
		HWND hWnd=(HWND)(pWindow->windowhandle());

		D3DFORMAT adapterfmt;
		if (windowed) {
			 D3DDISPLAYMODE d3ddm;
			 m_pD3D9->GetAdapterDisplayMode(adapter,&d3ddm);
			 adapterfmt=d3ddm.Format;
		} else adapterfmt=d3dpixelformat(colorbufferformat);

		if (adapterfmt==D3DFMT_UNKNOWN) {
			base::log("D3D9Device::init()",base::Error) << "Unknown/unsupported color buffer format\n";
			m_IsValid=false; return;
		}

		RECT r;
		GetClientRect(hWnd,&r);

		m_D3DPP.Windowed=windowed;
		m_D3DPP.BackBufferWidth=r.right-r.left;
		m_D3DPP.BackBufferHeight=r.bottom-r.top;
		m_D3DPP.BackBufferCount=1;
		m_D3DPP.BackBufferFormat=adapterfmt;
		m_D3DPP.AutoDepthStencilFormat=d3dpixelformat(depthstencilformat);
		m_D3DPP.EnableAutoDepthStencil=TRUE;
		m_D3DPP.Flags=0;
		m_D3DPP.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;
		m_D3DPP.FullScreen_RefreshRateInHz=windowed ? 0 : Hz;
		m_D3DPP.hDeviceWindow=hWnd;
		m_D3DPP.MultiSampleQuality=0;
		m_D3DPP.MultiSampleType=D3DMULTISAMPLE_NONE;
		m_D3DPP.SwapEffect=D3DSWAPEFFECT_DISCARD;
		m_D3DPP.Windowed=windowed;

		// Getting caps
		base::log("D3D9Device::init()",base::Message) << "Getting caps\n";

		HRESULT hr=m_pD3D9->GetDeviceCaps(adapter,D3DDEVTYPE_HAL,&m_D3DCaps);
		if (FAILED(hr)) {
			base::log("D3D9Device::init()",base::Error) << "GetDeviceCaps() failed\n";
			m_IsValid=false;
			return;
		}

		// Caps structure is initialized here
		{
			base::log("D3D9Device::init()",base::Message) << "Analyzing caps\n";

			m_Caps.m_NPOTTextures=!((m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_POW2) ||
				(m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_NONPOW2CONDITIONAL));
			m_Caps.m_RectangularTextures=(m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_SQUAREONLY)==0;
			m_Caps.m_Cubemaps=(m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_CUBEMAP)!=0;
			m_Caps.m_NPOTCubemaps=(m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_CUBEMAP_POW2)==0;
			m_Caps.m_ProjectedTextures=(m_D3DCaps.TextureCaps&D3DPTEXTURECAPS_PROJECTED)!=0;
			m_Caps.m_DisplacementMaps=false;
			m_Caps.m_CompressedTextures=false;

			m_Caps.m_MaxTextureWidth=m_D3DCaps.MaxTextureWidth;
			m_Caps.m_MaxTextureHeight=m_D3DCaps.MaxTextureHeight;
			m_Caps.m_MaxTextureVolumeDepth=m_D3DCaps.MaxVolumeExtent;
			m_Caps.m_MaxAnisotropy=m_D3DCaps.MaxAnisotropy;
			m_Caps.m_MaxTextureBlendStages=m_D3DCaps.MaxTextureBlendStages;
			m_Caps.m_MaxActiveFFLights=m_D3DCaps.MaxActiveLights;
			m_Caps.m_MaxPointSize=m_D3DCaps.MaxPointSize;
			m_Caps.m_MaxPrimitiveCount=m_D3DCaps.MaxPrimitiveCount;
			m_Caps.m_MaxVertexIndex=m_D3DCaps.MaxVertexIndex;

		}

		// Probing formats
		{

			video::Caps::Texflags **pTexflags,
				*pTexflagarrays[]={m_Caps.m_Valid2DTextureFormats,m_Caps.m_Valid3DTextureFormats,
				m_Caps.m_ValidCubemapFormats,0};

			const D3DRESOURCETYPE rtype[]={D3DRTYPE_TEXTURE,D3DRTYPE_VOLUMETEXTURE,D3DRTYPE_CUBETEXTURE};

			pTexflags=pTexflagarrays;
			unsigned int ii=0;

			base::log("D3D9Device::init()",base::Message) << "Probing texture formats\n";

			while (*pTexflags) {

				video::Caps::Texflags *pTexflagarray=*pTexflags;

				for (unsigned int i=0;i<video::Pixelformat_NumFormats;++i) {
					D3DFORMAT fmt=d3dpixelformat((video::Pixelformat)i);
					hr=0;

					hr=m_pD3D9->CheckDeviceFormat(adapter,D3DDEVTYPE_HAL,adapterfmt,0,rtype[ii],fmt);
					if (FAILED(hr)) { pTexflagarray[i].m_Supported=false; continue; }
					else pTexflagarray[i].m_Supported=true;

					if ((i==video::Pixelformat_RGB_DXT1)
						|| (i==video::Pixelformat_RGBA_DXT1)
						|| (i==video::Pixelformat_RGBA_DXT3)
						|| (i==video::Pixelformat_RGBA_DXT5))
						m_Caps.m_CompressedTextures=true;

					// Volume textures are not supported as render targets
					if (rtype[ii]!=D3DRTYPE_VOLUMETEXTURE) {
						hr=m_pD3D9->CheckDeviceFormat(adapter,D3DDEVTYPE_HAL,adapterfmt,D3DUSAGE_RENDERTARGET,rtype[ii],fmt);
						pTexflagarray[i].m_Rendertarget=(hr==D3D_OK);
					} else pTexflagarray[i].m_Rendertarget=false;

					hr=m_pD3D9->CheckDeviceFormat(adapter,D3DDEVTYPE_HAL,adapterfmt,D3DUSAGE_DYNAMIC,rtype[ii],fmt);
					pTexflagarray[i].m_Dynamic=(hr==D3D_OK);

					// For depth buffers, only depth formats are supported
					// Volume textures are not supported
					if (isDepthformat((video::Pixelformat)i) && (rtype[ii]!=D3DRTYPE_VOLUMETEXTURE)) {
						hr=m_pD3D9->CheckDeviceFormat(adapter,D3DDEVTYPE_HAL,adapterfmt,D3DUSAGE_DEPTHSTENCIL,rtype[ii],fmt);
						pTexflagarray[i].m_Depth=(hr==D3D_OK);
					} else pTexflagarray[i].m_Depth=false;

					hr=m_pD3D9->CheckDeviceFormat(adapter,D3DDEVTYPE_HAL,adapterfmt,D3DUSAGE_DMAP,rtype[ii],fmt);
					pTexflagarray[i].m_DisplacementMap=(hr==D3D_OK);
				}

				++pTexflags;
				++ii;
			}
		}

		base::log("D3D9Device::init()",base::Message) << "Creating D3D device\n";

		hr=m_pD3D9->CreateDevice(adapter,D3DDEVTYPE_HAL,hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,&m_D3DPP,&m_pD3DDev9);

		if (FAILED(hr)) {
			base::log("D3D9Device::init()",base::Error) << "CreateDevice() failed\n";
			m_IsValid=false;
			return;
		}

		D3DVIEWPORT9 viewport;
		viewport.X=0;
		viewport.Y=0;
		viewport.Width=r.right-r.left;
		viewport.Height=r.bottom-r.top;
		viewport.MinZ=0;
		viewport.MaxZ=1;
		m_pD3DDev9->SetViewport(&viewport);
			
		base::log("D3D9Device::init()",base::Message) << "Probing queries\n";
		{
			IDirect3DQuery9* pQuery=0;
			hr=m_pD3DDev9->CreateQuery(D3DQUERYTYPE_OCCLUSION,&pQuery);
			m_Caps.m_HWOcclusionQueries=(hr==D3D_OK);
			if (pQuery) pQuery->Release();
		}
			
		{
			base::log("D3D9Device::init()",base::Message) << "Probing vertex program support\n";

			// Supported vertex programs
			{
				unsigned int version=HIBYTE(LOWORD(m_D3DCaps.VertexShaderVersion));
				unsigned int subversion=LOBYTE(LOWORD(m_D3DCaps.VertexShaderVersion));

				if (version>=1) {
					if (subversion>=1) m_Caps.m_SupportedProgramFormats.addString("d3d_vs11");
				}

				if (version>=2) {
					if (subversion>=0) m_Caps.m_SupportedProgramFormats.addString("d3d_vs20");
				}

				if (version>=3) {
					if (subversion>=0) m_Caps.m_SupportedProgramFormats.addString("d3d_vs30");
				}

				// HLSL support

				LPCSTR vsprofile=D3DXGetVertexShaderProfile(m_pD3DDev9);
				base::String hlslvsprofile;
				if (vsprofile) hlslvsprofile=vsprofile;

				if (hlslvsprofile.compare("vs_1_1",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs11");
				} else if (hlslvsprofile.compare("vs_2_0",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs20");
				} else if (hlslvsprofile.compare("vs_2_a",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs20");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs2a");
				} else if (hlslvsprofile.compare("vs_3_0",true) || (version>=3)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs20");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs2a");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_vs30");
				}
			}

			base::log("D3D9Device::init()",base::Message) << "Probing fragment program support\n";

			// Supported fragment programs
			{
				unsigned int version=HIBYTE(LOWORD(m_D3DCaps.PixelShaderVersion));
				unsigned int subversion=LOBYTE(LOWORD(m_D3DCaps.PixelShaderVersion));

				if (version>=1) {
					if (subversion>=1) m_Caps.m_SupportedProgramFormats.addString("d3d_ps11");
					if (subversion>=2) m_Caps.m_SupportedProgramFormats.addString("d3d_ps12");
					if (subversion>=3) m_Caps.m_SupportedProgramFormats.addString("d3d_ps13");
					if (subversion>=4) m_Caps.m_SupportedProgramFormats.addString("d3d_ps14");
				}

				if (version>=2) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps14");
					if (subversion>=0) m_Caps.m_SupportedProgramFormats.addString("d3d_ps20");
				}

				if (version>=3) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps14");
					m_Caps.m_SupportedProgramFormats.addString("d3d_ps20");
					if (subversion>=0) m_Caps.m_SupportedProgramFormats.addString("d3d_ps30");
				}

				// HLSL support

				LPCSTR vsprofile=D3DXGetPixelShaderProfile(m_pD3DDev9);
				base::String hlslpsprofile;
				if (vsprofile) hlslpsprofile=vsprofile;

				if (hlslpsprofile.compare("ps_1_1",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
				} else if (hlslpsprofile.compare("ps_1_2",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
				} else if (hlslpsprofile.compare("ps_1_3",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps13");
				} else if (hlslpsprofile.compare("ps_1_4",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps14");
				} else if (hlslpsprofile.compare("ps_2_0",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps14");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps20");
				} else if (hlslpsprofile.compare("ps_2_a",true)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps14");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps20");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps2a");
				} else if (hlslpsprofile.compare("ps_3_0",true) || (version>=3)) {
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps11");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps12");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps13");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps14");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps20");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps2a");
					m_Caps.m_SupportedProgramFormats.addString("d3d_hlsl_ps30");
				}
			}

		} // Caps end

		m_Displaywidth=viewport.Width;
		m_Displayheight=viewport.Height;
		m_ColorbufferFormat=colorbufferformat;
		m_DepthstencilbufferFormat=depthstencilformat;

		fixedLighting(false);

		base::log("D3D9Device::init()",base::Message) << "Init complete\n";
	}

	bool D3D9Device::testVertexProgram(const base::String& program,const base::String& format)
	{
		if (m_Caps.m_SupportedProgramFormats.find(format,true)==0) return false;

		return true;
	}

	bool D3D9Device::testFragmentProgram(const base::String& program,const base::String& format)
	{
		if (m_Caps.m_SupportedProgramFormats.find(format,true)==0) return false;

		return true;
	}

	ovis_uint32 D3D9Device::displaywidth() const
	{
		return m_Displaywidth;
	}

	ovis_uint32 D3D9Device::displayheight() const
	{
		return m_Displayheight;
	}

	video::Pixelformat D3D9Device::colorbufferFormat() const
	{
		return m_ColorbufferFormat;
	}

	video::Pixelformat D3D9Device::depthstencilbufferFormat() const
	{
		return m_DepthstencilbufferFormat;
	}










	void D3D9Device::clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue,
		const ovis_uint32 stencilvalue)
	{
		ovis_uint32 clearflags=
			(((buffers&video::Clear_Colorbuffer)!=0) ? D3DCLEAR_TARGET  : 0)|
			(((buffers&video::Clear_Stencilbuffer)!=0) ? D3DCLEAR_STENCIL  : 0)|
			(((buffers&video::Clear_Depthbuffer)!=0) ? D3DCLEAR_ZBUFFER  : 0);

		ovis_uint32 col=
			(((ovis_uint32)(color[0]*255.0f))<<24)|
			(((ovis_uint32)(color[1]*255.0f))<<16)|
			(((ovis_uint32)(color[2]*255.0f))<<8)|
			(((ovis_uint32)(color[3]*255.0f)));

		m_pD3DDev9->Clear(0,NULL,clearflags,col,zvalue,stencilvalue);
	}

	void D3D9Device::begin()
	{
		m_pD3DDev9->BeginScene();
	}

	void D3D9Device::end()
	{
		m_pD3DDev9->EndScene();
	}

	void D3D9Device::updateScreen()
	{
		m_pD3DDev9->Present(0,0,0,0);
	}

	bool D3D9Device::resizeViewport(const ovis_uint32 newx,const ovis_uint32 newy,
		const ovis_uint32 newwidth,const ovis_uint32 newheight,video::ViewportResizingCallback* callback)
	{
		m_D3DPP.BackBufferWidth=m_D3DPP.Windowed ? 0 : newwidth;
		m_D3DPP.BackBufferHeight=m_D3DPP.Windowed ? 0 : newheight;

		// Notify all resources
		{
			std::list<D3D9Texture2D* > m_2DTextures;
			std::list<D3D9Cubemap* > m_Cubemaps;
			std::list<D3D9Vertexstream* > m_Vertexstreams;
			std::list<D3D9Indexstream* > m_Indexstreams;

			std::list<D3D9Texture2D* >::iterator it2dtex=m_pResources->m_2DTextures.begin();
			std::list<D3D9Cubemap* >::iterator itcubemap=m_pResources->m_Cubemaps.begin();
			std::list<D3D9Vertexstream* >::iterator itvstr=m_pResources->m_Vertexstreams.begin();
			std::list<D3D9Indexstream* >::iterator itistr=m_pResources->m_Indexstreams.begin();

			for (;it2dtex!=m_pResources->m_2DTextures.end();++it2dtex) (*it2dtex)->d3d9releaseBeforeReset();
			for (;itcubemap!=m_pResources->m_Cubemaps.end();++itcubemap) (*itcubemap)->d3d9releaseBeforeReset();
			for (;itvstr!=m_pResources->m_Vertexstreams.end();++itvstr) (*itvstr)->d3d9releaseBeforeReset();
			for (;itistr!=m_pResources->m_Indexstreams.end();++itistr) (*itistr)->d3d9releaseBeforeReset();
		}
		
		HRESULT hr=m_pD3DDev9->Reset(&m_D3DPP);
		do {
			if (hr==D3DERR_DRIVERINTERNALERROR) return false;
			hr=m_pD3DDev9->TestCooperativeLevel();

			if (hr==D3DERR_DEVICENOTRESET) hr=m_pD3DDev9->Reset(&m_D3DPP);
		} while (hr!=D3D_OK);

		if (callback) callback->checkIonVideodeviceResources();

		D3DVIEWPORT9 vp;
		vp.X=newx;
		vp.Y=newy;
		vp.Width=newwidth;
		vp.Height=newheight;
		vp.MinZ=0;
		vp.MaxZ=1;
		m_pD3DDev9->SetViewport(&vp);

		return true;
	}







	video::Vertexstream* D3D9Device::createVertexstream(const ovis_uint32 numVertices,const video::Vertexformat& format,
		const ovis_uint32 flags,const video::Memorypool pool)
	{
		return new D3D9Vertexstream(*this,format,flags,pool,numVertices,0);
	}

	video::Indexstream* D3D9Device::createIndexstream(const ovis_uint32 numIndices,const video::Indexformat format,
		const ovis_uint32 flags,const video::Memorypool pool)
	{
		return new D3D9Indexstream(*this,format,flags,pool,numIndices);
	}

	video::Texture2D* D3D9Device::create2DTexture(const base::String& identifier,
		const ovis_uint32 width,const ovis_uint32 height,
		const ovis_uint32 levels,const ovis_uint32 flags,const video::Pixelformat format,const video::Memorypool pool)
	{
		return new D3D9Texture2D(*this,identifier,width,height,levels,flags,format,pool);
	}

	video::Cubemap* D3D9Device::createCubemap(const base::String& identifier,
		const ovis_uint32 edgelength,const ovis_uint32 levels,
		const ovis_uint32 flags,const video::Pixelformat format,const video::Memorypool pool)
	{
		return new D3D9Cubemap(*this,identifier,edgelength,levels,flags,format,pool);
	}

	video::Occlusionquery* D3D9Device::createOcclusionquery()
	{
		return 0;
	}

	video::Program* D3D9Device::createVertexProgram(const base::String& identifier,
		const base::String& entrypoint,const base::String& program,const base::String& format)
	{
		return new D3D9Vertexprogram(*this,identifier,entrypoint,program,format);
	}

	video::Program* D3D9Device::createFragmentProgram(const base::String& identifier,
		const base::String& entrypoint,const base::String& program,const base::String& format)
	{
		return new D3D9Fragmentprogram(*this,identifier,entrypoint,program,format);
	}








	void D3D9Device::drawPrimitives(const video::PrimitivesType type,
		video::Vertexstream& vertexstream,const ovis_uint32 firstElement,
		const ovis_uint32 numElements)
	{
		vertexstream.bind();
		m_pD3DDev9->DrawPrimitive(d3dprimitivetype(type),firstElement,numElements);
	}

	void D3D9Device::drawIndexedPrimitives(
		const video::PrimitivesType type,
		video::Vertexstream& vertexstream,
		video::Indexstream& indexstream,
		const ovis_uint32 indexOffset,
		const ovis_uint32 numElements)
	{
		vertexstream.bind();
		indexstream.bind();
		m_pD3DDev9->DrawIndexedPrimitive(
			d3dprimitivetype(type),0,0,vertexstream.capacity(),indexOffset,numElements);
	}





	void D3D9Device::blending(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_ALPHABLENDENABLE,state ? TRUE : FALSE);
	}

	void D3D9Device::zbuffer(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_ZENABLE,state ? D3DZB_TRUE : D3DZB_FALSE);
	}

	void D3D9Device::zwrite(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_ZWRITEENABLE,state ? TRUE : FALSE);
	}

	void D3D9Device::stencilbuffer(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_STENCILENABLE,state ? TRUE : FALSE);
	}

	void D3D9Device::stencilwrite(const ovis_uint32 stencilmask)
	{
		m_pD3DDev9->SetRenderState(D3DRS_STENCILMASK,stencilmask);
	}

	void D3D9Device::culling(const bool state)
	{
		m_CullingEnabled=state;
		m_pD3DDev9->SetRenderState(D3DRS_CULLMODE,state ? m_Cullmode : D3DCULL_NONE);
	}

	void D3D9Device::cullmode(const video::Cullingmode mode)
	{
		switch (mode) {
		case video::Cullingmode_Clockwise:m_Cullmode=D3DCULL_CW; break;
		case video::Cullingmode_Counterclockwise:m_Cullmode=D3DCULL_CCW; break;
		}

		if (m_CullingEnabled)
			m_pD3DDev9->SetRenderState(D3DRS_CULLMODE,m_Cullmode);
	}

	void D3D9Device::blendfunc(const video::BlendFunc srcblend,const video::BlendFunc destblend)
	{
		m_pD3DDev9->SetRenderState(D3DRS_SRCBLEND,d3dblendparameter(srcblend));
		m_pD3DDev9->SetRenderState(D3DRS_DESTBLEND,d3dblendparameter(destblend));
	}

	void D3D9Device::zfunc(const video::CompareFunc func)
	{
		m_pD3DDev9->SetRenderState(D3DRS_ZFUNC,d3dcomparefunc(func));
	}

	void D3D9Device::stencilfunc(const video::CompareFunc func,const ovis_uint32 ref,const ovis_uint32 writemask)
	{
		m_pD3DDev9->SetRenderState(D3DRS_STENCILFUNC,d3dcomparefunc(func));
		m_pD3DDev9->SetRenderState(D3DRS_STENCILREF,ref);
		m_pD3DDev9->SetRenderState(D3DRS_STENCILMASK,writemask);
	}

	void D3D9Device::stencilop(const video::StencilOp opfail,const video::StencilOp opzfail,const video::StencilOp oppass)
	{
		m_pD3DDev9->SetRenderState(D3DRS_STENCILFAIL,d3dstenciloperation(opfail));
		m_pD3DDev9->SetRenderState(D3DRS_STENCILZFAIL,d3dstenciloperation(opzfail));
		m_pD3DDev9->SetRenderState(D3DRS_STENCILPASS,d3dstenciloperation(oppass));
	}

	void D3D9Device::drawingmode(const video::Drawingmode mode)
	{
		switch (mode) {
			case video::Drawingmode_Points:m_pD3DDev9->SetRenderState(D3DRS_FILLMODE,D3DFILL_POINT); break;
			case video::Drawingmode_Lines:m_pD3DDev9->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME); break;
			case video::Drawingmode_Faces:m_pD3DDev9->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID); break;
			default:break;
		}
	}

	void D3D9Device::solidshading(const video::Solidshading type)
	{
		switch (type) {
			case video::Solidshading_Flat:m_pD3DDev9->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT); break;
			case video::Solidshading_Smooth:m_pD3DDev9->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD); break;
			default:break;
		}
	}

	void D3D9Device::depthbias(const float scaledbias,const float uniformbias)
	{
		m_pD3DDev9->SetRenderState(D3DRS_DEPTHBIAS,*((DWORD*)(&uniformbias)) );
		m_pD3DDev9->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS,*((DWORD*)(&scaledbias)) );
	}


	unsigned long D3D9Device::samplerstageParameterInt(const ovis_uint32 texstage,const video::Samplerstageparameter texparam) const
	{
		return 0;
	}

	float D3D9Device::samplerstageParameterFloat(const ovis_uint32 texstage,const video::Samplerstageparameter texparam) const
	{
		return 0;
	}

	void D3D9Device::samplerstageParameterInt(const ovis_uint32 texstage,const video::Samplerstageparameter texparam,const unsigned long value)
	{
		const char *where="D3D9Device::samplerstageParameterInt()";

		switch (texparam) {

			case video::Samplerstageparameter_MaxAnisotropy:
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_MAXANISOTROPY,value); break;

			case video::Samplerstageparameter_Minfilter: {
				D3DTEXTUREFILTERTYPE d3dtft=d3dtexturefilter( (video::Texfilter)value );
				if (d3dtft==D3DTEXF_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture min filter type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_MINFILTER,d3dtft);
			} break;

			case video::Samplerstageparameter_Magfilter: {
				D3DTEXTUREFILTERTYPE d3dtft=d3dtexturefilter( (video::Texfilter)value );
				if (d3dtft==D3DTEXF_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture mag filter type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_MAGFILTER,d3dtft);
			} break;

			case video::Samplerstageparameter_Mipfilter: {
				D3DTEXTUREFILTERTYPE d3dtft=d3dtexturefilter( (video::Texfilter)value );
				if (d3dtft==D3DTEXF_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture mip filter type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_MIPFILTER,d3dtft);
			} break;

			case video::Samplerstageparameter_UWrap: {
				D3DTEXTUREADDRESS d3dadr=d3dtextureaddress((video::Texwrap)value);
				if (d3dadr==D3DTADDRESS_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture U address type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_ADDRESSU,d3dadr);
			} break;

			case video::Samplerstageparameter_VWrap: {
				D3DTEXTUREADDRESS d3dadr=d3dtextureaddress((video::Texwrap)value);
				if (d3dadr==D3DTADDRESS_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture V address type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_ADDRESSV,d3dadr); break;
			} break;

			case video::Samplerstageparameter_WWrap: {
				D3DTEXTUREADDRESS d3dadr=d3dtextureaddress((video::Texwrap)value);
				if (d3dadr==D3DTADDRESS_FORCE_DWORD) base::log(where,base::Warning) << "Invalid texture W address type\n";
				m_pD3DDev9->SetSamplerState(texstage,D3DSAMP_ADDRESSW,d3dadr); break;
			} break;

		}
	}

	void D3D9Device::samplerstageParameterFloat(const ovis_uint32 texstage,const video::Samplerstageparameter texparam,const float value)
	{
	}


	unsigned long D3D9Device::texstageParameterInt(const ovis_uint32 texstage,const video::Texstageparameter texparam) const
	{
		return 0;
	}

	float D3D9Device::texstageParameterFloat(const ovis_uint32 texstage,const video::Texstageparameter texparam) const
	{
		return 0;
	}

	void D3D9Device::texstageParameterInt(const ovis_uint32 texstage,const video::Texstageparameter texparam,const unsigned long value)
	{
		switch (texparam) {

			case video::Texstageparameter_Colorop: {
				D3DTEXTUREOP d3dtexop=d3dtextureop((video::Texstageoperation)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_COLOROP,d3dtexop);
			} break;

			case video::Texstageparameter_Colorarg1: {
				DWORD d3dtexarg=d3dtexturearg((video::Texstageargument)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_COLORARG1,d3dtexarg);
			} break;

			case video::Texstageparameter_Colorarg2: {
				DWORD d3dtexarg=d3dtexturearg((video::Texstageargument)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_COLORARG2,d3dtexarg);
			} break;

			case video::Texstageparameter_Alphaop: {
				D3DTEXTUREOP d3dtexop=d3dtextureop((video::Texstageoperation)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_ALPHAOP,d3dtexop);
			} break;

			case video::Texstageparameter_Alphaarg1: {
				DWORD d3dtexarg=d3dtexturearg((video::Texstageargument)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_ALPHAARG1,d3dtexarg);
			} break;

			case video::Texstageparameter_Alphaarg2: {
				DWORD d3dtexarg=d3dtexturearg((video::Texstageargument)value);
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_ALPHAARG2,d3dtexarg);
			} break;

			case video::Texstageparameter_Texcoordindex: {
				m_Texcoordindexmodes[texstage]=value;
				DWORD dw=m_Texcoordindexmodes[texstage]|m_Texcoordgenmodes[texstage];
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_TEXCOORDINDEX,dw);
			} break;

			case video::Texstageparameter_Textransformations: {
				// TODO: implement textransformations
			} break;

			case video::Texstageparameter_Constant: {
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_CONSTANT,value);
			} break;

			case video::Texstageparameter_Texcoordgeneration: {
				m_Texcoordgenmodes[texstage]=d3dtexcoordgen((video::Texcoordgeneration)value);
				DWORD dw=m_Texcoordindexmodes[texstage]|m_Texcoordgenmodes[texstage];
				m_pD3DDev9->SetTextureStageState(texstage,D3DTSS_TEXCOORDINDEX,dw);
			} break;

		}
	}

	void D3D9Device::texstageParameterFloat(const ovis_uint32 texstage,const video::Texstageparameter texparam,const float value)
	{
	}


	void D3D9Device::vprogramSetFloat2(const base::String& varname,const base::Float2& f)
	{
		if (m_pCurrentVertexProgram) m_pCurrentVertexProgram->setFloat2(varname,f);
	}

	void D3D9Device::vprogramSetFloat3(const base::String& varname,const base::Float3& f)
	{
		if (m_pCurrentVertexProgram) m_pCurrentVertexProgram->setFloat3(varname,f);
	}

	void D3D9Device::vprogramSetFloat4(const base::String& varname,const base::Float4& f)
	{
		if (m_pCurrentVertexProgram) m_pCurrentVertexProgram->setFloat4(varname,f);
	}

	void D3D9Device::vprogramSetFloat3x3(const base::String& varname,const base::Float3x3& f)
	{
		if (m_pCurrentVertexProgram) m_pCurrentVertexProgram->setFloat3x3(varname,f);
	}

	void D3D9Device::vprogramSetFloat4x3(const base::String& varname,const base::Float4x3& f)
	{
		if (m_pCurrentVertexProgram) m_pCurrentVertexProgram->setFloat4x3(varname,f);
	}

	void D3D9Device::vprogramSetFloat3x4(const base::String& varname,const base::Float3x4& f)
	{
		if (m_pCurrentVertexProgram) m_pCurrentVertexProgram->setFloat3x4(varname,f);
	}

	void D3D9Device::vprogramSetFloat4x4(const base::String& varname,const base::Float4x4& f)
	{
		if (m_pCurrentVertexProgram) m_pCurrentVertexProgram->setFloat4x4(varname,f);
	}

	void D3D9Device::fprogramSetFloat2(const base::String& varname,const base::Float2& f)
	{
		if (m_pCurrentFragmentProgram) m_pCurrentFragmentProgram->setFloat2(varname,f);
	}

	void D3D9Device::fprogramSetFloat3(const base::String& varname,const base::Float3& f)
	{
		if (m_pCurrentFragmentProgram) m_pCurrentFragmentProgram->setFloat3(varname,f);
	}

	void D3D9Device::fprogramSetFloat4(const base::String& varname,const base::Float4& f)
	{
		if (m_pCurrentFragmentProgram) m_pCurrentFragmentProgram->setFloat4(varname,f);
	}

	void D3D9Device::fprogramSetFloat3x3(const base::String& varname,const base::Float3x3& f)
	{
		if (m_pCurrentFragmentProgram) m_pCurrentFragmentProgram->setFloat3x3(varname,f);
	}

	void D3D9Device::fprogramSetFloat4x3(const base::String& varname,const base::Float4x3& f)
	{
		if (m_pCurrentFragmentProgram) m_pCurrentFragmentProgram->setFloat4x3(varname,f);
	}

	void D3D9Device::fprogramSetFloat3x4(const base::String& varname,const base::Float3x4& f)
	{
		if (m_pCurrentFragmentProgram) m_pCurrentFragmentProgram->setFloat3x4(varname,f);
	}

	void D3D9Device::fprogramSetFloat4x4(const base::String& varname,const base::Float4x4& f)
	{
		if (m_pCurrentFragmentProgram) m_pCurrentFragmentProgram->setFloat4x4(varname,f);
	}

	void D3D9Device::fprogramBind2DTexture(const base::String& varname,const video::Texture2D* texVar)
	{
		if (m_pCurrentFragmentProgram) m_pCurrentFragmentProgram->bind2DTexture(varname,texVar);
	}

	void D3D9Device::fprogramBindCubemap(const base::String& varname,const video::Cubemap* texVar)
	{
		if (m_pCurrentFragmentProgram) m_pCurrentFragmentProgram->bindCubemap(varname,texVar);
	}

	video::Program* D3D9Device::vertexprogram()
	{
		return m_pCurrentVertexProgram;
	}

	video::Program* D3D9Device::fragmentprogram()
	{
		return m_pCurrentFragmentProgram;
	}

	void D3D9Device::vertexprogram(video::Program* pVertexprogram)
	{
		if (m_pCurrentVertexProgram==pVertexprogram) return;
		m_pCurrentVertexProgram=pVertexprogram;
	}

	void D3D9Device::fragmentprogram(video::Program* pFragmentprogram)
	{
		if (m_pCurrentFragmentProgram==pFragmentprogram) return;
		m_pCurrentFragmentProgram=pFragmentprogram;
	}


	void D3D9Device::activateFFPipeline(const video::Fixedfunctionactivationtype pipeline)
	{
		if (pipeline==video::FixedfunctionVertex) {
			m_pCurrentVertexProgram=0;
			m_pD3DDev9->SetVertexShader(0);
		} else {
			m_pCurrentFragmentProgram=0;
			m_pD3DDev9->SetPixelShader(0);
		}
	}

	void D3D9Device::fixedSetMaterialAmbientColor(const base::Float4& color)
	{
		m_D3D9FFMaterial.Ambient.a=color[0];
		m_D3D9FFMaterial.Ambient.r=color[1];
		m_D3D9FFMaterial.Ambient.g=color[2];
		m_D3D9FFMaterial.Ambient.b=color[3];
		m_pD3DDev9->SetMaterial(&m_D3D9FFMaterial);
	}

	void D3D9Device::fixedSetMaterialDiffuseColor(const base::Float4& color)
	{
		m_D3D9FFMaterial.Diffuse.a=color[0];
		m_D3D9FFMaterial.Diffuse.r=color[1];
		m_D3D9FFMaterial.Diffuse.g=color[2];
		m_D3D9FFMaterial.Diffuse.b=color[3];
		m_pD3DDev9->SetMaterial(&m_D3D9FFMaterial);
	}

	void D3D9Device::fixedSetMaterialSpecularColor(const base::Float4& color)
	{
		m_D3D9FFMaterial.Specular.a=color[0];
		m_D3D9FFMaterial.Specular.r=color[1];
		m_D3D9FFMaterial.Specular.g=color[2];
		m_D3D9FFMaterial.Specular.b=color[3];
		m_pD3DDev9->SetMaterial(&m_D3D9FFMaterial);
	}

	void D3D9Device::fixedSetMaterialEmissiveColor(const base::Float4& color)
	{
		m_D3D9FFMaterial.Emissive.a=color[0];
		m_D3D9FFMaterial.Emissive.r=color[1];
		m_D3D9FFMaterial.Emissive.g=color[2];
		m_D3D9FFMaterial.Emissive.b=color[3];
		m_pD3DDev9->SetMaterial(&m_D3D9FFMaterial);
	}

	void D3D9Device::fixedSetMaterialShininess(const float shininess)
	{
		m_D3D9FFMaterial.Power=shininess;
		m_pD3DDev9->SetMaterial(&m_D3D9FFMaterial);
	}

	void D3D9Device::fixedSet2DTexture(const ovis_uint32 texstage,const video::Texture2D* texVar)
	{
		m_pD3DDev9->SetTexture(texstage,
			texVar ? ((D3D9Texture2D*)texVar)->d3d9texture() : 0);
	}

	ovis_uint32 D3D9Device::fixedMaxTextures()
	{
		return 0;
	}

	ovis_uint32 D3D9Device::fixedMaxTextureStages()
	{
		return m_Caps.m_MaxTextureBlendStages;
	}

	void D3D9Device::fixedLighting(const bool state)
	{
		m_pD3DDev9->SetRenderState(D3DRS_LIGHTING,state ? TRUE : FALSE);
	}

	void D3D9Device::fixedSetLightEnabled(const ovis_uint32 lightNr,const bool state)
	{
		m_pD3DDev9->LightEnable(lightNr,state ? TRUE : FALSE);
	}

	void D3D9Device::fixedSetLightProperties(const ovis_uint32 lightNr,const video::FFLightProperties& properties)
	{
		D3DLIGHT9 d3dlight9;
		memset(&d3dlight9,0,sizeof(d3dlight9));
		switch (properties.m_Lighttype) {
			case video::Lighttype_Directional:d3dlight9.Type=D3DLIGHT_DIRECTIONAL; break;
			case video::Lighttype_Point:d3dlight9.Type=D3DLIGHT_POINT; break;
			case video::Lighttype_Spot:d3dlight9.Type=D3DLIGHT_SPOT; break;
		}
		d3dlight9.Position.x=properties.m_Position.x();
		d3dlight9.Position.y=properties.m_Position.y();
		d3dlight9.Position.z=properties.m_Position.z();
		d3dlight9.Direction.x=properties.m_Direction.x();
		d3dlight9.Direction.y=properties.m_Direction.y();
		d3dlight9.Direction.z=properties.m_Direction.z();
		d3dlight9.Diffuse.a=properties.m_Color[0];
		d3dlight9.Diffuse.r=properties.m_Color[1];
		d3dlight9.Diffuse.g=properties.m_Color[2];
		d3dlight9.Diffuse.b=properties.m_Color[3];
		d3dlight9.Range=properties.m_Range;
		d3dlight9.Falloff=properties.m_Falloff;
		d3dlight9.Attenuation0=properties.m_ConstantAttenuation;
		d3dlight9.Attenuation1=properties.m_LinearAttenuation;
		d3dlight9.Attenuation2=properties.m_QuadraticAttenuation;

		m_pD3DDev9->SetLight(lightNr,&d3dlight9);
	}

	void D3D9Device::fixedSetProjectionMatrix(const base::Float4x4& matrix)
	{
		D3DMATRIX m;
		tod3dmatrix(matrix,m,false);
		m_pD3DDev9->SetTransform(D3DTS_PROJECTION,&m);
	}

	void D3D9Device::fixedSetWorldMatrix(const base::Float4x4& matrix)
	{
		D3DMATRIX m;
		tod3dmatrix(matrix,m,false);
		m_pD3DDev9->SetTransform(D3DTS_WORLD,&m);
	}

	void D3D9Device::fixedSetViewMatrix(const base::Float4x4& matrix)
	{
		D3DMATRIX m;
		tod3dmatrix(matrix,m,false);
		m_pD3DDev9->SetTransform(D3DTS_VIEW,&m);
	}

	ovis_uint32 D3D9Device::fixedMaxLights() const
	{
		return m_D3DCaps.MaxActiveLights;
	}





}
}


extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
ovis::video::Videodevice* createVideodeviceInstance()
{
	return new ovis::d3d9drv::D3D9Device;
}


extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
const char* videodeviceDescription()
{
	return "Direct3D 9 videodevice v0.5 build 2005-08-12 18:30";
}

