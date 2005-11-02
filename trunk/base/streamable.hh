#ifndef OVIS_BASE_STREAMABLE_HH_INCLUDED
#define OVIS_BASE_STREAMABLE_HH_INCLUDED

#include "dll.hh"
#include "itypes.hh"
#include <string>

namespace ovis {
namespace base {

	class String;

	//! Abstract class for a data stream object.
	/** 
	* Streamables are the primary data transfer mechanism in ovis. Streamable itself
	* is an abstract base class with a number of derivatives, like Datablock for data
	* in memory, Localfile for files on the local filesystem etc.
	*/
	class OVIS_API Streamable {
	public:
		//! Number handling mode.
		/**
		* This only affects number inputs through streams, and then only if raw mode
		* is disabled. See rawMode(), raw(), ascii() and isInRawMode() for more.
		*/
		enum NumMode {
			NumMode_Dez,	// Numbers are written in decimal format. (The default)
			NumMode_Hex,	// Numbers are written in hexadecimal format.
			NumMode_Oct,	// Numbers are written in octal format.
			NumMode_Bin		// Numbers are written in binary format.
		};

		//! Returns whether a nullbyte is written after a string.
		inline bool nullAfterString() const { return m_NullAfterString; }
		//! Specifies whether a nullbyte shall be written after a string.
		/** 
		* If state is false, only the string itself is written, not the trailing zero.
		* This is useful in some binary formats, but not wanted when writing ascii
		* data.
		* @param state true If the streamable shall write a null after a string, false otherwise.
		*/
		inline void nullAfterString(const bool state) { m_NullAfterString=state; }

		inline bool isInRawMode() const { return m_RawMode; }
		inline Streamable& rawMode(const bool rawmode) { m_RawMode=rawmode; return *this; }
		inline Streamable& raw() { m_RawMode=true; return *this; }
		inline Streamable& ascii() { m_RawMode=false; return *this; }

		inline NumMode numMode() const { return m_NumMode; }
		inline Streamable& numMode(NumMode nummode) { m_NumMode=nummode; return *this; }
		inline Streamable& dez() { m_NumMode=NumMode_Dez; return *this; }
		inline Streamable& hex() { m_NumMode=NumMode_Hex; return *this; }
		inline Streamable& oct() { m_NumMode=NumMode_Oct; return *this; }
		inline Streamable& bin() { m_NumMode=NumMode_Bin; return *this; }

		virtual ~Streamable();
			
		inline unsigned long streamCopyBuffersize() const { return m_StreamCopyBufsize; }
		void setStreamCopyBuffersize(const unsigned long newBuffersize);

		//const std::string& readStr();
		void readStr(String& str);
		void writeStr(const char *cstr);
		void writeStr(const std::string &str);
		void writeStr(const String &str);

		//! The precision of float->string conversions.
		/**
		* Indicates the amount of digits after the dot.
		* @param precision Number of digits after the dot.
		* Example: if this is 2, a number 48.17481 is truncated to 48.17.
		*/
		void decimalConvPrecision(int precision) { precision=m_DecPrecision; }
		int decimalConvPrecision() const { return m_DecPrecision; }
		
		
	/*	template <typename T> friend inline Streamable& operator >> (Streamable& stream,T& t)		{ stream.read(&t,sizeof(T));	return stream; }
		template <typename T> friend inline const T& operator >> (const T t,Streamable& stream)			{ stream.write(&t,sizeof(T));	return t; }
		template <typename T> friend inline Streamable& operator << (Streamable& stream,const T t)	{ t >> stream;					return stream; }
		template <typename T> friend inline T& operator << (T& t,Streamable& stream)				{ stream.read(&t,sizeof(T));	return t; }
	*/

		friend inline Streamable& operator >> (Streamable& stream,float& f)		{ stream.read(&f,sizeof(float));				return stream; }
		friend inline Streamable& operator >> (Streamable& stream,double& d)	{ stream.read(&d,sizeof(double));				return stream; }

		friend inline Streamable& operator >> (Streamable& stream,char& c)			{ stream.read(&c,sizeof(char));				return stream; }
		friend inline Streamable& operator >> (Streamable& stream,short& s)			{ stream.read(&s,sizeof(short));			return stream; }
		friend inline Streamable& operator >> (Streamable& stream,int& i)			{ stream.read(&i,sizeof(int));				return stream; }
		friend inline Streamable& operator >> (Streamable& stream,long& l)			{ stream.read(&l,sizeof(long));				return stream; }
		friend inline Streamable& operator >> (Streamable& stream,ovis_longlong& ll)	{ stream.read(&ll,sizeof(ovis_longlong));	return stream; }

		friend inline Streamable& operator >> (Streamable& stream,unsigned char& c)		{ stream.read(&c,sizeof(char));				return stream; }
		friend inline Streamable& operator >> (Streamable& stream,unsigned short& s)	{ stream.read(&s,sizeof(short));			return stream; }
		friend inline Streamable& operator >> (Streamable& stream,unsigned int& i)		{ stream.read(&i,sizeof(int));				return stream; }
		friend inline Streamable& operator >> (Streamable& stream,unsigned long& l)		{ stream.read(&l,sizeof(long));				return stream; }
		friend inline Streamable& operator >> (Streamable& stream,ovis_ulonglong& ll)	{ stream.read(&ll,sizeof(ovis_longlong));	return stream; }


