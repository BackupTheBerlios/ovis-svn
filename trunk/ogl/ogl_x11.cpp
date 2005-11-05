/* Standard OpenGL/GLX header files */
#include <GL/glx.h>
#include <GL/gl.h>
/* Headers needed for keys used in the program */
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>

#include "../base/log.hh"
#include "../base/window.hh"
#include "ovisgl.hh"
#include "ogldevice.hh"

namespace ovis {
namespace opengldrv {

/* Define our window structure here
 *
 * Description of Structure Members:
 *
 * Display 				*dpy: 	This is the display. In X Windows, a display considers
 *								everything. Monitor, Mouse, Keyboard.
 * int	  				screen:	Which window we are rendering too ?
 * Window 				win:	This is our drawable surrface
 * GLXContext 			ctx:	Stores information about our OpenGL Context
 * XSetWindowAttributes attr:
 * BOOL					fs:		This is the fullscreen flag. Weither or not we have a fullscreen
 *								or not. 
 * XF86VidModeModeInfo 	deskmode:
 * int					x,y:
 * unsigned int			width:	Width of the window
 * unsigned int			height:	Height of the window
 * unsigned int 		bpp:	Bits Per Pixel. With XFree86, highest we can acheive is 24.	
 */

	struct OGLDevice::OGLInternaldata
	{
		::Display 				*dpy;
		int						screen;
		::Window					win;
		GLXContext				ctx;
		XSetWindowAttributes	attr;
		BOOL					fs;
		XF86VidModeModeInfo		deskMode;
		int						x,y;
		unsigned int			width, height;
		unsigned int			bpp;
	};

	void OGLDevice::initGL(base::Window* pWindow,const ovis_uint32 adapter,
		const video::Pixelformat colorbufferformat,
		const video::Pixelformat depthstencilformat,const ovis_uint32 Hz)
	{
		if (m_pOGLInternaldata) return;
		m_pOGLInternaldata=new OGLInternaldata;

		int attrListSgl[] = {GLX_RGBA, GLX_RED_SIZE, 4,
							GLX_GREEN_SIZE, 4,
							GLX_BLUE_SIZE, 4,
							GLX_DEPTH_SIZE, 16,
							None};

		int attrListDbl[] = {GLX_RGBA, GLX_DOUBLEBUFFER,
							GLX_RED_SIZE, 4,
							GLX_GREEN_SIZE, 4,
							GLX_BLUE_SIZE, 4,
							GLX_DEPTH_SIZE, 16,
							None};

		XVisualInfo *vi;
		Colormap cmap;
		int dpyWidth, dpyHeight;
		int i;
		int vidModeMajorVersion, vidModeMinorVersion;
		XF86VidModeModeInfo **modes;
		int modeNum;
		int bestMode;
		unsigned int borderDummy;

		m_pOGLInternaldata->fs=pWindow->fullscreenMode();
		bestMode=0;

		m_pOGLInternaldata->dpy=( ::Display*)(pWindow->displayhandle());
		m_pOGLInternaldata->win=( ::Window)(pWindow->windowhandle());
		m_pOGLInternaldata->screen=DefaultScreen(m_pOGLInternaldata->dpy);
		XF86VidModeQueryVersion(m_pOGLInternaldata->dpy, &vidModeMajorVersion,&vidModeMinorVersion);

		XF86VidModeGetAllModeLines(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, &modeNum, &modes);

		m_pOGLInternaldata->deskMode = *modes[0];

		for (i=0;i<modeNum;i++) {
			if ((modes[i]->hdisplay==pWindow->width()) && (modes[i]->vdisplay==pWindow->height())) bestMode=i;
		}

		vi=glXChooseVisual(m_pOGLInternaldata->dpy,m_pOGLInternaldata->screen,attrListDbl);
		if(NULL==vi) {
			vi=glXChooseVisual(m_pOGLInternaldata->dpy,m_pOGLInternaldata->screen,attrListSgl);
			base::log("OGLDevice::initGL()",base::Message) << "Singlebuffered : true\n";
		} else {
			base::log("OGLDevice::initGL()",base::Message) << "Doublebuffered : true\n";
		}

		m_pOGLInternaldata->ctx = glXCreateContext(m_pOGLInternaldata->dpy, vi, 0, GL_TRUE);
		
		if (pWindow->fullscreenMode()) {
			XF86VidModeSwitchToMode(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, modes[bestMode]);
			XF86VidModeSetViewPort(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, 0, 0);
			dpyWidth = modes[bestMode]->hdisplay;
			dpyHeight = modes[bestMode]->vdisplay;
			XFree(modes);
		}
		
		glXMakeCurrent(m_pOGLInternaldata->dpy, m_pOGLInternaldata->win, m_pOGLInternaldata->ctx);
		/*XGetGeometry(GLWin.dpy, GLWin.win, &winDummy, &GLWin.x, &GLWin.y,
				&GLWin.width, &GLWin.height, &borderDummy, &GLWin.bpp);*/

		base::log("OGLDevice::initGL()",base::Message) << "Direct Rendering: " <<
			(glXIsDirect(m_pOGLInternaldata->dpy, m_pOGLInternaldata->ctx) ? "true" : "false") << "\n";

		base::log("OGLDevice::initGL()",base::Message) << "Running in " <<
			(m_pOGLInternaldata->fs ? "fullscreen" : "window") << " mode\n";
	}
	
	void OGLDevice::updateGL()
	{
		glXSwapBuffers(m_pOGLInternaldata->dpy,m_pOGLInternaldata->win);
	}
	
	void OGLDevice::shutdownGL()
	{
		if (!m_pOGLInternaldata) return;
	
		if(m_pOGLInternaldata->ctx)
		{
			if(!glXMakeCurrent(m_pOGLInternaldata->dpy, None, NULL))
			{
				base::log("OGLDevice::shutdownGL()",base::FatalError) << "Error releasing drawing context : killGLWindow\n";
			}
			glXDestroyContext(m_pOGLInternaldata->dpy, m_pOGLInternaldata->ctx);
			m_pOGLInternaldata->ctx = NULL;
		}

		if(m_pOGLInternaldata->fs)
		{
			XF86VidModeSwitchToMode(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, &(m_pOGLInternaldata->deskMode));
			XF86VidModeSetViewPort(m_pOGLInternaldata->dpy, m_pOGLInternaldata->screen, 0, 0);
		}

		delete m_pOGLInternaldata;
		m_pOGLInternaldata=0;
	}

}
}
