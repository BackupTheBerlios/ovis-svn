#ifndef OVIS_BASE_STRING_HH_INCLUDED
#define OVIS_BASE_STRING_HH_INCLUDED

#include <string>
#include "basememobj.hh"
#include "dll.hh"

namespace ovis {
namespace base {

	class OVIS_API String:public BaseMemObj
	{
	public:
		String();
		String(const char *cstr);
		String(const std::string& str);
		String(const String& str);
		~String();

		String& operator =(const char *cstr);
		String& operator =(const std::string& str);
		String& operator =(const String& str);

		String& operator +=(const char *cstr);
		String& operator +=(const std::string& str);
		String& operator +=(const String& str);

		void set(const char *cstr);
		void set(const std::string& str);
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
