#ifndef OVIS_VIDEO_ATTRIBUTETABLE_HH_INCLUDED
#define OVIS_VIDEO_ATTRIBUTETABLE_HH_INCLUDED

#include "../base/dll.hh"
#include <vector>

namespace ovis {
namespace video {

	class Attributebuffer;

	struct OVIS_API Attributetableentry
	{
		ovis_uint32 m_Faceattribute,m_Startface,m_Faceamount;
	};

	class OVIS_API Attributetable
	{
	public:
		Attributetable();
		Attributetable(const Attributetable& table);
		Attributetable(const Attributebuffer& buffer);
		~Attributetable();

		void clone(const Attributetable& table);

		void recalculate(const Attributebuffer& buffer);

		void dumpTable();

		const Attributetableentry* entry(const ovis_uint32 index) const;
		ovis_uint32 numEntries() const;
		//inline const std::vector <Attributetableentry>& entries() const { return *m_pEntries; }
	protected:
		std::vector <Attributetableentry> *m_pEntries;
	};

}
}

#endif
