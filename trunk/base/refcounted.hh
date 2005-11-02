#ifndef OVIS_BASE_REFCOUNTED_HH_INCLUDED
#define OVIS_BASE_REFCOUNTED_HH_INCLUDED

#include "dll.hh"
#include "managedmemobj.hh"

namespace ovis {
namespace base {

	class RefcountedManager;

	class OVIS_API RefCounted:public ManagedMemObj
	{
	public:
		virtual ~RefCounted();
		
		int addRef();
		int release();
		inline int refcount() const { return m_RefCount; }

		RefcountedManager* refcountedManager();
		void refcountedManager(RefcountedManager* pManager);

	protected:
		RefCounted(const String& objectidentifier);
		virtual void deleteThis();

	private:
		int m_RefCount;
		RefcountedManager *m_pRefcountedManager;
	};

	class OVIS_API RefcountedManager
	{
	public:
		virtual void refcountedIsBeingDeleted(RefCounted* pRefCounted)=0;
		virtual ~RefcountedManager() {}
	};

}
}

#endif
