#ifndef OVIS_BASE_LOCALSTREAMABLE_HH_INCLUDED
#define OVIS_BASE_LOCALSTREAMABLE_HH_INCLUDED

#include "dll.hh"
#include "streamable.hh"

namespace ovis {
namespace base {

	//! Abstract class for a local streamable object.
	/**
	* A local streamable is similar to a streamable, except that its length is known,
	* allowing random-access seeks within the streamable.
	*/
	class OVIS_API LocalStreamable:public Streamable {
	public:
		//! Retrieves the size of the LocalStreamable.
		/** @return Size of the LocalStreamable, in bytes. */
		virtual unsigned long size() const=0;	
		//! Retrieves the current position in the streamable.
		/**
		* See seek() for more on this.
		* @return The current position, in bytes. 0 is the beginning of the file.
		*/
		virtual unsigned long tell() const=0;

		//! The mode seek() operates with.
		enum Seekmode {
			SeekAbsolute,	//! Absolute seeking; 0 always refers to the beginning of the file
			/**
			* Relative seeking, seeking happens relative to the current position,
			* so 0 means no seeking, -1 means one byte back, 58 means 58 byte forwards etc.
			*/
			SeekRelative,
			/**
			* Seeks backwards. 0 means the end of the streamble, -1 (NOT 1) means one byte
			* before the end of the streamable etc.
			*/
			SeekFromEnd
		};

		// Just like fseek() in the standard C library, this method is used to move the current position.
		/**
		* The current position specified where to read and write data. Both operations
		* move the current position forwards. With this method, it is possible to skip data.
		* @param Pos The new position. The actual meaning of this value depends on seekmode
		* @param seekmode The seeking mode. Refer to Seekmode for details on the modes
		*/
		virtual void seek(const long Pos,const Seekmode seekmode)=0;
	};

}
}

#endif