		friend inline void operator >> (const char c,Streamable& stream) { stream.write(&c,sizeof(char)); }
		friend inline void operator >> (const unsigned char c,Streamable& stream) { stream.write(&c,sizeof(char)); }


		friend OVIS_API void operator >> (const float f,Streamable& stream);
		friend OVIS_API void operator >> (const double d,Streamable& stream);

		friend OVIS_API void operator >> (const short s,Streamable& stream);
		friend OVIS_API void operator >> (const int i,Streamable& stream);
		friend OVIS_API void operator >> (const long l,Streamable& stream);
		friend OVIS_API void operator >> (const ovis_longlong ll,Streamable& stream);

		friend OVIS_API void operator >> (const unsigned short s,Streamable& stream);
		friend OVIS_API void operator >> (const unsigned int i,Streamable& stream);
		friend OVIS_API void operator >> (const unsigned long l,Streamable& stream);
		friend OVIS_API void operator >> (const ovis_ulonglong ll,Streamable& stream);


		friend inline void operator << (float &f,Streamable& stream)	{ stream >> f; }
		friend inline void operator << (double &d,Streamable& stream)	{ stream >> d; }

		friend inline void operator << (short &s,Streamable& stream)			{ stream >> s; }
		friend inline void operator << (int &i,Streamable& stream)				{ stream >> i; }
		friend inline void operator << (long &l,Streamable& stream)				{ stream >> l; }
		friend inline void operator << (ovis_longlong &ll,Streamable& stream)	{ stream >> ll; }

		friend inline void operator << (unsigned short &s,Streamable& stream)	{ stream >> s; }
		friend inline void operator << (unsigned int &i,Streamable& stream)		{ stream >> i; }
		friend inline void operator << (unsigned long &l,Streamable& stream)	{ stream >> l; }
		friend inline void operator << (ovis_ulonglong &ll,Streamable& stream)	{ stream >> ll; }


		friend inline Streamable& operator << (Streamable& stream,const char c)	{ stream.write(&c,sizeof(char)); return stream; }
		friend inline Streamable& operator << (Streamable& stream,const unsigned char c)	{ stream.write(&c,sizeof(char)); return stream; }


		friend inline Streamable& operator << (Streamable& stream,const float f)	{ f >> stream; return stream; }
		friend inline Streamable& operator << (Streamable& stream,const double d)	{ d >> stream; return stream; }

		friend inline Streamable& operator << (Streamable& stream,const short s)			{ s >> stream; return stream; }
		friend inline Streamable& operator << (Streamable& stream,const int i)				{ i >> stream; return stream; }
		friend inline Streamable& operator << (Streamable& stream,const long l)				{ l >> stream; return stream; }
		friend inline Streamable& operator << (Streamable& stream,const ovis_longlong ll)	{ ll >> stream; return stream; }

		friend inline Streamable& operator << (Streamable& stream,const unsigned short s)	{ s >> stream; return stream; }
		friend inline Streamable& operator << (Streamable& stream,const unsigned int i)		{ i >> stream; return stream; }
		friend inline Streamable& operator << (Streamable& stream,const unsigned long l)	{ l >> stream; return stream; }
		friend inline Streamable& operator << (Streamable& stream,const ovis_ulonglong ll)	{ ll >> stream; return stream; }




		// Stream operators for strings 
		friend inline Streamable& operator << (Streamable& stream,const char *cstr)	{ stream.writeStr(cstr); return stream; }
		friend inline Streamable& operator << (Streamable& stream,const std::string &str)	{ stream.writeStr(str); return stream; }
		friend inline Streamable& operator << (Streamable& stream,const String &str)	{ stream.writeStr(str); return stream; }

		friend inline void operator >> (const char *cstr,Streamable& stream)	{ stream.writeStr(cstr); }
		friend inline void operator >> (const std::string &str,Streamable& stream)	{ stream.writeStr(str); }
		friend inline void operator >> (const String &str,Streamable& stream)	{ stream.writeStr(str); }

		friend inline void operator << (String &str,Streamable& stream)	{ stream.readStr(str); }
		friend inline Streamable& operator >> (Streamable& stream,String &str)	{ stream.readStr(str); return stream; }
		// Stream operators for strings 


		void transferFrom(Streamable& src);
		
		inline friend Streamable& operator >> (Streamable &src,Streamable &dest) { dest.transferFrom(src); return src; }
		inline friend Streamable& operator << (Streamable &dest,Streamable &src) { dest.transferFrom(src); return dest; }




		// Pure virtual methods

		virtual const std::string& name() const=0;

		virtual unsigned long write(const void *Src,const unsigned long wsize)=0;
		virtual unsigned long read(void *Dest,const unsigned long rsize)=0;

		virtual bool eof() const=0;


	protected:
		Streamable();	

		unsigned long m_StreamCopyBufsize;
		unsigned char* m_pStreamCopyBuffer;

		bool m_RawMode,m_NullAfterString;
		NumMode m_NumMode;
		int m_DecPrecision;

		//std::string* m_pBufstring;
	};

}
}

#endif
