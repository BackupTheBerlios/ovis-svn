#include <algorithm>
#include <vector>

#include "camera.hh"
#include "renderer.hh"
#include "scenenode.hh"

namespace ovis {
namespace video {

	Visitcontext::Visitcontext(const Camera &rCamera,Visitorqueue &rVisitorqueue,Renderer &rRenderer):
	m_rCamera(rCamera),m_rVisitorqueue(rVisitorqueue),m_rRenderer(rRenderer)
	{
	}

	const Camera& Visitcontext::camera()
	{
		return m_rCamera;
	}

	Visitorqueue& Visitcontext::visitorqueue()
	{
		return m_rVisitorqueue;
	}

	Renderer& Visitcontext::renderer()
	{
		return m_rRenderer;
	}







	struct Scenenode::Internaldata
	{
		std::vector<Scenenode*> m_InputNodes,m_OutputNodes;
	};


	Scenenode::Internaldata& Scenenode::internaldata() { return *m_pInternaldata; }

	Scenenode::Scenenode(const base::String& identifier):base::RefCounted(identifier),
	m_pInternaldata(new Internaldata)
	{
	}

	Scenenode::Scenenode(const Scenenode &srcSceneNode):base::RefCounted(srcSceneNode.objIdentifier()),
	m_pInternaldata(new Internaldata){ clone(srcSceneNode); }

	Scenenode::~Scenenode()
	{
		std::vector<Scenenode*>::iterator it,it2;

		for (it=m_pInternaldata->m_InputNodes.begin();it!=m_pInternaldata->m_InputNodes.end();++it) {
			std::vector<Scenenode*> &nodelist=(*it)->internaldata().m_OutputNodes;
			it2=std::find(nodelist.begin(),nodelist.end(),this);
			if (it2!=nodelist.end()) nodelist.erase(it2);
			(*it)->release();
		}

		for (it=m_pInternaldata->m_OutputNodes.begin();it!=m_pInternaldata->m_OutputNodes.end();++it) {
			std::vector<Scenenode*> &nodelist=(*it)->internaldata().m_InputNodes;
			it2=std::find(nodelist.begin(),nodelist.end(),this);
			if (it2!=nodelist.end()) nodelist.erase(it2);
			(*it)->release();
		}

		delete m_pInternaldata;
	}

	void Scenenode::clone(const Scenenode &srcSceneNode)
	{
		unsigned long nr;
		for (nr=0;nr<srcSceneNode.numInputNodes();++nr)
			addInputNode(*(srcSceneNode.inputnode(nr)));
		for (nr=0;nr<srcSceneNode.numOutputNodes();++nr)
			addOutputNode(*(srcSceneNode.outputnode(nr)));
	}

	Scenenode& Scenenode::operator =(const Scenenode& src) { clone(src); return *this; }

	unsigned long Scenenode::numInputNodes() const
	{
		return (unsigned long)(m_pInternaldata->m_InputNodes.size());
	}

	unsigned long Scenenode::numOutputNodes() const
	{
		return (unsigned long)(m_pInternaldata->m_OutputNodes.size());
	}

	Scenenode* Scenenode::inputnode(const unsigned long nr) const
	{
		return m_pInternaldata->m_InputNodes[nr];
	}

	Scenenode* Scenenode::outputnode(const unsigned long nr) const
	{
		return m_pInternaldata->m_OutputNodes[nr];
	}

	bool Scenenode::hasInputNode(Scenenode &rNode)
	{
		std::vector<Scenenode*>::iterator it=
			std::find(m_pInternaldata->m_InputNodes.begin(),m_pInternaldata->m_InputNodes.end(), &rNode);
		return (it!=m_pInternaldata->m_InputNodes.end());
	}

	bool Scenenode::hasOutputNode(Scenenode &rNode)
	{
		std::vector<Scenenode*>::iterator it=
			std::find(m_pInternaldata->m_OutputNodes.begin(),m_pInternaldata->m_OutputNodes.end(), &rNode);
		return (it!=m_pInternaldata->m_OutputNodes.end());
	}

	void Scenenode::addInputNode(Scenenode &rNode)
	{
		std::vector<Scenenode*>::iterator it=
			std::find(m_pInternaldata->m_InputNodes.begin(),m_pInternaldata->m_InputNodes.end(), &rNode);

		if (it==m_pInternaldata->m_InputNodes.end()) {
			m_pInternaldata->m_InputNodes.push_back( &rNode );
			rNode.internaldata().m_OutputNodes.push_back( this );

			rNode.addRef();
			addRef();
		}
	}

	void Scenenode::addOutputNode(Scenenode &rNode)
	{
		std::vector<Scenenode*>::iterator it=
			std::find(m_pInternaldata->m_OutputNodes.begin(),m_pInternaldata->m_OutputNodes.end(), &rNode);

		if (it==m_pInternaldata->m_OutputNodes.end()) {
			m_pInternaldata->m_OutputNodes.push_back( &rNode );
			rNode.internaldata().m_InputNodes.push_back( this );

			rNode.addRef();
			addRef();
		}
	}

	void Scenenode::removeInputNode(Scenenode &rNode)
	{
		std::vector<Scenenode*>::iterator it=
			std::find(m_pInternaldata->m_InputNodes.begin(),m_pInternaldata->m_InputNodes.end(), &rNode);

		if (it!=m_pInternaldata->m_InputNodes.end()) {
			m_pInternaldata->m_InputNodes.erase(it);
			it=std::find(rNode.internaldata().m_OutputNodes.begin(),rNode.internaldata().m_OutputNodes.end(), this);
			if (it!=rNode.internaldata().m_OutputNodes.end())
				rNode.internaldata().m_OutputNodes.erase(it);

			rNode.release();
			release();
		}
	}

	void Scenenode::removeOutputNode(Scenenode &rNode)
	{
		std::vector<Scenenode*>::iterator it=
			std::find(m_pInternaldata->m_OutputNodes.begin(),m_pInternaldata->m_OutputNodes.end(), &rNode);

		if (it!=m_pInternaldata->m_OutputNodes.end()) {
			m_pInternaldata->m_OutputNodes.erase(it);
			it=std::find(rNode.internaldata().m_InputNodes.begin(),rNode.internaldata().m_InputNodes.end(), this);
			if (it!=rNode.internaldata().m_InputNodes.end())
				rNode.internaldata().m_InputNodes.erase(it);

			rNode.release();
			release();
		}
	}




}
}
