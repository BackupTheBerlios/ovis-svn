#ifndef OVIS_BASE_REFPTR_HH_INCLUDED
#define OVIS_BASE_REFPTR_HH_INCLUDED

#include <assert.h>
#include "dll.hh"
#include "itypes.hh"

namespace ovis {
namespace base {

	template <class T> class OVIS_API RefPtr
	{
	typedef RefPtr<T> _Self;
	public:
		RefPtr():m_Pointer(0) {}
		explicit RefPtr(T *pointer):m_Pointer(pointer) {}
		RefPtr (const _Self &p) {
			m_Pointer=p.get();
			m_Pointer->addRef();
		}
		~RefPtr ()
		{
			if (m_Pointer) { m_Pointer->release(); }
		}
		
		inline void clearPointer(const bool release) {
			if (release && m_Pointer) m_Pointer->release();
			m_Pointer=0;
		}

		inline T* get() const { assert(m_Pointer); return m_Pointer; }
		inline bool hasPointer() const { return m_Pointer!=0; }

		inline _Self& operator =(const _Self& refptr) {
			T* ptr=refptr.get();
			ptr->addRef();
			if (m_Pointer) { m_Pointer->release(); }
			m_Pointer=ptr;

			return *this;
		}
		inline T* operator ->() const { return get(); }
		inline T* operator *() const { return *get(); }

	private:
		T *m_Pointer;
	};

}
}

#endif
