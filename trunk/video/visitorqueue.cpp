#include "visitorqueue.hh"

namespace ovis {
namespace video {

	ScenenodeVisit::ScenenodeVisit(Scenenode &rScenenode):m_pScenenode(&rScenenode),
	m_pVertexstream(0),m_pIndexstream(0)
	{
	}

	ScenenodeVisit::ScenenodeVisit(const ScenenodeVisit &src)
	{
		clone(src);
	}


	void ScenenodeVisit::clone(const ScenenodeVisit &src)
	{
		m_pVertexstream=src.vertexstream();
		m_pIndexstream=src.indexstream();
		m_pScenenode=&(src.scenenode());

		m_Worldmatrix=src.worldmatrix();

		m_Callparams.m_PrimitivesType=src.callParams().m_PrimitivesType;
		m_Callparams.m_FirstElement=src.callParams().m_FirstElement;
		m_Callparams.m_NumElements=src.callParams().m_NumElements;
		m_Callparams.m_IndexOffset=src.callParams().m_IndexOffset;

		Propertytable::clone(src);
	}

	void ScenenodeVisit::vertexstream(Vertexstream* pStream)
	{
		m_pVertexstream=pStream;
	}

	Vertexstream* ScenenodeVisit::vertexstream() const
	{
		return m_pVertexstream;
	}

	void ScenenodeVisit::indexstream(Indexstream* pStream)
	{
		m_pIndexstream=pStream;
	}

	Indexstream* ScenenodeVisit::indexstream() const
	{
		return m_pIndexstream;
	}

	Visitorqueue::Visitorqueue()
	{
	}

	Visitorqueue::~Visitorqueue()
	{
		std::vector<ScenenodeVisit*>::iterator itvisit=m_Visits.begin();
		for (;itvisit!=m_Visits.end();++itvisit) {
			delete (*itvisit);
		}
	}

	void Visitorqueue::addVisit(ScenenodeVisit* pVisit)
	{
		m_Visits.push_back(pVisit);
	}

}
}
