#include "datablock.hh"
#include <memory.h>
#include <cstdio>
#include <cstdlib>
#include <sstream>

namespace ovis {
namespace base {

	Datablock::Datablock():m_pData(0),m_Size(0),m_ActualSize(0),m_Pos(0),m_BitBuffer(0),
	m_BitCount(0)
	{
	}

	Datablock::Datablock(const Datablock& rDatablockFrom):m_pData(0),m_Size(0),m_ActualSize(0),m_Pos(0),
	m_BitBuffer(0),m_BitCount(0)
	{
		clone(rDatablockFrom);
	}

	Datablock::~Datablock()
	{
		if (m_pData) free(m_pData);
	}

	const std::string& Datablock::name() const
	{	
		static std::string dblockname("Datablock");
		return dblockname;
	}

	void Datablock::resize(const unsigned long newsize)
	{
		if (newsize==0) {
			if (m_pData) free(m_pData);
			m_pData=0;
			m_Size=0;
			m_ActualSize=0;
			m_Pos=0;
			return;
		}
		
		if (m_Pos>=newsize) m_Pos=newsize-1;
		m_Size=newsize;
		unsigned long oasize=m_ActualSize;
		//m_ActualSize=(newsize/20000)*20000+((newsize%20000) ? 20000 : 0);#
		m_ActualSize=(newsize&(~32767))+((newsize&32767) ? 32768 : 0);
		if (oasize==m_ActualSize) return;

		if (m_pData) m_pData=(ovis_uint8 *)realloc(m_pData,m_ActualSize);
		else m_pData=(ovis_uint8 *)malloc(m_ActualSize);
	}

	ovis_uint8* Datablock::expand(const unsigned long addsize)
	{
		unsigned long newpos=m_Size;
		resize(m_Size+addsize);
		return m_pData+newpos;
	}

	void Datablock::seek(const long Pos,const Seekmode seekmode)
	{
		long pos=m_Pos;

		switch (seekmode) {
		case SeekAbsolute: pos=Pos; break;
		case SeekRelative: pos+=Pos; break;
		case SeekFromEnd: pos=m_Size+Pos; break;
		}

		if (pos<0) pos=0;
		if (pos>(long)m_Size) pos=(long)m_Size;

		m_Pos=(unsigned long)pos;
	}

	unsigned long Datablock::read(void *dest, const unsigned long rsize)
	{
		if (eof()) return 0;

		unsigned long _size=rsize;
		if (m_Pos+_size>m_Size) _size=m_Size-m_Pos;

		memcpy(dest,m_pData+m_Pos,_size);
		m_Pos+=_size;

		return _size;
	}

	unsigned long Datablock::write(const void *src, const unsigned long wsize)
	{
		unsigned long _size=wsize+m_Pos;

		if (_size>m_Size) expand(_size-m_Size);
		memcpy(m_pData+m_Pos,src,wsize);
		m_Pos+=wsize;

		return wsize;
	}

	unsigned long Datablock::hash() const
	{
		unsigned long h=0;
		ovis_uint8 *pByte=m_pData;
		ovis_uint8 *pByteEnd=m_pData+m_Size;
		for (;pByte!=pByteEnd;++pByte) h=(h<<5)-h+*pByte;

		return h;
	}

	void Datablock::copyTo(Datablock &rDatablock) const
	{
		rDatablock.write(m_pData,m_Size);
	}

	void Datablock::clone(const Datablock &rDatablockFrom)
	{
		resize(rDatablockFrom.size());
		memcpy(m_pData,(const void*)rDatablockFrom.data(),m_Size);
		m_Pos=rDatablockFrom.m_Pos;
		m_BitBuffer=rDatablockFrom.m_BitBuffer;
		m_BitCount=rDatablockFrom.m_BitCount;
	}

	void Datablock::addBit(const bool bit)
	{
		m_BitBuffer=(m_BitBuffer<<1)|(bit ? 1 : 0);
		m_BitCount++;

		if(m_BitCount==8)
		{
			write(&m_BitBuffer,1);
			m_BitCount=0;
		}
	}

	bool Datablock::getBit()
	{
		if(m_BitCount == 0) // Puffer leer
		{
			if( !eof() ) { // Datei komplett eingelesen?
				read(&m_BitBuffer,1);
			}
			else
				m_BitBuffer = 0; // Nullen anhängen

			m_BitCount = 8;
		}

		// Bit aus Puffer extrahieren
		bool bit = (m_BitBuffer >> 7)!=0;
		m_BitBuffer <<= 1;
		m_BitCount--;

		return bit;

	}

	unsigned long Datablock::getBits(const unsigned long size)
	{
		unsigned long val=0;

		for (int i=size-1;i>=0;--i) {
			val|=(getBit() ? 1 : 0) << i;
		}

		return val;
	}

	void Datablock::addBits(const unsigned long bits, const unsigned short size)
	{
		for (int i=size-1;i>=0;--i) { addBit( (bits & (1<<i))!=0 ); }
	}

	/*std::ostream& operator << (std::ostream& stream,const Datablock& datablock)
	{
		if (!stream.good()) return stream;
		stream.write((char*)(datablock.data()),sizeof(char)*datablock.size());
		return stream;
	}

	std::istream& operator >> (std::istream& stream,Datablock& datablock)
	{
		if (!stream.good()) return stream;
		
		while (!stream.eof() && !stream.bad() && !stream.fail()) {
			const std::streamsize bufsize=65*1024;
			ovis_uint8 *p=datablock.expand(bufsize);
			std::streamsize actuallyread=stream.readsome((char*)p,bufsize);
		
			if (actuallyread<bufsize) {
				datablock.contract(bufsize-actuallyread);
				break;
			}
		}

		return stream;
	}*/



}
}
