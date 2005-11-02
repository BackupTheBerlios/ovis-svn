#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../base/string.hh"
#include "defaultwindow.hh"

namespace ovis {
namespace base {

	struct Defaultwindow::Internaldata
	{
		Window::Activatestate m_Activatestate;
		HWND m_hWindow;
		ovis_uint32 m_Width,m_Height;
		bool m_Fullscreen,m_Quit;
	};

	struct Defaultwindow::Messagesysparams
	{
		HWND m_Handle;
		UINT m_Message;
		WPARAM m_wParam;
		LPARAM m_lParam;
	};

	long long Defaultwindow::handleSysMessages(const Messagesysparams& params)
	{
		switch(params.m_Message)
		{
			case WM_ACTIVATEAPP:m_pInternaldata->m_Activatestate=(params.m_wParam==FALSE) ?
									Window::Deactivated : Window::Activated; break;
			case WM_ACTIVATE:m_pInternaldata->m_Activatestate=(params.m_wParam==WA_INACTIVE) ?
								Window::Deactivated : Window::Activated;

				if (params.m_wParam!=WA_INACTIVE) {
					break;
				}
				break;
			case WM_PAINT:ValidateRect(params.m_Handle,0); return 0;
			case WM_ERASEBKGND:return 1;
			case WM_DESTROY:m_pInternaldata->m_Quit=true; return 0;
			default:break;
		}

		return DefWindowProc(params.m_Handle,params.m_Message,params.m_wParam,params.m_lParam);
	}

	namespace ovisdefaultwindowWin32 {

		LRESULT CALLBACK messageHandler(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
		{
#ifdef _MSC_VER
#pragma warning(disable:4312 4244)
#endif
			Defaultwindow *window = (Defaultwindow*)GetWindowLongPtr(handle, GWL_USERDATA);

			if(!window) return DefWindowProc(handle, message, wparam, lparam);

			Defaultwindow::Messagesysparams params;
			params.m_Handle=handle;
			params.m_Message=message;
			params.m_wParam=wparam;
			params.m_lParam=lparam;

			return window->handleSysMessages(params);
#ifdef _MSC_VER
#pragma warning(default:4312 4244)
#endif
		}

		static long refcounter=0;

		void registerClass()
		{
			if (refcounter>0) return;

			WNDCLASSEX windowclass;

			windowclass.cbClsExtra=0;
			windowclass.cbSize=sizeof(windowclass);
			windowclass.cbWndExtra=0;
			windowclass.hbrBackground=GetSysColorBrush(COLOR_MENU);
			windowclass.hCursor=LoadCursor(NULL,IDC_ARROW);
			windowclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
			windowclass.hIconSm=0;
			windowclass.hInstance=GetModuleHandle(0);							
			windowclass.lpfnWndProc=messageHandler;
			windowclass.lpszClassName="ion_Win32_defaultwindow_h1546814671";
			windowclass.lpszMenuName=0;
			windowclass.style=0;

			RegisterClassEx(&windowclass);

			++refcounter;
		}

		void unregisterClass()
		{
			if (refcounter<=0) return;
			--refcounter;

			if (refcounter==0) {
				UnregisterClass("ion_Win32_defaultwindow_h1546814671",GetModuleHandle(0));
			}
		}
	}

	Defaultwindow::Defaultwindow(const String& title,
		const ovis_uint32 width,const ovis_uint32 height,const bool fullscreen)
	{
		m_pInternaldata=new Internaldata;
		m_pInternaldata->m_Quit=false;
		m_pInternaldata->m_Fullscreen=fullscreen;
		m_pInternaldata->m_Width=width;
		m_pInternaldata->m_Height=height;
		m_pInternaldata->m_Activatestate=NoActivationChange;

		iondefaultwindowWin32::registerClass();

		unsigned int flags=WS_VISIBLE|(fullscreen ? WS_POPUP : (WS_CAPTION|WS_BORDER|WS_OVERLAPPED|WS_SYSMENU));

		RECT rectangle;
		rectangle.left = 0;
		rectangle.right = width - 1;
		rectangle.top = 0;
		rectangle.bottom = height - 1;
		if (!fullscreen) AdjustWindowRect(&rectangle, flags, false);


		const int windowWidth= rectangle.right - rectangle.left + 1;
		const int windowHeight = rectangle.bottom - rectangle.top + 1;
		const int windowX=fullscreen ? 0 : ((GetSystemMetrics(SM_CXSCREEN)-windowWidth)>>1);
		const int windowY=fullscreen ? 0 : ((GetSystemMetrics(SM_CYSCREEN)-windowHeight)>>1);

		m_pInternaldata->m_hWindow = CreateWindowEx(0,
			"ion_Win32_defaultwindow_h1546814671",
			title.cstr(), flags, windowX, windowY, windowWidth, windowHeight,
			0, 0, GetModuleHandle(0), 0);

	#ifdef _MSC_VER
	#pragma warning(disable:4244)
	#endif
		SetWindowLongPtr(m_pInternaldata->m_hWindow, GWL_USERDATA, (LONG_PTR)this);
	#ifdef _MSC_VER
	#pragma warning(default:4244)
	#endif
		
		ShowWindow(m_pInternaldata->m_hWindow,SW_SHOW);

	//	RegisterHotKey(m_hWindow, 0, MOD_ALT, VK_RETURN);   // Alt-enter for fullscreen
	}

	Defaultwindow::~Defaultwindow()
	{
		DestroyWindow(m_pInternaldata->m_hWindow);
		iondefaultwindowWin32::unregisterClass();
		delete m_pInternaldata;
	}

	Window::Activatestate Defaultwindow::activatestate() const { return m_pInternaldata->m_Activatestate; }


	void Defaultwindow::title(const String& newtitle)
	{
		SetWindowText(m_pInternaldata->m_hWindow,newtitle.cstr());
	}

	bool Defaultwindow::fullscreenMode() const { return m_pInternaldata->m_Fullscreen; }
	bool Defaultwindow::quit() const { return m_pInternaldata->m_Quit; }

	void* Defaultwindow::windowhandle() const { return (void*)(m_pInternaldata->m_hWindow); }
	void* Defaultwindow::displayhandle() const { return 0; }
	ovis_uint32 Defaultwindow::width() const { return m_pInternaldata->m_Width; }
	ovis_uint32 Defaultwindow::height() const { return m_pInternaldata->m_Height; }

	void Defaultwindow::resetEventstates()
	{
		m_pInternaldata->m_Activatestate=Window::NoActivationChange;
	}

	bool Defaultwindow::handleEvents()
	{
		MSG message;
		if(PeekMessage(&message, m_pInternaldata->m_hWindow, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
			
			return true;
		} else return false;
	}

}
}
