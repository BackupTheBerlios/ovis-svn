#include <assert.h>
#include "coreenums.hh"
#include "scene.hh"

namespace ovis {
namespace video {

	Scene::Scene(Renderer &rRenderer):m_pMemIndexstream(0),m_pMemVertexstream(0),m_pIndexstream(0),m_pVertexstream(0),
		m_pRenderer(&rRenderer)
	{
	}

	Scene::~Scene()
	{
		if (m_pIndexstream) delete m_pIndexstream;
		if (m_pVertexstream) delete m_pVertexstream;
		if (m_pMemIndexstream) delete m_pMemIndexstream;
		if (m_pMemVertexstream) delete m_pMemVertexstream;
	}


	void Scene::allocGeometryStreams(const Vertexformat& format,const ovis_uint32 numVertices,const ovis_uint32 numIndices)
	{
		if (m_pIndexstream) delete m_pIndexstream; m_pIndexstream=0;
		if (m_pVertexstream) delete m_pVertexstream; m_pVertexstream=0;
		if (m_pMemIndexstream) delete m_pMemIndexstream;
		if (m_pMemVertexstream) delete m_pMemVertexstream;

		m_pMemIndexstream=new MemIndexstream(numIndices);
		m_pMemVertexstream=new MemVertexstream(format,numVertices);
	}


	const MemIndexstream& Scene::memIndexstream() const
	{
		return *m_pMemIndexstream;
	}

	MemIndexstream& Scene::memIndexstream()
	{
		return *m_pMemIndexstream;
	}


	const MemVertexstream& Scene::memVertexstream() const
	{
		return *m_pMemVertexstream;
	}

	MemVertexstream& Scene::memVertexstream()
	{
		return *m_pMemVertexstream;
	}


	const Indexstream* Scene::indexstream() const
	{
		return m_pIndexstream;
	}

	Indexstream* Scene::indexstream()
	{
		return m_pIndexstream;
	}


	const Vertexstream* Scene::vertexstream() const
	{
		return m_pVertexstream;
	}

	Vertexstream* Scene::vertexstream()
	{
		return m_pVertexstream;
	}


	const Renderer& Scene::renderer() const
	{
		return *m_pRenderer;
	}

	void Scene::renderer(Renderer &rRenderer)
	{
		m_pRenderer=&rRenderer;
	}
	
	const Camera& Scene::camera() const
	{
		return m_Camera;
	}
	
	Camera& Scene::camera()
	{
		return m_Camera;
	}

	


	bool Scene::isValid() const
	{
		if (!m_pRenderer || !m_pMemVertexstream || !m_pMemIndexstream || !m_pIndexstream || !m_pVertexstream) return false;
		else return m_pRenderer->isValid() && m_pIndexstream->isValid() && m_pVertexstream->isValid();
	}


	void Scene::render()
	{
		if (!m_pIndexstream || !m_pVertexstream) initGeometrystreams();

		m_pRenderer->render(*this);
	}

	void Scene::initGeometrystreams()
	{
		// If one of these is zero, either allocGeometrystreams() hasn't been called or something went wrong
		assert(m_pMemVertexstream && m_pMemIndexstream);

		if (m_pIndexstream) delete m_pIndexstream; m_pIndexstream=0;
		if (m_pVertexstream) delete m_pVertexstream; m_pVertexstream=0;

		if ((m_pMemIndexstream->capacity()==0) || (m_pMemVertexstream->capacity()==0)) return;

		m_pIndexstream=m_pRenderer->createIndexstream(
			m_pMemIndexstream->capacity(),
			Indexformat_32bit,
			Streamflags_Writeonly
			);

		m_pVertexstream=m_pRenderer->createVertexstream(
			m_pMemVertexstream->capacity(),
			m_pMemVertexstream->vertexformat(),
			Streamflags_Writeonly
			);

		m_pIndexstream->map(Map_Writeonly);
		m_pIndexstream->transferFrom(*m_pMemIndexstream);
		m_pIndexstream->unmap();
		m_pVertexstream->map(Map_Writeonly);
		m_pVertexstream->transferFrom(*m_pMemVertexstream);
		m_pVertexstream->unmap();
	}


}
}
