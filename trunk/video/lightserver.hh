#ifndef OVIS_VIDEO_LIGHTSERVER_HH
#define OVIS_VIDEO_LIGHTSERVER_HH

#include "lightlist.hh"

namespace ovis {
namespace video {

	class Scenenode;

	class OVIS_API Lightserver:public Lightlist
	{
	public:
		virtual void nearestLights(const Scenenode& scenenode,const unsigned long maxlights,Lightlist& lightlist)=0;
		virtual void nearestLightsInRadius(const Scenenode& scenenode,const float radius,Lightlist& lightlist)=0;
	};

}
}

#endif
