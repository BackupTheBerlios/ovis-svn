#include <algorithm>
#include <assert.h>
#include <vector>
#include "coreenums.hh"
#include "scene.hh"
#include <iostream>

namespace ovis {
namespace video {



	struct Scene::Materiallist
	{
		std::vector< Material* > m_Materials;
	};



	Scene::Scene(Renderer &rRenderer):m_pMemIndexstream(0),m_pMemVertexstream(0),m_pIndexstream(0),m_pVertexstream(0),
		m_pRenderer(&rRenderer),m_pCamera(0),m_pColorscale(0),m_pMateriallist(new Materiallist)
	{
	}

	Scene::~Scene()
	{
		if (m_pIndexstream) delete m_pIndexstream;
		if (m_pVertexstream) delete m_pVertexstream;
		if (m_pMemIndexstream) delete m_pMemIndexstream;
		if (m_pMemVertexstream) delete m_pMemVertexstream;

		std::vector< Material* >::iterator itmat=m_pMateriallist->m_Materials.begin();
		for (;itmat!=m_pMateriallist->m_Materials.end();++itmat) delete (*itmat);
		delete m_pMateriallist;
	}


	void Scene::allocGeometryStreams(const Vertexformat& format,const ovis_uint32 numVertices,const ovis_uint32 numIndices)
	{
		if (m_pIndexstream) delete m_pIndexstream; m_pIndexstream=0;
		if (m_pVertexstream) delete m_pVertexstream; m_pVertexstream=0;
		if (m_pMemIndexstream) delete m_pMemIndexstream;
		if (m_pMemVertexstream) delete m_pMemVertexstream;

		m_pMemIndexstream=new MemIndexstream(numIndices);
		m_pMemVertexstream=new MemVertexstream(format,numVertices);

		m_Attributebuffer.attributeAmount(numIndices/3);
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

	const Attributebuffer& Scene::attributebuffer() const
	{
		return m_Attributebuffer;
	}

	Attributebuffer& Scene::attributebuffer()
	{
		return m_Attributebuffer;
	}

	const Attributetable& Scene::attributetable() const
	{
		return m_Attributetable;
	}

	Attributetable& Scene::attributetable()
	{
		return m_Attributetable;
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
	
	const Camera* Scene::camera() const
	{
		return m_pCamera;
	}
	
	void Scene::camera(Camera &rCamera)
	{
		m_pCamera=&rCamera;
	}

	const Colorscale* Scene::colorscale() const
	{
		return m_pColorscale;
	}

	void Scene::colorscale(Colorscale &rColorscale)
	{
		m_pColorscale=&rColorscale;
	}




	void Scene::addMaterial(const Material& m)
	{
		Material *pM=new Material(m);
		m_pMateriallist->m_Materials.push_back(pM);
	}

	void Scene::removeMaterial(const Material* m)
	{
		std::vector< Material* >::iterator itmat=
			std::find(m_pMateriallist->m_Materials.begin(),m_pMateriallist->m_Materials.end(),m);

		delete (*itmat);
		m_pMateriallist->m_Materials.erase(itmat);
	}

	unsigned long Scene::numMaterials() const
	{
		return (unsigned long)(m_pMateriallist->m_Materials.size());
	}

	Material* Scene::material(const unsigned long index)
	{
		return (index<numMaterials()) ? m_pMateriallist->m_Materials[index] : 0;
	}

	const Material* Scene::material(const unsigned long index) const
	{
		return (index<numMaterials()) ? m_pMateriallist->m_Materials[index] : 0;
	}





	bool Scene::isValid() const
	{
		if (!m_pRenderer || !m_pMemVertexstream || !m_pMemIndexstream || !m_pIndexstream || !m_pVertexstream || !m_pCamera)
			return false;
		else return m_pRenderer->isValid() && m_pIndexstream->isValid() && m_pVertexstream->isValid();
	}


	void Scene::render()
	{
		if (!m_pRenderer || !m_pMemVertexstream || !m_pMemIndexstream || !m_pCamera) return;
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



		m_Attributebuffer.reorganize(m_pMemIndexstream);
		m_Attributetable.recalculate(m_Attributebuffer);




		m_pIndexstream=m_pRenderer->createIndexstream(
			m_pMemIndexstream->capacity(),
			Indexformat_32bit,
			Streamflags_Writeonly
			);

		m_pIndexstream->map(Map_Writeonly);
		m_pIndexstream->transferFrom(*m_pMemIndexstream);
		m_pIndexstream->unmap();



		if (m_pMemVertexstream->vertexformat().contains(VertexFormatEntry_Texcoord1D,VertexFormatSemantic_Factor,0) && (m_pColorscale!=0)) {

			ovis_uint32 numVertices=m_pMemVertexstream->capacity();
			const Vertexformat &format=m_pMemVertexstream->vertexformat();
			Vertexformat newformat;
			for (ovis_uint32 eidx=0;eidx<format.numEntries();++eidx) {

				// If the factor entry is present, and no diffuse entry is there, create one
				if ((format.entry(eidx)==VertexFormatEntry_Texcoord1D) && (format.entrySemantic(eidx)==VertexFormatSemantic_Factor)) {
					if (!format.contains(VertexFormatEntry_Diffuse,0))
						newformat.addEntry(VertexFormatEntry_Diffuse,VertexFormatSemantic_None);
				}
				else newformat.addEntry(format.entry(eidx),format.entrySemantic(eidx));
			}
			MemVertexstream *pMVtx=new MemVertexstream(newformat,numVertices);
			pMVtx->transferFrom(*m_pMemVertexstream);

			// Calculate the colors using the color scale and the factors from m_pMemVertexstream
			for (ovis_uint32 vtxidx=0;vtxidx<numVertices;++vtxidx) {
				float f=m_pMemVertexstream->texcoord1D(vtxidx,0);
				ovis_uint8 a,b,g,r;
				m_pColorscale->lerpColor(f,a,b,g,r);
				pMVtx->diffuseColor(vtxidx,a,r,g,b);
			}

			delete m_pMemVertexstream;
			m_pMemVertexstream=pMVtx;
		}

		m_pVertexstream=m_pRenderer->createVertexstream(
			m_pMemVertexstream->capacity(),
			m_pMemVertexstream->vertexformat(),
			Streamflags_Writeonly
			);

		m_pVertexstream->map(Map_Writeonly);
		m_pVertexstream->transferFrom(*m_pMemVertexstream);
		m_pVertexstream->unmap();
	}


}
}
