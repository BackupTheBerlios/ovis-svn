#include <string.h>
#include <sstream>
#include "streamable.hh"
#include "string.hh"


namespace ovis {
namespace base {

	Streamable::Streamable():m_StreamCopyBufsize(32768),m_pStreamCopyBuffer(0),
	m_DecPrecision(5)/*,m_pBufstring(new std::string)*/ {
		raw();
		nullAfterString(true);
		numMode(NumMode_Dez);
		setStreamCopyBuffersize(m_StreamCopyBufsize);
	}

	Streamable::~Streamable()
	{
		if (m_pStreamCopyBuffer) delete [] m_pStreamCopyBuffer;
		//delete m_pBufstring;
	}









void Streamable::setStreamCopyBuffersize(const unsigned long newBuffersize)
{
	//if (m_pStreamCopyBuffer) delete [] m_pStreamCopyBuffer;
	
	m_StreamCopyBufsize=newBuffersize;
	//m_pStreamCopyBuffer=new unsigned char[m_StreamCopyBufsize];
}

void Streamable::readStr(String& str)
{
	std::string tmp;
	char c=0;
	do {
		c=0;
		(*this) >> c;
		if (!c) break;
		tmp+=c;
	} while (!eof());

	str.set(tmp);
}

void Streamable::writeStr(const char *cstr)
{
	unsigned long len=static_cast<unsigned long>(strlen(cstr));
	if (nullAfterString()) ++len;
	write((void*)cstr,len);
}

void Streamable::writeStr(const std::string &str)
{
	unsigned long len=static_cast<unsigned long>(str.length());
	if (nullAfterString()) ++len;
	write((void*)str.c_str(),len);
}

void Streamable::writeStr(const String &str)
{
	unsigned long len=static_cast<unsigned long>(str.length());
	if (nullAfterString()) ++len;
	write((void*)str.cstr(),len);
}






//Streamable& Streamable::operator >> (Streamable &streamable)
/*Streamable& operator >> (Streamable& src,Streamable &dest)
{
	src.m_pStreamCopyBuffer=new unsigned char[src.m_StreamCopyBufsize];

	while (1) {
		if (src.endOfStream()) break;
		
		unsigned long amountread=src.read(src.m_pStreamCopyBuffer,src.m_StreamCopyBufsize);
		if (amountread==0) break;
		dest.write(src.m_pStreamCopyBuffer,amountread);
		if (amountread<src.m_StreamCopyBufsize) break;
	}
	
	delete [] src.m_pStreamCopyBuffer; src.m_pStreamCopyBuffer=0;

	return src;
}



Streamable& operator << (Streamable& dest,Streamable &src)
{
	dest.m_pStreamCopyBuffer=new unsigned char[dest.m_StreamCopyBufsize];

	while (1) {
		if (src.endOfStream()) break;
		
		unsigned long amountread=src.read(dest.m_pStreamCopyBuffer,dest.m_StreamCopyBufsize);
		if (amountread==0) break;
		dest.write(dest.m_pStreamCopyBuffer,amountread);
		if (amountread<dest.m_StreamCopyBufsize) break;
	}

	delete [] dest.m_pStreamCopyBuffer; dest.m_pStreamCopyBuffer=0;

	return dest;
}*/


void Streamable::transferFrom(Streamable& src)
{
	m_pStreamCopyBuffer=new unsigned char[m_StreamCopyBufsize];

	while (1) {
		if (src.eof()) break;
		
		unsigned long amountread=src.read(m_pStreamCopyBuffer,m_StreamCopyBufsize);
		if (amountread==0) break;
		write(m_pStreamCopyBuffer,amountread);
		if (amountread<m_StreamCopyBufsize) break;
	}

	delete [] m_pStreamCopyBuffer; m_pStreamCopyBuffer=0;
}



/*Streamable& operator << (Streamable& stream,const float f)
{
	if (stream.isInRawMode()) stream.write(&f,sizeof(float));
	else {
		String str(f,stream.decimalConvPrecision());
		stream.writeStr(str);
	}
	return stream;
}
Streamable& operator << (Streamable& stream,const double d)
{
	if (stream.isInRawMode()) stream.write(&d,sizeof(double));
	else {
		String str(d,stream.decimalConvPrecision());
		stream.writeStr(str);
	}
	return stream;
}*/







void operator >> (const float f,Streamable& stream)
{
	if (stream.isInRawMode()) stream.write(&f,sizeof(float));
	else {
		std::stringstream str/*(f,stream.decimalConvPrecision())*/;
		str << f;
		stream.writeStr(str.str());
	}
}

void operator >> (const double d,Streamable& stream)
{
	if (stream.isInRawMode()) stream.write(&d,sizeof(double));
	else {
		std::stringstream str/*(d,stream.decimalConvPrecision())*/;
		str << d;
		stream.writeStr(str.str());
	}
}

void operator >> (const short s,Streamable& stream)
{
	if (stream.isInRawMode()) stream.write(&s,sizeof(short));
	else {
		std::stringstream str; str << s;
		stream.writeStr(str.str());
	}
}

void operator >> (const int i,Streamable& stream)
{
	if (stream.isInRawMode()) stream.write(&i,sizeof(int));
	else {
		std::stringstream str; str << i;
		stream.writeStr(str.str());
	}
}

void operator >> (const long l,Streamable& stream)
{
	if (stream.isInRawMode()) stream.write(&l,sizeof(long));
	else {
		std::stringstream str; str << l;
		stream.writeStr(str.str());
	}
}

void operator >> (const ovis_longlong ll,Streamable& stream)
{
	if (stream.isInRawMode()) stream.write(&ll,sizeof(ovis_longlong));
	else {
		std::stringstream str; str << ll;
		stream.writeStr(str.str());
	}
}

void operator >> (const unsigned short s,Streamable& stream)
{
	if (stream.isInRawMode()) stream.write(&s,sizeof(unsigned short));
	else {
		std::stringstream str; str << s;
		stream.writeStr(str.str());
	}
}

void operator >> (const unsigned int i,Streamable& stream)
{
	if (stream.isInRawMode()) stream.write(&i,sizeof(unsigned int));
	else {
		std::stringstream str; str << i;
		stream.writeStr(str.str());
	}
}

void operator >> (const unsigned long l,Streamable& stream)
{
	if (stream.isInRawMode()) stream.write(&l,sizeof(unsigned long));
	else {
		std::stringstream str; str << l;
		stream.writeStr(str.str());
	}
}

void operator >> (const ovis_ulonglong ll,Streamable& stream)
{
	if (stream.isInRawMode()) stream.write(&ll,sizeof(ovis_ulonglong));
	else {
		std::stringstream str; str << ll;
		stream.writeStr(str.str());
	}
}

}
}
