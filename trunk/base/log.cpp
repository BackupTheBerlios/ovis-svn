#include "log.hh"
#include "nullstream.hh"

namespace ovis {
namespace base {

	Nullstream g_Nullstream;
	Streamable *g_pLogstream=&g_Nullstream;

	const char *logtypedescription(const Logtype level)
	{
		switch (level) {
			case Message:return "Message";
			case MinorWarning:return "Minor warning";
			case Warning:return "Warning";
			case MajorWarning:return "Major warning";
			case Error:return "ERROR";
			case FatalError:return "FATAL ERROR";
			case CriticalError:return "CAUTION: CRITICAL ERROR";
			default:return "<Unknown>";
		}
	}

	void logstream(Streamable& newstream)
	{
		g_pLogstream=&newstream;
	}
	
	void nologstream()
	{
		g_pLogstream=&g_Nullstream;
	}

	Streamable& logstream()
	{
		return *g_pLogstream;
	}
	
	Streamable& log(const char *where,const Logtype level)
	{
		Streamable& os=logstream();
		os << logtypedescription(level) << " : " << where << " : ";
		return os;
	}

}
}
