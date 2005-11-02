#ifndef OVIS_VIDEO_SCENENODEFACTORY_HH_INCLUDED
#define OVIS_VIDEO_SCENENODEFACTORY_HH_INCLUDED

#include "../base/datablock.hh"
#include "../base/string.hh"
#include "rootnode.hh"

namespace ovis {
namespace video {

	class Scenenodefactory
	{
	public:
		virtual Scenenode* createScenenode(const base::String& type,const base::String& identifier)=0;
		virtual ~Scenenodefactory() {}

		Rootnode* spawnScene(base::Datablock& source);
		void serializeScene(base::Datablock& sink,Rootnode& rRootNode);
	protected:
		Scenenodefactory() {}
		void spawnScenenode(base::Datablock& source,Scenenode* pScenenode);
		void serializeScenenode(base::Datablock& sink,Scenenode* pScenenode);
	};

}
}

#endif
