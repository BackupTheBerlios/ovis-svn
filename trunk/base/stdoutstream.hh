#ifndef OVIS_BASE_STDOUTSTREAM_HH_INCLUDED
#define OVIS_BASE_STDOUTSTREAM_HH_INCLUDED

#include "dll.hh"
#include "localstreamable.hh"

namespace ovis {
namespace base {

	class OVIS_API Stdoutstream:public Streamable
	{
	public:
		Stdoutstream();
		~Stdoutstream();

		const std::string& name() const;

		unsigned long write(const void *Src,const unsigned long wsize);
		unsigned long read(void *Dest,const unsigned long rsize);

		bool eof() const;

		static Stdoutstream& stream();
	};

}
}

#endif
