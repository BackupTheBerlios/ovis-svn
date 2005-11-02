#ifndef OVIS_BASE_STRINGLIST_HH_INCLUDED
#define OVIS_BASE_STRINGLIST_HH_INCLUDED

#include <vector>
#include "../base/dll.hh"
#include "string.hh"

namespace ovis {
namespace base {

	class OVIS_API Stringlist
	{
	public:
		Stringlist();
		Stringlist(const Stringlist& src);
		~Stringlist();

		Stringlist& operator = (const Stringlist& src);

		void addString(const String& str);
		size_t numStrings() const;
		const String* string(const size_t index) const;
		void emptyList();

		const String* find(const String& str,const bool caseSensitive) const;
		String* find(const String& str,const bool caseSensitive);

	protected:
		void clone(const Stringlist& src);

		std::vector<String> *m_Strings;
	};

}
}

#endif
