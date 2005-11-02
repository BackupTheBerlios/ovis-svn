#ifndef OVIS_BASE_NULLSTREAM_HH_INCLUDED
#define OVIS_BASE_NULLSTREAM_HH_INCLUDED

#include "dll.hh"
#include "localstreamable.hh"

namespace ovis {
namespace base {

	/**
	This stream does .... nothing! :)
	read() commands always return 0, write() commands always return the write amount
	(without actually writing anything). Data sent to this stream vanishes in hyperspace.
	*/
	class OVIS_API Nullstream:public LocalStreamable {
	public:
		const std::string& name() const;
		
		inline unsigned long size() const { return 0; }
		inline unsigned long tell() const { return 0; }
		
		inline void seek(const long Pos,const Seekmode seekmode) {}
		
		inline bool eof() const { return false; }
		
		inline unsigned long write(const void *Src,const unsigned long wsize) { return wsize; }
		inline unsigned long read(void *Dest,const unsigned long rsize) { return 0; }
		
		// To prevent unnecessary streamcopybuffer usage
		friend inline Nullstream& operator >> (Nullstream& nullstr,Streamable& stream)	{ return nullstr; }
		friend inline Streamable& operator >> (Streamable& stream,Nullstream& nullstr)	{ return stream; }
		friend inline Nullstream& operator << (Nullstream& nullstr,Streamable& stream)	{ return nullstr; }
		friend inline Streamable& operator << (Streamable& stream,Nullstream& nullstr)	{ return stream; }
	};

}
}

#endif
