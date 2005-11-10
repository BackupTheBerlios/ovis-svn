#ifndef OVIS_BASE_STRING_HH_INCLUDED
#define OVIS_BASE_STRING_HH_INCLUDED

#include <string>
#include "basememobj.hh"
#include "dll.hh"

namespace ovis {
namespace base {

	//! STL string wrapper class.
	/**
	* This class is necessary for ensuring safe handling of strings across DLLs.
	*/
	class OVIS_API String:public BaseMemObj
	{
	public:
		//! Constructor creating an empty (but valid) string.
		String();
		//! Constructor creating a string and copying a C-string's contents into it.
		/**
		* @param cstr The C string to be copied
		*/
		String(const char *cstr);
		//! Constructor creating a string and copying an STL-string's contents into it.
		/**
		* @param str The STL string to be copied
		*/
		String(const std::string& str);
		//! Copy constructor
		/**
		* @param str The string to be copied
		*/
		String(const String& str);
		
		//! Destructor. Deallocates the internal STL string.
		~String();
		
		

		//! Copies a C string's contents.
		/**
		* @param cstr The C string to be copied
		*/
		String& operator =(const char *cstr);
		//! Copies a STL string's contents.
		/**
		* @param str The STL string to be copied
		*/
		String& operator =(const std::string& str);
		//! Copies a string.
		/**
		* @param str The string to be copied
		*/
		String& operator =(const String& str);
		
		
		

		//! Appends a C string's contents to this string.
		/**
		* @param cstr The C string to be appended
		*/
		String& operator +=(const char *cstr);
		//! Appends a STL string's contents to this string.
		/**
		* @param str The STL string to be appended
		*/
		String& operator +=(const std::string& str);
		//! Appends a string to this string.
		/**
		* @param str The string to be appended
		*/
		String& operator +=(const String& str);
		
		
		

		//! Copies a C string's contents.
		/**
		* @param cstr The C string to be copied
		*/
		void set(const char *cstr);
		//! Copies a STL string's contents.
		/**
		* @param str The STL string to be copied
		*/
		void set(const std::string& str);
		//! Copies a string.
		/**
		* @param str The string to be copied
		*/
		void set(const String& str);



		
		bool compare(const String& str,const bool caseSensitive) const;
		bool compare(const std::string& str,const bool caseSensitive) const;
		bool compare(const char *cstr,const bool caseSensitive) const;
		
		

		bool contains(const String& str,const unsigned long startpos=0) const;
		bool contains(const std::string& str,const unsigned long startpos=0) const;
		bool contains(const char *str,const unsigned long startpos=0) const;
		
		

		bool empty() const;

		unsigned long length() const;
		const char* cstr() const;
		const std::string& STLstring() const;
		operator const std::string& () const;

		bool isValid() const;
	protected:
		std::string *m_String;
	};

	OVIS_API bool operator ==(const String& s1,const String& s2);
	OVIS_API bool operator ==(const String& s,const char *cstr);
	OVIS_API bool operator ==(const char *cstr,const String& s);
	OVIS_API bool operator ==(const String& s,const std::string& str);
	OVIS_API bool operator ==(const std::string& str,const String& s);
	
	OVIS_API String operator +(const String& s1,const String& s2);
	OVIS_API String operator +(const String& s1,const std::string& s2);
	OVIS_API String operator +(const std::string& s1,const String& s2);
	
}
}

#endif
