#ifndef OVIS_BASE_LOG_HH_INCLUDED
#define OVIS_BASE_LOG_HH_INCLUDED

#include "dll.hh"
#include "streamable.hh"
#include "stdoutstream.hh"

namespace ovis {
namespace base {

	//! Log message type.
	enum Logtype
	{
		Message=0,				//! Useful for informing about something that is not an error or a warning
		Warning=2,				//! A warning; operation can continue, but something unusual happened
		Error=3,				//! Something caused an error, operation should abort by now
		FatalError=4			//! Fatal error, for example when crucial data is not found, or data has been corrupted
	};

	//! Returns a string with a brief description of the error.
	/**
	* @param level The log level type
	*/
	OVIS_API const char *logtypedescription(const Logtype level);

	//! Sets a streamable as the log output stream.
	/**
	* Log messages are output using this stream. If no streamable shall be used,
	* call nologstream().
	* @param newstream Reference to the streamable to be used.
	*/
	OVIS_API void logstream(Streamable& newstream);
	//! Causes all logging to be ignored.
	/**
	* Internally, the current logstream is set to a nullstream, which simply dumps
	* any data it receives, thus after a nologstream() logging can still be performed,
	* its just ignored.
	*/
	OVIS_API void nologstream();
	//! Returns the current log streamable.
	/**
	* @note If no logstream is used, this returns a reference to the internal nullstream
	* @return The streamable used for logging.
	*/
	OVIS_API Streamable& logstream();

	//! Logs and adds a note about the log level.
	/**
	* Output looks like "<loglevel> : <where> : <additional output>".
	* Example:
	* @code
	* log("main()",base::Message) << "Program terminated\n";
	* @endcode
	*
	* @param where Location in the code where the log happens. An example of a good location specifier is "base::Streamable::Streamable()"
	* @param level Log level
	*
	* @return The streamable used for logging. This returnvalue equals the return value of logstream().
	*/
	OVIS_API Streamable& log(const char *where,const Logtype level);

}
}

#endif
