#include "refcounted.hh"

namespace ovis {
namespace base {

	RefCounted::RefCounted(const String& objectidentifier):
	ManagedMemObj(objectidentifier),m_RefCount(0),m_pRefcountedManager(0)
	{
	}

	RefCounted::~RefCounted()
	{
		if (m_pRefcountedManager) m_pRefcountedManager->refcountedIsBeingDeleted(this);
	}

	void RefCounted::deleteThis()
	{
		delete(this);
	}

	int RefCounted::addRef() { return (++m_RefCount); }
	int RefCounted::release()
	{
		--m_RefCount;
		if (m_RefCount>0) return m_RefCount;
		else {
			deleteThis();
			return 0;
		}
	}

	RefcountedManager* RefCounted::refcountedManager()
	{
		return m_pRefcountedManager;
	}

	void RefCounted::refcountedManager(RefcountedManager* pManager)
	{
		m_pRefcountedManager=pManager;
	}

}
}
