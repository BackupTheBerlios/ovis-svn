#ifndef OVIS_VIDEO_SIMPLERENDERER_HH_INCLUDED
#define OVIS_VIDEO_SIMPLERENDERER_HH_INCLUDED

#include <set>

#include "renderer.hh"
#include "scenenode.hh"

namespace ovis {
namespace video {

	class OVIS_API SimpleRenderer:public Renderer
	{
	public:
		SimpleRenderer(Videodevice& rVideodevice);
		~SimpleRenderer();

		bool supported() const;
		void renderScene(const Camera& camera,Scenenode& scenenode);

		void checkResources();
	protected:
		void traverseScene(std::set<Scenenode*> &donenodes,Visitcontext& vctx,Scenenode& scenenode);
	};

}
}

#endif
