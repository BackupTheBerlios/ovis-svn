#ifndef OVIS_LOADSCENEFROMFILE_HH_INCLUDED
#define OVIS_LOADSCENEFROMFILE_HH_INCLUDED

#include "../base/string.hh"

namespace ovis {
namespace video {

	class Renderer;
	class Scene;
	
	Scene* loadSceneFromFile(const base::String& filename,Renderer &rRenderer);

}	
}

#endif
