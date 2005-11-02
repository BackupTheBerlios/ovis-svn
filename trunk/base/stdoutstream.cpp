#include "stdoutstream.hh"

namespace ovis {
namespace base {

	Stdoutstream& Stdoutstream::stream()
	{
		static Stdoutstream stdoutstream;
		return stdoutstream;
	}

	Stdoutstream::Stdoutstream()
	{
		ascii();
	}

	Stdoutstream::~Stdoutstream()
	{
	}

	const std::string& Stdoutstream::name() const
	{
		static std::string stdoutstreamname("Stdoutstream");
		return stdoutstreamname;
	}

	unsigned long Stdoutstream::write(const void *Src,const unsigned long wsize)
	{
		unsigned long ret=wsize;
		char *csrc=(char*)Src;
		while (ret--)
			if ((*csrc>=' ') || (*csrc=='\t') || (*csrc=='\r') || (*csrc=='\n'))
				fwrite((csrc++),1,1,stdout);
		fflush(stdout);

		return wsize;
	}

	unsigned long Stdoutstream::read(void *Dest,const unsigned long rsize)
	{
		return 0;
	}

	bool Stdoutstream::eof() const { return false; }

}
}
