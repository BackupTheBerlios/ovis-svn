#ifndef OVIS_VIDEO_OCCLUSIONQUERY_HH_INCLUDED
#define OVIS_VIDEO_OCCLUSIONQUERY_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/managedmemobj.hh"

namespace ovis {
namespace video {

	class OVIS_API Occlusionquery:public base::ManagedMemObj
	{
	public:
		virtual ~Occlusionquery() {}

		virtual void start()=0;
		virtual void end()=0;

		virtual bool pixelsVisible(unsigned int& result)=0;

	protected:
		Occlusionquery(const char *queryidentifier):ManagedMemObj(queryidentifier) {}
	};

}
}

#endif
