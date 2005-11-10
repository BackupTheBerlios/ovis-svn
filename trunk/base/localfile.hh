#ifndef OVIS_BASE_LOCALFILE_HH_INCLUDED
#define OVIS_BASE_LOCALFILE_HH_INCLUDED

#include "dll.hh"
#include "localstreamable.hh"
#include <stdio.h>
#include <string>

namespace ovis {
namespace base {

	//! LocalStreamable implementation for files in the local filesystem.
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

		//! Creates a temporary file.
		/**
		* The Streamable name is "<temporary>", the actual file name is not known
		* and internally managed by the file system.
		* If this file is closed, it ceases to exist. It is opened in "w+b" mode.
		* @return true if the file has been opened successfully, false otherwise.
		*/
		bool openTemporaryFile();

		//! Opens a file.
		/**
		* Behavior matches fopen() exactly, including the open modes.
		* For example, to open a file in binary mode, specify "rb" etc.
		* @note If a file is already opened, this method does nothing.
		*/
		
		// TODO: Match the binary/ascii specifier with the raw/ascii streamable mode.
		bool open(const std::string& filename,const std::string& openmode);
		//! Closes a file.
		/**
		* @note If there is no opened file, this method does nothing.
		*/
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
