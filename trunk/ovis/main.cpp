#include "../base/log.hh"
#include "../base/stdoutstream.hh"
#include "renderinstance.hh"

namespace ovis {
namespace video {
	OVIS_API Scene* generateTestscene(Renderer *pRenderer,Colorscale *pColorscale);
}
}

int main(int argc,char **argv)
{
	ovis::base::logstream(ovis::base::Stdoutstream::stream());

	ovis::video::Scene *pScene=ovis::video::generateTestscene(0,0);
	ovis::Renderinstance ri;
	ri.initRenderer(pScene,ovis::Renderinstance::UsedRenderer_Opengl,ovis::Renderinstance::Window_Normal,640,480);
	ri.interactiveLoop();
	ri.endRenderer();
	
	delete pScene;

	return 0;
}
