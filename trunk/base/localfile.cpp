#include "localfile.hh"
#include <stdio.h>

namespace ovis {
namespace base {

	const std::string& Localfile::name() const
	{
		return *m_pName;
	}

	unsigned long Localfile::write(const void *Src,const unsigned long wsize)
	{
		//m_pFStream->write((char*)Src,(std::streamsize)wsize);
		return (unsigned long)fwrite(Src,1,wsize,m_pFile);
	}

	unsigned long Localfile::read(void *Dest,const unsigned long rsize)
	{
		return (unsigned long)fread(Dest,1,rsize,m_pFile);
	}


	bool Localfile::eof() const
	{
		return tell()>=size();
	}


	unsigned long Localfile::size() const
	{
		long pos=ftell(m_pFile);
		fseek(m_pFile,0,SeekFromEnd);
		unsigned long _size=(unsigned long)ftell(m_pFile);
		fseek(m_pFile,pos,SeekAbsolute);

		return _size;
	}

	unsigned long Localfile::tell() const
	{
		return (unsigned long)ftell(m_pFile);
	}


	void Localfile::seek(const long Pos,const Seekmode seekmode)
	{
		int fseekorigin;
		switch (seekmode) {
			case SeekAbsolute:fseekorigin=SEEK_SET; break;
			case SeekRelative:fseekorigin=SEEK_CUR; break;
			case SeekFromEnd:fseekorigin=SEEK_END; break;
			default:fseekorigin=SEEK_SET;
		}
		fseek(m_pFile,Pos,fseekorigin);
	}

	bool Localfile::openTemporaryFile()
	{
		if (m_pFile) { fclose(m_pFile); m_pFile=0; }
		(*m_pName)="<temporary>";
		m_pFile=tmpfile();
		if (!m_pFile) { return false; }
		
		return true;
	}

	bool Localfile::open(const std::string& filename,const std::string& openmode)
	{
		if (m_pFile) { fclose(m_pFile); m_pFile=0; }
		(*m_pName)="";
		m_pFile=fopen(filename.c_str(),openmode.c_str());
		if (!m_pFile) { return false; }
		(*m_pName)=filename;
		return true;
	}

	void Localfile::close()
	{
		if (!m_pFile) return;

		(*m_pName)="";
		fclose(m_pFile);
		m_pFile=0;
	}

	Localfile::Localfile():m_pFile(0),m_pName(new std::string) {}
	Localfile::~Localfile() { close(); delete m_pName; }

}
}
