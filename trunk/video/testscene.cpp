#include "../video/coreenums.hh"
#include "../video/renderer.hh"
#include "../video/scene.hh"

namespace ovis {
namespace video {

	Scene* generateTestscene(Renderer &rRenderer,Colorscale *pColorscale)
	{
		const float innerradius=1.4f;
		const float outerradius=0.5f;
		const unsigned long tesselation=60;

		Scene *pScene=new video::Scene(rRenderer);
		if (pColorscale!=0) pScene->colorscale(*pColorscale);

		unsigned long numVertices=(tesselation+1)*(tesselation+1);
		unsigned long numIndices=2*tesselation*tesselation*3;

		Vertexformat vf;
		vf.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_Position);
		vf.addEntry(VertexFormatEntry_Texcoord1D,VertexFormatSemantic_Factor);
		vf.addEntry(VertexFormatEntry_Normal,VertexFormatSemantic_Normal);

		pScene->allocGeometryStreams(vf,numVertices,numIndices);

		pScene->memVertexstream().map(Map_Writeonly);
		Vertexiterator v(pScene->memVertexstream());

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

				float f=(float)(rand())/(float)RAND_MAX;
				v.texcoord1D(0,f*f*f);

				math::Vector3f nn(cosT*cosP,sinT*cosP,sinP);

				v.normal(nn);

				++v;
			}
		}

		pScene->memVertexstream().unmap();

		pScene->memIndexstream().map(Map_Writeonly);
		Indexiterator idx(pScene->memIndexstream());

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

		for (ovis_uint32 attridx=0;attridx<pScene->attributebuffer().attributeAmount();++attridx) {
			pScene->attributebuffer()[attridx]=MATERIAL_VERTEXCOLOR;
		}

		pScene->memIndexstream().unmap();

		pScene->initGeometrystreams();

		return pScene;
	}

}
}
