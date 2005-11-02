#ifndef OVIS_BASE_LOG_HH_INCLUDED
#define OVIS_BASE_LOG_HH_INCLUDED

#include "dll.hh"
#include "streamable.hh"
#include "stdoutstream.hh"

namespace ovis {
namespace base {

	enum Logtype
	{
		Message=0,
		MinorWarning=1,
		Warning=2,
		MajorWarning=3,
		Error=4,
		FatalError=5,
		CriticalError=6
	};

	OVIS_API const char *logtypedescription(const Logtype level);

	OVIS_API void logstream(Streamable& newstream);
	OVIS_API void nologstream();
	OVIS_API Streamable& logstream();

	OVIS_API Streamable& log(const char *where,const Logtype level);

}
}

#endif
