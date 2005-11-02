#ifndef OVIS_VIDEO_SCENENODE_HH_INCLUDED
#define OVIS_VIDEO_SCENENODE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/managedmemobj.hh"
#include "../base/refcounted.hh"
#include "../base/streamable.hh"

#include <deque>
#include <vector>

#ifndef SCENENODEACCESSOR
#define SCENENODEACCESSOR(Classname,Methodname,Pointername) \
void Methodname(Classname& node) \
{ \
	if ((&node)!=Pointername) { \
		addInputNode(node); \
		if (Variablename!=0) removeInputNode( *Pointername ); \
		Pointername=&node; \
	} \
} \
Classname& Methodname() { return *Pointername; } \
const Classname& Methodname() const { return *Pointername; }
#endif

namespace ovis {
namespace video {

	class Camera;
	class Visitorqueue;
	class Renderer;

	class OVIS_API Visitcontext
	{
	public:
		Visitcontext(const Camera &rCamera,Visitorqueue &rVisitorqueue,Renderer &rRenderer);

		const Camera& camera();
		Visitorqueue& visitorqueue();
		Renderer& renderer();
	protected:
		const Camera &m_rCamera;
		Visitorqueue &m_rVisitorqueue;
		Renderer &m_rRenderer;
	};

	//! A scenegraph node.

	class OVIS_API Scenenode:public base::RefCounted
	{
	public:
		virtual void onVisit(Visitcontext& visitcontext)=0;

		virtual void spawn(base::Streamable& datasource)=0;
		virtual void serialize(base::Streamable& datasink) const=0;

		virtual const char *type() const=0;

		//virtual bool serializeThis() const;

		virtual void clone(const Scenenode &srcSceneNode);

		virtual Scenenode& operator =(const Scenenode& src);

		Scenenode(const Scenenode &srcSceneNode);
		virtual ~Scenenode();

		unsigned long numInputNodes() const;
		unsigned long numOutputNodes() const;

		Scenenode* inputnode(const unsigned long nr) const;
		Scenenode* outputnode(const unsigned long nr) const;

		bool hasInputNode(Scenenode &rNode);
		bool hasOutputNode(Scenenode &rNode);

		struct Internaldata;
		Internaldata& internaldata();

	protected:
		Scenenode(const base::String& identifier);

		void addInputNode(Scenenode &rNode);
		void addOutputNode(Scenenode &rNode);

		void removeInputNode(Scenenode &rNode);
		void removeOutputNode(Scenenode &rNode);

		Internaldata* m_pInternaldata;
	};

}
}

#endif
