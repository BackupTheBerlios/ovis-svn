#ifndef OVIS_BASE_WINDOW_HH_INCLUDED
#define OVIS_BASE_WINDOW_HH_INCLUDED

#include "dll.hh"
#include "itypes.hh"

namespace ovis {
namespace base {

	class String;

	//! Base window interface.
	/**
	* A window is used for graphics display, input- and event handling.
	*/
	class OVIS_API Window
	{
	protected:
		Window() {}
	public:
		virtual ~Window() {}

		//! Window activation state.
		enum Activatestate {
			Deactivated,		//!< Window has been deactivated since the last event update
			Activated,			//!< Window has been (re)activated since the last event update
			NoActivationChange	//!< No activation change since the last event update happened
		};

		//! Returns the window activation state.
		/** @return The activation state */
		virtual Activatestate activatestate() const=0;

		//! Resets internal event states.
		/**
		* This is commonly used to reset the activation state, which is needed for
		* graphics device handling and input device reacquiring.
		*/
		virtual void resetEventstates()=0;
		//! Handles the internal event queue and updates the current states.
		/**
		* Internally, this calls a system-dependent message pump.
		* For example, the activation state is updated here. Call this every frame.
		* @return true If no error occurred and a quit message hasn't been posted
		* (like when the window gets closed); false otherwise.
		*/
		virtual bool handleEvents()=0;

		//! Sets the window title
		/** 
		* The window title is usually displayed in the caption of the window border.
		* Some window managers in X11 might handle this differently though.
		* @param newtitle New title for the window
		*/
		virtual void title(const String& newtitle)=0;

		//! Returns the fullscreen attribute.
		/**
		* The fullscreen attribute specifies if the window should be used in a
		* fullscreen environment or not. This attribute is set in the Window derivatives
		* like Defaultwindow (where it can be specified in the constructor).
		* When initializing a video device this attribute determines if the device
		* initializes the fullscreen mode or not.
		* @return The fullscreen attribute
		*/
		virtual bool fullscreenMode() const=0;
		//! Returns whether a quit message has been posted or not.
		/** 
		* A quit message is posted internally, for example when the window is closed.
		* If this is true, the next handleEvents() call will return false.
		* @return true if a quit message has been posted, false otherwise.
		*/
		virtual bool quit() const=0;

		//! Returns the system-specific window handle.
		/**
		* This is a HWND handle in windows, and a Window handle in X11.
		* Usually, this getter isn't used in external code.
		* @return The system-specific window handle
		*/ 
		virtual void* windowhandle() const=0;
		virtual void* displayhandle() const=0;
		//! Returns the window width, in pixels.
		virtual ovis_uint32 width() const=0;
		//! Returns the window height, in pixels.
		virtual ovis_uint32 height() const=0;
	};

}
}

#endif
