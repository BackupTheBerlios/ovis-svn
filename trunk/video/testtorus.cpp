#include "camera.hh"
#include "indexiterator.hh"
#include "indexstream.hh"
#include "vertexiterator.hh"
#include "vertexstream.hh"
#include "renderer.hh"
#include "visitorqueue.hh"
#include "testtorus.hh"
#include <math.h>

namespace ovis {
namespace video {

	const char* Testtorus::type() const { return "Testtorus"; }

	Testtorus::Testtorus(Renderer& renderer,const base::String& identifier,
		const float innerradius,const float outerradius,const unsigned long tesselation):Transformnode(identifier),
		m_pTexture2D(0)
	{
		unsigned long numVertices=(tesselation+1)*(tesselation+1);
		unsigned long numIndices=2*tesselation*tesselation*3;

		Vertexformat vf;
		vf.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_Position);
		vf.addEntry(VertexFormatEntry_Texcoord2D,VertexFormatSemantic_Texcoord);
		vf.addEntry(VertexFormatEntry_Normal,VertexFormatSemantic_Normal);

		m_pVertexstream=renderer.createVertexstream(numVertices,vf,Streamflags_Writeonly);
		m_pIndexstream=renderer.createIndexstream(numIndices,Indexformat_16bit,Streamflags_Writeonly);

		m_pVertexstream->map(Map_Writeonly);
		Vertexiterator v(*m_pVertexstream);

		unsigned long ring;
	
		for (ring=0;ring<(tesselation+1);++ring) {
			float tpos=(float(ring)/float(tesselation));
			float t=2.0f*3.1415926535f*tpos;
			float sinT=sinf(t);
			float cosT=cosf(t);
			for (unsigned long i=0;i<(tesselation+1);++i) {
				float ppos=(float(i)/float(tesselation));
				float p=2.0f*3.1415926535f*ppos;
				float sinP=sinf(p);
				float cosP=cosf(p);
				v.position(
					(innerradius+outerradius*cosP)*cosT,
					(innerradius+outerradius*cosP)*sinT,
					outerradius*sinP
				);

				v.texcoord2D(0,2.0f*tpos,ppos);

				math::Vector3f nn(cosT*cosP,sinT*cosP,sinP);

				v.normal(nn);

				++v;
			}
		}

		m_pVertexstream->unmap();

		m_pIndexstream->map(Map_Writeonly);
		Indexiterator idx(*m_pIndexstream);

		for (ring=0;ring<tesselation;++ring) {
			for (unsigned long i=0;i<tesselation;++i) {
				idx.positionInStream(((ring*tesselation+i)*2)*3+0); idx=ring*(tesselation+1)+i;
				idx.positionInStream(((ring*tesselation+i)*2)*3+1); idx=(ring+1)*(tesselation+1)+i;
				idx.positionInStream(((ring*tesselation+i)*2)*3+2); idx=ring*(tesselation+1)+i+1;
				idx.positionInStream(((ring*tesselation+i)*2+1)*3+0); idx=ring*(tesselation+1)+i+1;
				idx.positionInStream(((ring*tesselation+i)*2+1)*3+1); idx=(ring+1)*(tesselation+1)+i;
				idx.positionInStream(((ring*tesselation+i)*2+1)*3+2); idx=(ring+1)*(tesselation+1)+i+1;
			}
		}


		m_pIndexstream->unmap();
	}

	Testtorus::~Testtorus()
	{
		delete m_pIndexstream;
		delete m_pVertexstream;
	}

	void Testtorus::onVisit(Visitcontext& visitcontext)
	{
		ScenenodeVisit *pVisit=new ScenenodeVisit(*this);
		if (m_pTexture2D) pVisit->add2DTexture("diffuseTexture",m_pTexture2D);
		pVisit->vertexstream(this->m_pVertexstream);
		
		
		pVisit->indexstream(this->m_pIndexstream);
		pVisit->callParams().m_FirstElement=0;
		pVisit->callParams().m_NumElements=m_pIndexstream->capacity()/3;
		pVisit->callParams().m_IndexOffset=0;
		pVisit->callParams().m_PrimitivesType=Primitives_Triangles;
		pVisit->worldmatrix(worldmatrix());
		
		visitcontext.visitorqueue().addVisit(pVisit);
		/*pVisit->indexstream(0);
		pVisit->callParams().m_FirstElement=0;
		pVisit->callParams().m_NumElements=m_pVertexstream->capacity();
		pVisit->callParams().m_IndexOffset=0;
		pVisit->callParams().m_PrimitivesType=Primitives_Points;
		visitorqueue.addVisit(pVisit);*/
	}

	bool Testtorus::isValid() const
	{
		return true;
	}

	void Testtorus::texture2d(Texture2D* pTex)
	{
		m_pTexture2D=pTex;
	}

}
}
