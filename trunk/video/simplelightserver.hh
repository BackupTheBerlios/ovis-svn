#ifndef OVIS_VIDEO_SIMPLELIGHTSERVER_HH_INCLUDED
#define OVIS_VIDEO_SIMPLELIGHTSERVER_HH_INCLUDED

#include "../base/dll.hh"
#include "../math/vectors.hh"
#include "light.hh"
#include "lightserver.hh"
#include <vector>

namespace ovis {
namespace video {

	class OVIS_API SimpleLightserver:public Lightserver
	{
	public:
		void nearestLights(const Scenenode& scenenode,const unsigned long maxlights,Lightlist& lightlist);
		void nearestLightsInRadius(const Scenenode& scenenode,const float radius,Lightlist& lightlist);
	};

}
}

#endif
