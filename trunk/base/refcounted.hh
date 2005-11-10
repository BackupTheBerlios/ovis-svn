#ifndef OVIS_BASE_REFCOUNTED_HH_INCLUDED
#define OVIS_BASE_REFCOUNTED_HH_INCLUDED

#include "dll.hh"
#include "managedmemobj.hh"

namespace ovis {
namespace base {

	class RefcountedManager;

	//! Base class for reference counted objects.
	/**
	* This implements the classic approach to reference counting.
	* It also inherits its main weakness: circular references. Unfortunately, boost::shared_ptr
	* has serious DLL problems, making it useless in cross-DLL environments.
	* This class contains a reference counter, which is incremented with addRef().
	* To decrement it, release() is called, and when the counter reaches zero, release() destroys
	* the object.
	*
	* The initial reference count is zero.
	* Example usage:
	*
	* @code
	* RefCounted *pRefcountedObj=someResourceManager->resource("res1"); // InsomeResourceManager->resource(), addRef() is called
	* ....
	* pRefcountedObj->release(); // Decrements the reference count. If this was the only reference, the count becomes zero, and the object is deallocated
	* @endcode
	*
	* Additionally, a pointer to a refcount manager is stored in RefCounted. If this pointer points to a RefcountedManager
	* instance, the destructor calls this instance, notifying it from the RefCounted instance's destruction. This is useful for
	* resource management; the resource manager is also the refcount manager. If one of the resources is destroyed, the resource
	* manager is notified, thus giving it a chance to erase the resource from its list of active resources. 
	*/
	class OVIS_API RefCounted:public ManagedMemObj
	{
	public:
		//! Destructor.
		virtual ~RefCounted();
		
		//! Increments the reference counter.
		/**
		* Usually, this is called when some object acquired an additional reference to this object.
		* @return The new reference count.
		*/
		int addRef();
		//! Decrements the reference counter, and eventually destroys the object.
		/**
		* First, this decrements the reference counter. If after the decrement the counter is zero,
		* or if the counter was zero before, "delete this;" is called, destroying the object.
		* After this, all calls to this object are invalid! So, if release() is called, the object
		* should be considered deallocated.
		* @return The new reference count.
		*/
		int release();
		//! Returns the reference count.
		/**
		* @return The new reference count.
		*/
		inline int refcount() const { return m_RefCount; }

		//! Returns the refcount manager.
		/**
		* @return Pointer to the refcount manager, or 0 if no manager is used.
		*/
		RefcountedManager* refcountedManager();
	
		//! Sets the refcount manager.
		/**
		* @param pManager The refcount manager to use, or 0 if no manager shall be used.
		*/
		void refcountedManager(RefcountedManager* pManager);

	protected:
		RefCounted(const String& objectidentifier);
		virtual void deleteThis();

	private:
		int m_RefCount;
		RefcountedManager *m_pRefcountedManager;
	};

	//! Refcount manager which gets notified when RefCounted objects are destroyed.
	/**
	* For details, see the RefCount description.
	*/
	class OVIS_API RefcountedManager
	{
	public:
		//! This method is called when a RefCounted instance is destroyed.
		/**
		* @param pRefCounted The RefCounted instance that is being destroyed
		*/
		virtual void refcountedIsBeingDeleted(RefCounted* pRefCounted)=0;
		virtual ~RefcountedManager() {}
	};

}
}

#endif
