#include <windows.h>
#include "dynobject.hh"

namespace ovis {
namespace base {

	struct Dynobject::Dynobjdata
	{
		HINSTANCE m_DLLInstance;
	};

	Dynobject::Dynobject(const char *name):m_pData(0)
	{
		m_pData=new Dynobjdata;
		m_pData->m_DLLInstance=LoadLibraryA(name);
	}

	Dynobject::~Dynobject()
	{
		if (m_pData) {
			FreeLibrary(m_pData->m_DLLInstance);
			delete m_pData;
		}
	}

	void* Dynobject::function(const char *funcname)
	{
		if (!m_pData) return 0;
		else return (void*)(GetProcAddress(m_pData->m_DLLInstance,funcname));
	}

	bool Dynobject::isValid() const
	{
		if (!m_pData) return false;
		else return m_pData->m_DLLInstance!=0;
	}

}
}
