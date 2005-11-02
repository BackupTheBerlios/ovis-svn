#include "simplelightserver.hh"

namespace ovis {
namespace video {

	void SimpleLightserver::nearestLights(const Scenenode& scenenode,const unsigned long maxlights,Lightlist& lightlist)
	{
		unsigned long num=(maxlights<numLights()) ? maxlights : numLights();
		
		for (unsigned long l=0;l<num;++l) {
			lightlist.addLight(&light(l));
		}
	}

	void SimpleLightserver::nearestLightsInRadius(const Scenenode& scenenode,const float radius,Lightlist& lightlist)
	{
		unsigned long num=numLights();

		for (unsigned long l=0;l<num;++l) {
			lightlist.addLight(&light(l));
		}
	}

}
}
