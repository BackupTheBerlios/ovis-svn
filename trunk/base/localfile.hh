#ifndef OVIS_BASE_LOCALFILE_HH_INCLUDED
#define OVIS_BASE_LOCALFILE_HH_INCLUDED

#include "dll.hh"
#include "localstreamable.hh"
#include <stdio.h>
#include <string>

namespace ovis {
namespace base {

	class OVIS_API Localfile:public LocalStreamable
	{
	public:
		const std::string& name() const;

		unsigned long write(const void *Src,const unsigned long wsize);
		unsigned long read(void *Dest,const unsigned long rsize);

		bool eof() const;

		unsigned long size() const;
		unsigned long tell() const;

		void seek(const long Pos,const Seekmode seekmode);

		// Creates a temporary file. Name is "<temporary>". If this file is closed, it ceases to exist. It is
		// opened in "w+b" mode.
		bool openTemporaryFile();

		bool open(const std::string& filename,const std::string& openmode);
		void close();

		Localfile();
		~Localfile();
	protected:
		FILE* m_pFile;
		std::string *m_pName;
	};

}
}

#endif
