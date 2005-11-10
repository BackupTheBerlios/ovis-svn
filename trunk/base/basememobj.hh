#ifndef OVIS_BASE_MEM_OBJECT_HH_INCLUDED
#define OVIS_BASE_MEM_OBJECT_HH_INCLUDED

#include <cstdlib>
#include "dll.hh"

namespace ovis {
namespace base {

	/**
	Implements basic memory management using new/delete operator overloading.
	This is necessary to ensure DLL-safety. The overloaded operators make sure that
	allocation and deallocation happen in the same heap.
	*/
	class OVIS_API BaseMemObj
	{
	public:
		virtual ~BaseMemObj();
		
		void * operator new(size_t size);
		void * operator new(size_t size,void *p);
		void operator delete(void *mem);
		void operator delete(void *mem,void *p);

		void * operator new[](size_t size,void *p);
		void * operator new[](size_t size);
		void operator delete[](void *mem);
		void operator delete[](void *mem,void *p);

		// Getter for sanity check
		/**
		* This is intended for implementation in derived classes. It allows
		* a sanity check whether the object's state is OK or invalid.
		* For example, when a component could not be initialized, the object
		* is in an invalid state, and this returns false.
		* @return true if the object is valid, false otherwise.
		*/
		virtual bool isValid() const=0;

	protected:
		BaseMemObj();
	};

}
}

#endif
