#ifndef OVIS_BASE_DEFAULTWINDOW_HH_INCLUDED
#define OVIS_BASE_DEFAULTWINDOW_HH_INCLUDED

#include "window.hh"

namespace ovis {
namespace base {

	//! Default window implementation.
	/**
	* This implementation of the Window interface opens a regular window,
	* handles events, and manages the activation state. In 99% of all cases,
	* this implementation is used. (3D Editors need to implement their own Window
	* derivative).
	*/
	class OVIS_API Defaultwindow:public Window
	{
	public:
		//! Constructs the window.
		/**
		* @param title The window title
		* @param width The window width, in pixels
		* @param width The window height, in pixels
		* @param fullscreen Fullscreen attribute. If this is true, the window has
		* no borders and covers the entire screen.
		*/
		Defaultwindow(const String& title,
			const ovis_uint32 width,const ovis_uint32 height,const bool fullscreen);
		~Defaultwindow();
		
		void resetEventstates();
		bool handleEvents();

		Activatestate activatestate() const;

		void* windowhandle() const;
		void* displayhandle() const;
		ovis_uint32 width() const;
		ovis_uint32 height() const;

		void title(const String& newtitle);

		bool fullscreenMode() const;
		bool quit() const;

		struct Messagesysparams;
		long long handleSysMessages(const Messagesysparams& params);
	private:
		struct Internaldata;
		Internaldata *m_pInternaldata;
	};

}
}

#endif
