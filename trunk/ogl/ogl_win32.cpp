#include <windows.h>
#include <GL/gl.h>
#include <vector>

#include "../base/log.hh"
#include "ovisgl.hh"
#include "ogldevice.hh"

namespace ovis {
namespace opengldrv {

	struct OGLDevice::OGLInternaldata
	{
		HWND m_hWindow;
		HDC m_hDC;
		HGLRC m_hGLRC;
		bool m_Fullscreen;

		struct GDIEnum {
			ovis_uint32 m_Adapterindex,m_Modecount;
		};

		std::vector<GDIEnum> m_Adaptermodescount;
	};

	void OGLDevice::initGL(base::Window* pWindow,const ovis_uint32 adapter,
		const video::Pixelformat colorbufferformat,
		const video::Pixelformat depthstencilformat,const ovis_uint32 Hz)
	{
		if (m_pOGLInternaldata) return;
		m_pOGLInternaldata=new OGLInternaldata;
		m_pOGLInternaldata->m_Fullscreen=pWindow->fullscreenMode();

		unsigned int bpp=0,alpha=0,depth=0,stencil=0;

		switch (colorbufferformat) {
			case video::Pixelformat_XRGB1555:
			case video::Pixelformat_RGB565:
				alpha=0; bpp=16; break;
			case video::Pixelformat_ARGB1555:
				alpha=1; bpp=16; break;
			case video::Pixelformat_ARGB4444:
				alpha=4; bpp=16; break;
			case video::Pixelformat_RGB888:

			case video::Pixelformat_XRGB8888:
			case video::Pixelformat_XBGR8888:
				alpha=0; bpp=24; break;

			case video::Pixelformat_ARGB8888:
			case video::Pixelformat_ABGR8888:
				alpha=8; bpp=24; break;
			default:
				base::log("OGLDevice::initGL()",base::FatalError) << "Invalid pixelformat for the colorbuffer\n";
				return;
		}

		switch (depthstencilformat) {
			case video::Pixelformat_D16:depth=16; stencil=0; break;
			case video::Pixelformat_D24:depth=24; stencil=0; break;
			case video::Pixelformat_D32:depth=32; stencil=0; break;
			case video::Pixelformat_D15S1:depth=16; stencil=1; break;
			case video::Pixelformat_D24S4:depth=24; stencil=4; break;
			case video::Pixelformat_D24S8:depth=24; stencil=8; break;
			case video::Pixelformat_FD24S8:depth=24; stencil=8; break;
			default:
				base::log("OGLDevice::initGL()",base::FatalError) << "Invalid pixelformat for the depthstencilbuffer\n";
				return;
		}

		PIXELFORMATDESCRIPTOR pfd={
			sizeof(PIXELFORMATDESCRIPTOR),							// Size Of This Pixel Format Descriptor
				1,														// Version Number
				PFD_DRAW_TO_WINDOW |									// Format Must Support Window
				PFD_SUPPORT_OPENGL |									// Format Must Support OpenGL
				PFD_DOUBLEBUFFER,										// Must Support Double Buffering
				PFD_TYPE_RGBA,											// Request An RGBA Format
				bpp,													// Select Our Color Depth
				0, 0, 0, 0, 0, 0,										// Color Bits Ignored
				alpha,														// No Alpha Buffer
				0,														// Shift Bit Ignored
				0,														// No Accumulation Buffer
				0, 0, 0, 0,												// Accumulation Bits Ignored
				depth,														// 16Bit Z-Buffer (Depth Buffer)  
				stencil,														// No Stencil Buffer
				0,														// No Auxiliary Buffer
				PFD_MAIN_PLANE,											// Main Drawing Layer
				0,														// Reserved
				0, 0, 0													// Layer Masks Ignored
		};

		if (pWindow->fullscreenMode())												// Attempt Fullscreen Mode?
		{
			DEVMODE dmScreenSettings;								// Device Mode
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth	= pWindow->width();		// Selected Screen Width
			dmScreenSettings.dmPelsHeight	= pWindow->height();		// Selected Screen Height
			dmScreenSettings.dmBitsPerPel	= bpp;
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

			DISPLAY_DEVICE displaydev;
			EnumDisplayDevices(0,adapter,&displaydev,0);

			ChangeDisplaySettingsEx(displaydev.DeviceName,&dmScreenSettings,
				(HWND)(pWindow->windowhandle()),CDS_FULLSCREEN,0);
		}

		m_pOGLInternaldata->m_hWindow=(HWND)(pWindow->windowhandle());
		m_pOGLInternaldata->m_hDC=GetDC(m_pOGLInternaldata->m_hWindow);
		GLuint PixelFormat=ChoosePixelFormat(m_pOGLInternaldata->m_hDC,&pfd);
		SetPixelFormat(m_pOGLInternaldata->m_hDC,PixelFormat,&pfd);
		m_pOGLInternaldata->m_hGLRC=wglCreateContext(m_pOGLInternaldata->m_hDC);
		wglMakeCurrent(m_pOGLInternaldata->m_hDC,m_pOGLInternaldata->m_hGLRC);

		m_IsValid=true;
	}

	void OGLDevice::updateGL()
	{
		SwapBuffers(m_pOGLInternaldata->m_hDC);
	}

	void OGLDevice::shutdownGL()
	{
		if (!m_pOGLInternaldata) return;

		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(m_pOGLInternaldata->m_hGLRC);
		ReleaseDC(m_pOGLInternaldata->m_hWindow,m_pOGLInternaldata->m_hDC);

		if (m_pOGLInternaldata->m_Fullscreen) ChangeDisplaySettings(NULL,0);

		if (m_pOGLInternaldata) delete m_pOGLInternaldata;
		m_pOGLInternaldata=0;
	}

}
}