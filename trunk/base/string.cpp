#include <algorithm>
#include <functional>
#include "string.hh"

namespace ovis {
namespace base {

	String::String():m_String(new std::string)
	{
	}

	String::String(const char *cstr)
	{
		m_String=cstr ? (new std::string(cstr)) : new std::string;
	}

	String::String(const std::string& str):m_String(new std::string(str))
	{
	}

	String::String(const String& str):m_String(new std::string(str.STLstring()))
	{
	}

	String::~String()
	{
		delete m_String;
	}

	String& String::operator =(const char *cstr)
	{
		(*m_String)=cstr;
		return *this;
	}

	String& String::operator =(const std::string& str)
	{
		(*m_String)=str;
		return *this;
	}

	String& String::operator =(const String& str)
	{
		(*m_String)=str.STLstring();
		return *this;
	}

	String& String::operator +=(const char *cstr)
	{
		(*m_String)+=cstr;
		return *this;
	}

	String& String::operator +=(const std::string& str)
	{
		(*m_String)+=str;
		return *this;
	}

	String& String::operator +=(const String& str)
	{
		(*m_String)+=str.STLstring();
		return *this;
	}

	void String::set(const char *cstr)
	{
		(*m_String)=cstr;
	}

	void String::set(const std::string& str)
	{
		(*m_String)=str;
	}

	void String::set(const String& str)
	{
		(*m_String)=str.STLstring();
	}

	bool String::compare(const std::string& str,const bool caseSensitive) const
	{
		return compare(String(str),caseSensitive);
	}

	bool String::compare(const char *cstr,const bool caseSensitive) const
	{
		return compare(String(cstr),caseSensitive);
	}
	
	bool String::compare(const String& str,const bool caseSensitive) const
	{
		if (length()!=str.length()) return false;

		if (caseSensitive) {
			for (unsigned long l=0;l<length();++l) {
				if (m_String->c_str()[l]!=str.cstr()[l]) return false;
			}
		} else {
			for (unsigned long l=0;l<length();++l) {
				if (toupper(m_String->c_str()[l])!=toupper(str.cstr()[l])) return false;
			}
		}

		return true;
	}

	bool String::contains(const String& str,const unsigned long startpos) const
	{
		return (m_String->find(str.STLstring(),startpos)!=std::string::npos);
	}

	bool String::contains(const std::string& str,const unsigned long startpos) const
	{
		return (m_String->find(str,startpos)!=std::string::npos);
	}

	bool String::contains(const char *str,const unsigned long startpos) const
	{
		return (m_String->find(std::string(str),startpos)!=std::string::npos);
	}

	bool String::empty() const
	{
		return m_String->empty();
	}

	unsigned long String::length() const
	{
		return static_cast<unsigned long>(m_String->length());
	}

	const char* String::cstr() const
	{
		return m_String->c_str();
	}

	
	const std::string& String::STLstring() const
	{
		return *m_String;
	}
		
	String::operator const std::string& () const
	{
		return *m_String;
	}

	bool String::isValid() const
	{
		return (m_String!=0);
	}




	bool operator ==(const String& s1,const String& s2)
	{
		return s1.compare(s2,true);
	}

	bool operator ==(const String& s,const char *cstr)
	{
		return s.compare(cstr,true);
	}

	bool operator ==(const char *cstr,const String& s)
	{
		return s.compare(cstr,true);
	}

	bool operator ==(const String& s,const std::string& str)
	{
		return s.compare(str,true);
	}

	bool operator ==(const std::string& str,const String& s)
	{
		return s.compare(str,true);
	}

	String operator +(const String& s1,const String& s2)
	{
		return String(s1.STLstring()+s2.STLstring());
	}

	String operator +(const String& s1,const std::string& s2)
	{
		return String(s1.STLstring()+s2);
	}

	String operator +(const std::string& s1,const String& s2)
	{
		return String(s1+s2.STLstring());
	}

}
}
