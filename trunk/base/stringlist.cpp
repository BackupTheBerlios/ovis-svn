#include "stringlist.hh"

namespace ovis {
namespace base {

	Stringlist::Stringlist():m_Strings(0)
	{
		m_Strings=new std::vector<String>;
	}

	Stringlist::Stringlist(const Stringlist& src):m_Strings(new std::vector<String>)
	{
		clone(src);
	}

	Stringlist::~Stringlist()
	{
		delete m_Strings;
	}

	Stringlist& Stringlist::operator = (const Stringlist& src)
	{
		clone(src);
		return *this;
	}

	void Stringlist::addString(const String& str)
	{
		m_Strings->push_back(str);
	}

	size_t Stringlist::numStrings() const
	{
		return m_Strings->size();
	}

	const String* Stringlist::string(const size_t index) const
	{
		if (index>=m_Strings->size()) return 0;

		return &((*m_Strings)[index]);
	}

	void Stringlist::emptyList()
	{
		m_Strings->clear();
	}

	void Stringlist::clone(const Stringlist& src)
	{
		for (size_t i=0;i<src.numStrings();++i) {
			addString(*(src.string(i)));
		}
	}

	const String* Stringlist::find(const String& str,const bool caseSensitive) const
	{
		for (std::vector<String>::const_iterator it=m_Strings->begin();it!=m_Strings->end();++it) {
			const String& curstr=(*it);
			if (curstr.compare(str,caseSensitive)) return &curstr;
		}

		return 0;
	}

	String* Stringlist::find(const String& str,const bool caseSensitive)
	{
		for (std::vector<String>::iterator it=m_Strings->begin();it!=m_Strings->end();++it) {
			String& curstr=(*it);
			if (curstr.compare(str,caseSensitive)) return &curstr;
		}

		return 0;
	}

}
}
