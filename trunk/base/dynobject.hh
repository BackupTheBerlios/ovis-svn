#ifndef OVIS_BASE_DYNOBJECT_HH_INCLUDED
#define OVIS_BASE_DYNOBJECT_HH_INCLUDED

#include "dll.hh"

namespace ovis {
namespace base {

	//! Dynamic objects loader class.
	/** 
	* Dynamic objects contain code which can be loaded at run-time.
	* In Windows, they are called DLLs, in *nix/Linux SOs (shared objects).
	* This class handles the dynamic object loading as well as loading functions
	* from them.
	*/
	class OVIS_API Dynobject
	{
	public:
		//! Constructs the Dynobject instance and loads a dynamic object.
		/**
		* @param name The filename of the dynamic object. Example: "example.so"
		* @note If loading fails, isValid() returns false, else it returns true.
		*/
		Dynobject(const char *name);

		//! Destroys the Dynobject instance and unloads the dynamic object.
		/** 
		* @note All function pointers pointing to functions in this dynamic object
		* are invalid after unloading!
		*/
		~Dynobject();

		//! Retrieves a function pointer to the specified function.
		/** 
		* @param funcname Name of the function to be searched.
		* @return Pointer to the specified function, or 0 if none is found.
		*/
		void *function(const char *funcname);

		//! Returns whether this instance is valid or not.
		/** If the dynamic object could not be loaded properly in the constructor,
		* this instance is invalid.
		* @return true if the instance is valid, false otherwise.
		*/
		bool isValid() const;

	protected:
		struct Dynobjdata;
		Dynobjdata* m_pData;
	};

}
}

#endif
