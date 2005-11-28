#include "../base/log.hh"
#include "attributebuffer.hh"
#include "attributetable.hh"

namespace ovis {
namespace video {

	Attributetable::Attributetable():m_pEntries(new std::vector <Attributetableentry>)
	{
	}

	Attributetable::Attributetable(const Attributetable& table):m_pEntries(0)
	{
		clone(table);
	}

	Attributetable::Attributetable(const Attributebuffer& buffer):
	m_pEntries(new std::vector <Attributetableentry>)
	{
		recalculate(buffer);
	}

	Attributetable::~Attributetable()
	{
		if (m_pEntries) delete m_pEntries;
	}

	void Attributetable::clone(const Attributetable& table)
	{
		if (m_pEntries) delete m_pEntries;
		m_pEntries=new std::vector <Attributetableentry>;

		for (ovis_uint32 entrynr=0;entrynr<table.numEntries();++entrynr) {
			const Attributetableentry *pSrcEntry=table.entry(entrynr);
			Attributetableentry entry;
			entry.m_Faceattribute=pSrcEntry->m_Faceattribute;
			entry.m_Startface=pSrcEntry->m_Startface;
			entry.m_Faceamount=pSrcEntry->m_Faceamount;
			m_pEntries->push_back(entry);
		}
	}

	void Attributetable::recalculate(const Attributebuffer& buffer)
	{
		m_pEntries->clear();

		Attributetableentry entry;
		entry.m_Startface=0;
		entry.m_Faceamount=0;
		entry.m_Faceattribute=buffer[0];

		ovis_uint32 face=0;
		for (;face<buffer.attributeAmount();++face) {
			if (entry.m_Faceattribute!=buffer[face]) {
				entry.m_Faceamount=face-entry.m_Startface;
				m_pEntries->push_back(entry);
				entry.m_Startface=face;
				entry.m_Faceamount=0;
				entry.m_Faceattribute=buffer[face];
			}
		}

		entry.m_Faceamount=face-entry.m_Startface;
		m_pEntries->push_back(entry);
	}

	void Attributetable::dumpTable()
	{
		std::vector <Attributetableentry>::const_iterator itentry=m_pEntries->begin();
		for (;itentry!=m_pEntries->end();++itentry) {
			const Attributetableentry& entry=*itentry;
			base::logstream() << "[ " << entry.m_Startface << " " << entry.m_Faceamount << " " << entry.m_Faceattribute << " ]\n";
		}
	}

	const Attributetableentry* Attributetable::entry(const ovis_uint32 index) const
	{
		return (index>=m_pEntries->size()) ? 0 : &((*m_pEntries)[index]);
	}

	ovis_uint32 Attributetable::numEntries() const
	{
		return static_cast<ovis_uint32>(m_pEntries->size());
	}

}
}
