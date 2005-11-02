#include <memory.h>
#include "basememobj.hh"

#include <set>


namespace ovis {
namespace base {


	BaseMemObj::BaseMemObj()
	{
	}

	BaseMemObj::~BaseMemObj()
	{
	}
	
	// Just some overloading to assure that new and delete are called in base

	void* BaseMemObj::operator new(size_t size)
	{
		return ::operator new(size);
	}

	void* BaseMemObj::operator new(size_t size,void *p)
	{
		return p;
	}

	void BaseMemObj::operator delete(void *mem)
	{
		::operator delete(mem);
	}

	void BaseMemObj::operator delete(void *mem,void *p)
	{
		::operator delete[](mem);
	}

	void* BaseMemObj::operator new[](size_t size,void *p)
	{
		return p;
	}

	void BaseMemObj::operator delete[](void *mem)
	{
		::operator delete[](mem);
	}

	void BaseMemObj::operator delete[](void *mem,void *p)
	{
		::operator delete[](mem);
	}

}
}
