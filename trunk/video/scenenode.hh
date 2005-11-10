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
	/**
	* A scenegraph is a directed acyclic graph consisting of scenenodes.
	* Scenenode is the base class for each node.
	* A scenenode has ingoing and outgoing connections to other nodes.
	* A model node with connections to material nodes would have the material nodes
	* listed as input nodes. Likewise, material nodes would have a model node as
	* outgoing node. Since Scenenode is derived from RefCounted, reference counting
	* is performed; each time a node is connected to another one, reference counting
	* happens - both nodes' reference count increases.
	*
	* @note Circular references can occur when several nodes are isolated and no reference
	* other than each one's reference to each other exist.
	*
	* Usually, scenegraphs are rendered by a renderer using a visitor
	* visiting all nodes in the graph, calling onVisit() on each node and passing a visit context.
	* as parameter. The node then optionally stores information about the visit in the context.
	* For example, a meshnode would store rendering information in the visitorqueue in the context.
	* When all nodes have been visited, the renderer looks at all queued informations about the visits,
	* and renders according to these informations.
	*
	* Each scenenode class has its own type specification exposed by using type().
	* Note that this type specifier is NOT intended for RTTI use, but for debugging
	* and serialization purposes.
	*/
	class OVIS_API Scenenode:public base::RefCounted
	{
	public:
		//! This is called when a visitor visits all nodes in a graph.
		/**
		* @param visitcontext The visit context containing queues for visits etc. The node stores information about the visit there
		*/
		virtual void onVisit(Visitcontext& visitcontext)=0;

		//! Spawns a valid state of this node from serialized information.
		/**
		* Respawning serialized nodes start by creating the node according to the serialized type specifier.
		* After the node is created, spawn() is called and the streamable containing the serialized
		* information is provided. The node reads data from the streamable and restores its previous state.
		* @param datasource The streamable containing previously serialized information
		*/
		virtual void spawn(base::Streamable& datasource)=0;
		
		//! Serializes the node.
		/**
		* The node serializes itself into the specified streamable.
		* It is up to the node which data it writes into the streamable,
		* however it should contain enough information so that the node
		* can later be respawned successfully.
		* @param datasink The streamable where the node can serialize itself, i.e. where it can write data for later respawning
		*/
		virtual void serialize(base::Streamable& datasink) const=0;

		//! Type specifier.
		
		/*
		*Each scenenode class has its own type specification exposed by using this method.
		* Note that this type specifier is NOT intended for RTTI use, but for debugging
		* and serialization purposes. This specifier should be unique, ideally the class name.
		* 
		*/
		virtual const char *type() const=0;

		//! Clones the node.
		/**
		* Cloning functionality in the base class is limited to cloning the connections to other nodes.
		* The connections of the source node are not terminated.
		* Scenenode-derived classes may clone additional data.
		* @param srcSceneNode The source scenenode to be cloned.
		*/
		virtual void clone(const Scenenode &srcSceneNode);

		//! Clones the node.
		/**
		* This behaves exactly like clone().
		* @param src The source scenenode to be cloned.
		*/
		virtual Scenenode& operator =(const Scenenode& src);

		//! Copy constructor.
		/**
		* This behaves exactly like clone().
		* @param srcSceneNode The source scenenode to be cloned.
		*/
		Scenenode(const Scenenode &srcSceneNode);
		
		//! Destructor. Removes connections to other nodes, and decrements their reference counts.
		/**
		* The own reference count is not decreased, since it makes no sense in the destructor.
		*/
		virtual ~Scenenode();

		//! Returns the amount of connected input nodes.
		/**
		* @return Amount of connected input nodes
		*/
		unsigned long numInputNodes() const;
		//! Returns the amount of connected output nodes.
		/**
		* @return Amount of connected output nodes
		*/
		unsigned long numOutputNodes() const;

		//! Retrieves a pointer to one of the input nodes.
		/**
		* Node pointers are retrieved using a zero-based index. 0 is the first node.
		* Max value is numInputNodes()-1. Indices exceeding these values return a nullpointer.
		* @note This does not call addRef() on the retrieved node.
		* @param nr Index of the scenenode to be retrieved
		* @return Pointer to the scenenode, or 0 if the index was invalid.
		*/
		Scenenode* inputnode(const unsigned long nr) const;
		//! Retrieves a pointer to one of the output nodes.
		/**
		* Node pointers are retrieved using a zero-based index. 0 is the first node.
		* Max value is numOutputNodes()-1. Indices exceeding these values return a nullpointer.
		* @note This does not call addRef() on the retrieved node.
		* @param nr Index of the scenenode to be retrieved
		* @return Pointer to the scenenode, or 0 if the index was invalid.
		*/
		Scenenode* outputnode(const unsigned long nr) const;

		//! Tells whether the specified node is one of the connected input nodes or not.
		/**
		* @param rNode Node to be tested
		* @return true If the specified node is one of the connected input nodes, false otherwise.
		*/
		bool hasInputNode(Scenenode &rNode);
		//! Tells whether the specified node is one of the connected output nodes or not.
		/**
		* @param rNode Node to be tested
		* @return true If the specified node is one of the connected output nodes, false otherwise.
		*/
		bool hasOutputNode(Scenenode &rNode);

		struct Internaldata;
		Internaldata& internaldata();

	protected:
		Scenenode(const base::String& identifier);

		//! Adds an input node to this node.
		void addInputNode(Scenenode &rNode);
		//! Adds an output node to this node.
		void addOutputNode(Scenenode &rNode);

		//! Removes an input node from this node.
		void removeInputNode(Scenenode &rNode);
		//! Removes an output node from this node.
		void removeOutputNode(Scenenode &rNode);

		Internaldata* m_pInternaldata;
	};

}
}

#endif
