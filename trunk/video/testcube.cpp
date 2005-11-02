#include "camera.hh"
#include "indexiterator.hh"
#include "indexstream.hh"
#include "vertexiterator.hh"
#include "vertexstream.hh"
#include "videodevice.hh"
#include "testcube.hh"
#include "visitorqueue.hh"

namespace ovis {
namespace video {

	const char* Testcube::type() const { return "Testcube"; }

	Testcube::Testcube(Videodevice& videodevice,const base::String& identifier,
		const float width,const float height,const float depth):Transformnode(identifier),m_pTexture2D(0)
	{
		Vertexformat vf;
		vf.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_Position);
		vf.addEntry(VertexFormatEntry_Texcoord2D,VertexFormatSemantic_Texcoord);
		vf.addEntry(VertexFormatEntry_Normal,VertexFormatSemantic_Normal);

		m_pVertexstream=videodevice.createVertexstream(8,vf,Streamflags_Writeonly,Mempool_Managed);
		m_pIndexstream=videodevice.createIndexstream(6*2*3,Indexformat_16bit,Streamflags_Writeonly,Mempool_Managed);

		m_pVertexstream->map(Map_Writeonly);
		Vertexiterator v(*m_pVertexstream);

		v.position(-width,-height,-depth);	v.normal(math::Vector3f(-1,-1,-1).normalize());	v.texcoord2D(0,	0,0);	v++;
		v.position( width,-height,-depth);	v.normal(math::Vector3f( 1,-1,-1).normalize());	v.texcoord2D(0,	1,0);	v++;
		v.position(-width, height,-depth);	v.normal(math::Vector3f(-1, 1,-1).normalize());	v.texcoord2D(0,	0,1);	v++;
		v.position( width, height,-depth);	v.normal(math::Vector3f( 1, 1,-1).normalize());	v.texcoord2D(0,	1,1);	v++;

		v.position(-width,-height, depth);	v.normal(math::Vector3f(-1,-1, 1).normalize());	v.texcoord2D(0,	1,1);	v++;
		v.position( width,-height, depth);	v.normal(math::Vector3f( 1,-1, 1).normalize());	v.texcoord2D(0,	1,0);	v++;
		v.position(-width, height, depth);	v.normal(math::Vector3f(-1, 1, 1).normalize());	v.texcoord2D(0,	0,1);	v++;
		v.position( width, height, depth);	v.normal(math::Vector3f( 1, 1, 1).normalize());	v.texcoord2D(0,	1,1);

		m_pVertexstream->unmap();

		static const ovis_uint32 indices[12][3]={
			{0,2,3},
			{3,1,0},
			{4,5,7},
			{7,6,4},
			{0,1,5},
			{5,4,0},
			{1,3,7},
			{7,5,1},
			{3,2,6},
			{6,7,3},
			{2,0,4},
			{4,6,2}
		};

		m_pIndexstream->map(Map_Writeonly);
		Indexiterator i(*m_pIndexstream);
		for (int nr=0;nr<36;++nr) {
			i=indices[nr/3][nr%3];
			i++;
		}
		m_pIndexstream->unmap();

	}

	Testcube::~Testcube()
	{
		delete m_pIndexstream;
		delete m_pVertexstream;
	}

	void Testcube::onVisit(Visitcontext& visitcontext)
	{
		ScenenodeVisit *pVisit=new ScenenodeVisit(*this);
		if (m_pTexture2D) pVisit->add2DTexture("diffuseTexture",m_pTexture2D);
		pVisit->vertexstream(this->m_pVertexstream);
		pVisit->indexstream(this->m_pIndexstream);
		pVisit->callParams().m_FirstElement=0;
		pVisit->callParams().m_NumElements=12;
		pVisit->callParams().m_IndexOffset=0;
		pVisit->callParams().m_PrimitivesType=Primitives_Triangles;
		pVisit->worldmatrix(worldmatrix());
		visitcontext.visitorqueue().addVisit(pVisit);
	}

	bool Testcube::isValid() const
	{
		return true;
	}

	void Testcube::texture2d(Texture2D* pTex)
	{
		m_pTexture2D=pTex;
	}

}
}
