#include "rootnode.hh"
#include "scenenodefactory.hh"

namespace ovis {
namespace video {

	Rootnode* Scenenodefactory::spawnScene(base::Datablock& source)
	{
		base::String identifier;
		source >> identifier;
		Rootnode *pRootnode=new Rootnode(identifier);
		spawnScenenode(source,pRootnode);

		return pRootnode;
	}

	void Scenenodefactory::spawnScenenode(base::Datablock& source,Scenenode* pScenenode)
	{
		pScenenode->spawn(source);

		ovis_uint32 numchildren;
		source >> numchildren;
		for (ovis_uint32 i=0;i<numchildren;++i) {
			base::String type;
			source >> type;

			base::String identifier;
			source >> identifier;

			Scenenode *pChild=createScenenode(type,identifier);
			float x,y,z,w;
			source >> x >> y >> z; pChild->position(x,y,z);
			source >> x >> y >> z >> w; pChild->rotation(x,y,z,w);
			source >> x >> y >> z; pChild->scale(x,y,z);
			pScenenode->addChild(*pChild);
		
			spawnScenenode(source,pChild);
		}
	}

	// This is just for forcing the usage of a rootnode
	void Scenenodefactory::serializeScene(base::Datablock& sink,Rootnode& rRootNode)
	{
		if (!rRootNode.serializeThis()) return;
		serializeScenenode(sink,&rRootNode);
	}

	void Scenenodefactory::serializeScenenode(base::Datablock& sink,Scenenode* pScenenode)
	{
		pScenenode->serialize(sink);

		unsigned long childnr=0,numchildren=0;
		for (;childnr<pScenenode->numChildren();++childnr)
			if (pScenenode->child(childnr).serializeThis()) ++numchildren;

		sink << numchildren;
		
		for (unsigned long childnr=0;childnr<pScenenode->numChildren();++childnr) {
			Scenenode* pNode=&(pScenenode->child(childnr));
			if (!pNode->serializeThis()) continue;

			sink << pNode->type();
			sink << pNode->objIdentifier();
			sink << pNode->position().x() << pNode->position().y() << pNode->position().z();
			sink << pNode->rotation().x() << pNode->rotation().y() << pNode->rotation().z();
			sink << pNode->scale().x() << pNode->scale().y() << pNode->scale().z();
			serializeScenenode(sink,pNode);
		}
	}

}
}
