#include <dlfcn.h>
#include <iostream>
//#include <sstream>

#include "dynobject.hh"

namespace ovis {
namespace base {

struct Dynobject::Dynobjdata
{
	void *m_DLHandle;
};

Dynobject::Dynobject(const char *name):m_pData(new Dynobjdata)
{
	m_pData->m_DLHandle=dlopen(name,RTLD_LAZY);
	if (!m_pData->m_DLHandle) {
		//std::stringstream sstr;
		//sstr << "Dynobject error: could not load \"" << name << "\" :" << dlerror() << "\n";
		//ION_THROW(sstr.str().c_str());
	}
}

Dynobject::~Dynobject()
{
	dlclose(m_pData->m_DLHandle);
	delete m_pData;
}

void* Dynobject::function(const char *funcname)
{
	void *fnhandle=dlsym(m_pData->m_DLHandle,funcname);
	if (!fnhandle) {
		//std::cout << "Dynobject error: " << dlerror() << "\n";
	}

	
	return fnhandle;
}

}
}
