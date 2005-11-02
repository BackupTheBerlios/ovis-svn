#include "managedmemobj.hh"
#include "log.hh"
#include "string.hh"
#include <cstring>
#include <set>

namespace ovis {
namespace base {

	/*
	Object instance logger. Keeps track of all ManagedMemObj. Keeps a list of all newly created ManagedMemObjs.
	ManagedMemObjs being deleted are erased from the list. If a ManagedMemObj is not in the list,
	then its deletion is cancelled since this object wasnt created by new and thus is not delete-able.
	(Most likely a ManagedMemObj created in the stack and not in the heap)
	*/

	class ObjInstanceLogger
	{
	public:
		ObjInstanceLogger() {}
		~ObjInstanceLogger() {}

		void addObjInstance(ManagedMemObj* pInst,const bool array);
		bool delObjInstance(ManagedMemObj* pInst,const bool array);

		// For debugging puroses. Prints a list of all ManagedMemObjs.
		void dumpStats();

		void cleanupGarbage();


		static ObjInstanceLogger *m_InstanceLogger;
	protected:
		void dumpStats(std::set< ManagedMemObj* >& instances,const char *desc);

		std::set< ManagedMemObj* > m_Instances,m_ArrayInstances;
	};

	void ObjInstanceLogger::addObjInstance(ManagedMemObj* pInst,const bool array)
	{
		std::set< ManagedMemObj* >& iset=(array) ? m_ArrayInstances : m_Instances;
		iset.insert(pInst);
	}

	bool ObjInstanceLogger::delObjInstance(ManagedMemObj* pInst,const bool array)
	{
		std::set< ManagedMemObj* >& iset=(array) ? m_ArrayInstances : m_Instances;
		std::set< ManagedMemObj* >::iterator it=iset.find(pInst);
		if (it==iset.end()) {
			return false;
		} else {
			iset.erase(it);
			return true;
		}
	}

	void ObjInstanceLogger::dumpStats(std::set< ManagedMemObj* >& instances,const char *desc)
	{
		log("ObjInstanceLogger::dumpStats()",Message) << desc << (long)(instances.size()) << " entries\n";
		std::set< ManagedMemObj* >::iterator it=instances.begin();
		unsigned long i=0;
		for (;it!=instances.end();++it,++i) {
			logstream() << i << ": \"" << (*it)->objIdentifier() <<"\" ptr: " << ((long long)(*it)) << "\n";
		}
	}

	void ObjInstanceLogger::dumpStats()
	{
		dumpStats(m_Instances,"allocated blocks: ");
		dumpStats(m_ArrayInstances,"allocated array blocks: ");
		log("ObjInstanceLogger::dumpStats()",Message) << "Total allocated blocks: " << (long)((m_Instances.size()+m_ArrayInstances.size())) << " entries\n";
	}

	void ObjInstanceLogger::cleanupGarbage()
	{
		/*{
			std::set< ManagedMemObj* >::reverse_iterator it=m_Instances.rbegin();
			for (;it!=m_Instances.rend();++it) {
				ManagedMemObj* pObj=(*it);
				::delete(pObj);
				pObj=pObj;
			}
			m_Instances.clear();
		}
		{
			std::set< ManagedMemObj* >::reverse_iterator it=m_ArrayInstances.rbegin();
			for (;it!=m_ArrayInstances.rend();++it) {
				ManagedMemObj* pObj=(*it);
				::delete [] (pObj);
			}
			m_ArrayInstances.clear();
		}*/
		{
			std::set< ManagedMemObj* >::reverse_iterator it=m_Instances.rbegin();
			while (1) {
				it=m_Instances.rbegin(); if (it==m_Instances.rend()) break;
				ManagedMemObj* pObj=(*it);
				delete pObj;
			}
			m_Instances.clear();
		}
		{
			std::set< ManagedMemObj* >::reverse_iterator it=m_ArrayInstances.rbegin();
			while (1) {
				it=m_Instances.rbegin(); if (it==m_Instances.rend()) break;
				ManagedMemObj* pObj=(*it);
				delete [] (pObj);
			}
			m_ArrayInstances.clear();
		}
	}


	ObjInstanceLogger *ObjInstanceLogger::m_InstanceLogger=0;

	void startInstanceLogging()
	{
		if (ObjInstanceLogger::m_InstanceLogger) return;
		ObjInstanceLogger::m_InstanceLogger=new ObjInstanceLogger;

	}

	void endInstanceLogging()
	{
		if (!ObjInstanceLogger::m_InstanceLogger) return;
		ObjInstanceLogger::m_InstanceLogger->cleanupGarbage();
		delete ObjInstanceLogger::m_InstanceLogger;
		ObjInstanceLogger::m_InstanceLogger=0;
	}

	void dumpInstanceLoggingStats()
	{
		if (!ObjInstanceLogger::m_InstanceLogger) return;
		ObjInstanceLogger::m_InstanceLogger->dumpStats();

	}







	ManagedMemObj::ManagedMemObj(const String& objectidentifier):
	m_ObjIdentifier(new String(objectidentifier))
	{
	}
	
	ManagedMemObj::ManagedMemObj(const ManagedMemObj& src):
	m_ObjIdentifier(new String(src.objIdentifier()))
	{
	}

	ManagedMemObj::~ManagedMemObj()
	{
		delete m_ObjIdentifier;
	}

	void* ManagedMemObj::operator new(size_t size)
	{
//		std::cout << "operator new()\n";
	
		void *p= ::operator new(size);
		if (ObjInstanceLogger::m_InstanceLogger)
			ObjInstanceLogger::m_InstanceLogger->addObjInstance((ManagedMemObj*)p,false);
		return p;
	}

	void ManagedMemObj::operator delete(void *mem)
	{
//		std::cout << "operator delete()\n";

		bool b=true;
		if (ObjInstanceLogger::m_InstanceLogger)
			b=ObjInstanceLogger::m_InstanceLogger->delObjInstance((ManagedMemObj*)mem,false);
		if (b) ::operator delete(mem);
	}

	void* ManagedMemObj::operator new[](size_t size)
	{
//		std::cout << "operator new[]()\n";
	
		void *p= ::operator new[](size);
		if (ObjInstanceLogger::m_InstanceLogger)
			ObjInstanceLogger::m_InstanceLogger->addObjInstance((ManagedMemObj*)p,true);
		return p;
	}

	void ManagedMemObj::operator delete[](void *mem)
	{
//		std::cout << "operator delete[]()\n";
	
		bool b=true;
		if (ObjInstanceLogger::m_InstanceLogger)
			b=ObjInstanceLogger::m_InstanceLogger->delObjInstance((ManagedMemObj*)mem,true);
		if (b) ::operator delete[](mem);
	}
	
	const String& ManagedMemObj::objIdentifier() const
	{
		return *m_ObjIdentifier;
	}
	
	void ManagedMemObj::operator =(const ManagedMemObj& src)
	{
		//delete m_ObjIdentifier;
		//m_ObjIdentifier=new String(src.objIdentifier());
		*m_ObjIdentifier=src.objIdentifier();
	}

}
}
