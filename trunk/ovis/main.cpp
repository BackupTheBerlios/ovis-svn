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

	ovis::Renderinstance ri;
	ri.testinitRenderer(ovis::Renderinstance::UsedRenderer_Opengl,ovis::Renderinstance::Window_Normal,640,480,true);
	ri.waitUntilWindowClosed();
	ri.shutdownRenderer();

	return 0;
}
