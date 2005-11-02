#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "../base/string.hh"
#include "defaultwindow.hh"

namespace ovis {
namespace base {


	long g_X11RefcountDisplayInit=0;
	Display *g_X11Display=0;
	Atom WM_DELETE_WINDOW;

	struct Defaultwindow::Internaldata
	{
		Window::Activatestate m_Activatestate;
		ovis_uint32 m_Width,m_Height;
		bool m_Fullscreen,m_Quit;
		::Window m_X11Windowhandle;
	};

	void initX11()
	{
		if (g_X11RefcountDisplayInit==0) {
			g_X11Display=XOpenDisplay(0);
			WM_DELETE_WINDOW = XInternAtom(g_X11Display, "WM_DELETE_WINDOW", False);
		}
		
		++g_X11RefcountDisplayInit;
	}
	
	void uninitX11()
	{
		if (g_X11RefcountDisplayInit==1) {
			g_X11Display=0;
			g_X11RefcountDisplayInit=0;
		} else --g_X11RefcountDisplayInit;
	}

	struct Defaultwindow::Messagesysparams
	{
	};

	long long Defaultwindow::handleSysMessages(const Messagesysparams& params)
	{
		return 0;
	}

	Defaultwindow::Defaultwindow(const String& title,
		const ovis_uint32 width,const ovis_uint32 height,const bool fullscreen)
	{
		initX11();
	
		m_pInternaldata=new Internaldata;
		m_pInternaldata->m_Quit=false;
		m_pInternaldata->m_Fullscreen=fullscreen;
		m_pInternaldata->m_Width=width;
		m_pInternaldata->m_Height=height;
		m_pInternaldata->m_Activatestate=NoActivationChange;
	
		int blackColor = BlackPixel(g_X11Display, DefaultScreen(g_X11Display));
	
		m_pInternaldata->m_X11Windowhandle=XCreateSimpleWindow(
			g_X11Display,
			DefaultRootWindow(g_X11Display),
			0,0,width,height,
			0,
			blackColor,blackColor);
	
		XSetWMProtocols(g_X11Display,m_pInternaldata->m_X11Windowhandle, &WM_DELETE_WINDOW, 1);
	
		XTextProperty titleprop, iconprop;

		if (!title.empty()) {
			char *pC=const_cast<char*>(title.cstr());
			XStringListToTextProperty(&pC, 1, &titleprop);
			XSetWMName(g_X11Display, m_pInternaldata->m_X11Windowhandle, &titleprop);
			XFree(titleprop.value);

			XStringListToTextProperty(&pC, 1, &iconprop);
			XSetWMIconName(g_X11Display, m_pInternaldata->m_X11Windowhandle, &iconprop);
			XFree(iconprop.value);
		}
		XSync(g_X11Display, False);
		
		XSelectInput(g_X11Display,m_pInternaldata->m_X11Windowhandle, StructureNotifyMask );
		XMapWindow(g_X11Display,m_pInternaldata->m_X11Windowhandle);
		XFlush(g_X11Display);
	}

	Defaultwindow::~Defaultwindow()
	{
		delete m_pInternaldata;
	
		XUnmapWindow(g_X11Display,m_pInternaldata->m_X11Windowhandle);
		XDestroyWindow(g_X11Display,m_pInternaldata->m_X11Windowhandle);
	
		uninitX11();
	}

	Window::Activatestate Defaultwindow::activatestate() const { return m_pInternaldata->m_Activatestate; }


	void Defaultwindow::title(const String& newtitle)
	{
		XTextProperty titleprop, iconprop;

		char *pC=const_cast<char*>(newtitle.cstr());
		XStringListToTextProperty(&pC, 1, &titleprop);
		XSetWMName(g_X11Display, m_pInternaldata->m_X11Windowhandle, &titleprop);
		XFree(titleprop.value);

		XStringListToTextProperty(&pC, 1, &iconprop);
		XSetWMIconName(g_X11Display, m_pInternaldata->m_X11Windowhandle, &iconprop);
		XFree(iconprop.value);

		XSync(g_X11Display, False);
	}

	bool Defaultwindow::fullscreenMode() const { return m_pInternaldata->m_Fullscreen; }
	bool Defaultwindow::quit() const { return m_pInternaldata->m_Quit; }

	void* Defaultwindow::windowhandle() const { return (void*)(m_pInternaldata->m_X11Windowhandle); }
	void* Defaultwindow::displayhandle() const { return g_X11Display; }
	ovis_uint32 Defaultwindow::width() const { return m_pInternaldata->m_Width; }
	ovis_uint32 Defaultwindow::height() const { return m_pInternaldata->m_Height; }

	void Defaultwindow::resetEventstates()
	{
		m_pInternaldata->m_Activatestate=Window::NoActivationChange;
	}

	bool Defaultwindow::handleEvents()
	{
		XEvent ev;
		if (XPending(g_X11Display)) {
			XNextEvent(g_X11Display,&ev);
		
			if (ev.type==ClientMessage) {
				if ( (ev.xclient.format == 32) &&
					(ev.xclient.data.l[0] == (long)WM_DELETE_WINDOW) ) {
					m_pInternaldata->m_Quit=true;
				}
			}
		
			return true;
		}
	
		return false;
	}

}
}
