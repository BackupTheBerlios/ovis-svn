#ifndef OVIS_BASE_LOCALSTREAMABLE_HH_INCLUDED
#define OVIS_BASE_LOCALSTREAMABLE_HH_INCLUDED

#include "dll.hh"
#include "streamable.hh"

namespace ovis {
namespace base {

//! Abstract class for a local data stream object.
class OVIS_API LocalStreamable:public Streamable {
public:
	virtual unsigned long size() const=0;	
	virtual unsigned long tell() const=0;

	enum Seekmode {
		SeekAbsolute,
		SeekRelative,
		SeekFromEnd
	};

	// Just like fseek(), this method is used to move the current position.
	// "origin" can handle the three constants SeekAbsolute,SeekRelative,SeekFromEnd.
	virtual void seek(const long Pos,const Seekmode seekmode)=0;
};

}
}

#endif
