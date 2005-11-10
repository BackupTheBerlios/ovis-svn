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
	*
	* Streamables provide stream operators for data transfer. It is possible to write
	* stream << "Number 1: " << 571 << "\n"; for example. To enhance this, several
	* methods and all stream operators return a reference to the streamable, making this
	* possible: stream << "Test: " << stream.hex() << 223 << " " << stream.dez() << 19 << "\n";
	* In addition, number translations are available only when they are transferred with
	* stream operators. In other words: to make sure a 223 is written as "223" in the file,
	* and not as one byte containing the value 223, one has to switch on ascii mode (see
	* rawMode() for more), dez mode (see numMode() for mode), and transfer it using stream
	* operators.
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

		//! Returns true if the stream is in raw mode.
		/**
		* Look at rawMode() for general information about raw/ascii mode.
		* @return true if the streamable is in raw mode, false if it is in ascii mode.
		*/
		inline bool isInRawMode() const { return m_RawMode; }
		//! Enables/disables raw mode.
		/**
		* A streamable can have two data encoding modes, raw or ascii. The difference lies
		* in the way special codes are treated. In ascii mode, codes like line endings may
		* be transformed, in raw mode, the data is not changed. In the standard C library's
		* fopen() function raw mode is referred to as "binary" mode (the "b" specifier).
		* @param rawmode Enables raw mode if the value is true, and the ascii mode if it is false
		* @return Reference to this streamable. Useful for concatenating stream operations.
		*/
		inline Streamable& rawMode(const bool rawmode) { m_RawMode=rawmode; return *this; }
		//! Enables raw mode.
		/**
		* Look at rawMode() for general information about raw/ascii mode.
		* This method is equivalent to rawMode(true).
		* @return true if the streamable is in raw mode, false if it is in ascii mode.
		*/
		inline Streamable& raw() { m_RawMode=true; return *this; }
		//! Enables ascii mode.
		/**
		* Look at rawMode() for general information about raw/ascii mode.
		* This method is equivalent to rawMode(false).
		* @return true if the streamable is in raw mode, false if it is in ascii mode.
		*/
		inline Streamable& ascii() { m_RawMode=false; return *this; }

		//! Returns the currently used num mode.
		/**
		* Have a look at numMode(NumMode nummode) for details.
		* @return The num mode currently used.
		*/
		inline NumMode numMode() const { return m_NumMode; }
		//! Sets the num mode.
		/**
		* The num mode specifies how numerical data is to be treated.
		* There are several modes: decimal (dez), hexadecimal (hex), octal (oct),
		* binary (bin) (do not confuse the binary num mode with raw mode, which is often
		* referred to as "binary mode"). Numbers are written with the specified base, for
		* example, if hexadecimal mode is used, and 58261 is passed through a stream
		* operator to the streamable (e.g. streamable << 58261), then "e395" is written.
		* Note that the num mode is only relevant in ascii mode, and then only when
		* numbers are passed through the stream operators. If a number in a variable
		* is written with the write() method, the variable contents are always written
		* in raw mode (that is, the variable's bytes are written).
		* @param nummode The numerical mode to be used.
		* @return Reference to this streamable. Useful for concatenating stream operations.
		*/
		inline Streamable& numMode(NumMode nummode) { m_NumMode=nummode; return *this; }
		//! Enables decimal num mode.
		/**
		* This is equivalent to numMode(NumMode_Dez).
		* @return Reference to this streamable. Useful for concatenating stream operations.
		*/
		inline Streamable& dez() { m_NumMode=NumMode_Dez; return *this; }
		//! Enables hexadecimal num mode.
		/**
		* This is equivalent to numMode(NumMode_Hex).
		* @return Reference to this streamable. Useful for concatenating stream operations.
		*/
		inline Streamable& hex() { m_NumMode=NumMode_Hex; return *this; }
		//! Enables octal num mode.
		/**
		* This is equivalent to numMode(NumMode_Oct).
		* @return Reference to this streamable. Useful for concatenating stream operations.
		*/
		inline Streamable& oct() { m_NumMode=NumMode_Oct; return *this; }
		//! Enables binary num mode.
		/**
		* This is equivalent to numMode(NumMode_Bin).
		* @return Reference to this streamable. Useful for concatenating stream operations.
		*/
		inline Streamable& bin() { m_NumMode=NumMode_Bin; return *this; }

		//! Cleans up internal stream data.
		virtual ~Streamable();

		//! Returns the copy buffer size.
		/*
		* Have a look at setStreamCopyBuffersize(const unsigned long newBuffersize)
		* for details.
		* @return The copy buffer size, in bytes.
		*/
		inline unsigned long streamCopyBuffersize() const { return m_StreamCopyBufsize; }
		
		//! Sets the copy buffer size.
		/**
		* The stream copy buffer is necessary for stream-to-stream copies.
		* Since a Streamable has an undetermined length, it is not possible to
		* write all of its contents in one turn. Thus, chunks are read into the copy buffer
		* and written from that buffer to the destination stream until there is no more
		* data available to copy. This method sets the size of the copy buffer, in bytes.
		* Default size is 32 KByte.
		* @param newBuffersize New size of the copy buffer, in bytes.
		*/
		void setStreamCopyBuffersize(const unsigned long newBuffersize);

		//! Reads a string from the streamable and stores it into str.
		/**
		* This method reads a null-terminated string and stores it into
		* the specified String object. No std::string is used here to
		* prevent DLL boundary problems. The string size is adjusted to
		* the size of the string that has been read.
		* @param str The String object where the string is to be stored.
		* @note This method is rarely used directly. Instead, it is common
		* to use the stream operators for this (e.g. stream >> str).
		*/
		void readStr(String& str);
		//! Writes a C string into the streamable.
		/**
		* Depending on the nullAfterString() state, this may write a zero
		* after the string contents.
		* @param cstr Pointer to the C-string.
		* @note This method is rarely used directly. Instead, it is common
		* to use the stream operators for this (e.g. stream << str).
		*/
		void writeStr(const char *cstr);
		//! Writes an STL string into the streamable.
		/**
		* Depending on the nullAfterString() state, this may write a zero
		* after the string contents.
		* @param str Reference to the STL string.
		* @note This method is rarely used directly. Instead, it is common
		* to use the stream operators for this (e.g. stream << str).
		*/
		void writeStr(const std::string &str);
		//! Writes a base::String into the streamable.
		/**
		* Depending on the nullAfterString() state, this may write a zero
		* after the string contents.
		* @param str Reference to the base::String.
		* @note This method is rarely used directly. Instead, it is common
		* to use the stream operators for this (e.g. stream << str).
		*/
		void writeStr(const String &str);

		//! Sets the precision of float->string conversions.
		/**
		* Indicates the amount of digits after the dot.
		* @param precision Number of digits after the dot.
		* Example: if this is 2, a number 48.17481 is truncated to 48.17.
		*/
		void decimalConvPrecision(int precision) { precision=m_DecPrecision; }
		
		//! Returns the precision of float->string conversions.
		/**
		* Refer to decimalConvPrecision(int precision) for more details.
		* @return The decimal precision.
		*/
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


		//! Transfers the contents of the src streamable into this streamable.
		/**
		* This adds the contents of src into this streamable, and does not clone src,
		* thus it is possible that this streamable already has content, and src's contents
		* are appended.
		* @note The stream copy buffer is used for the transfer (see setStreamCopyBuffersize() for more).
		* @param src The streamable whose contents are to be copied to this stream
		*/
		void transferFrom(Streamable& src);
		
		inline friend Streamable& operator >> (Streamable &src,Streamable &dest) { dest.transferFrom(src); return src; }
		inline friend Streamable& operator << (Streamable &dest,Streamable &src) { dest.transferFrom(src); return dest; }




		// Pure virtual methods

		//! Streamable name.
		/**
		* A "name" of a stream can have very different meanings, depending on
		* on the streamable type. The Localfile class uses this for the file name,
		* for example. Either way, it should identify the stream reliably.
		* @return The name of the streamable.
		*/
		virtual const std::string& name() const=0;

		//! Writes data into the stream.
		/**
		* @param Src Pointer to the data to be written
		* @param wsize Write size specifying the amount of bytes to be written
		*
		* @note The numerical mode (see numMode() for this) has zero effects on this method,
		* but calling this while the stream is in ascii mode (see rawMode() for this)
		* causes undefined behavior.
		*/
		virtual unsigned long write(const void *Src,const unsigned long wsize)=0;
		//! Reads data from the stream.
		/**
		* @param Dest Pointer to a memory block where the read data is to be transferred
		* @param rsize Read size specifying the amount of bytes to be read
		*
		* @note The numerical mode (see numMode() for this) has zero effects on this method,
		* but calling this while the stream is in ascii mode (see rawMode() for this)
		* causes undefined behavior.
		*/
		virtual unsigned long read(void *Dest,const unsigned long rsize)=0;

		//! Specifies whether or not the end of the streamable has been reached.
		/**
		* This method is called eof() for historical purposes. Logically, it should
		* be called eos(), but eof is instantly recognizable.
		* @note This is always true if the stream has no defined end.
		* @return true if the end of the streamable is reached, false otherwise.
		*/
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
