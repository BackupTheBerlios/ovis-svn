#include "../base/itypes.hh"
#include "../base/localfile.hh"
#include "../base/log.hh"
#include "../video/memindexstream.hh"
#include "../video/memvertexstream.hh"
#include "loadscenefromfile.hh"
#include "scene.hh"

namespace ovis {
namespace video {

	/*struct Vertex
	{
		float m_X,m_Y,m_Z;
		double *m_pQuantities;
		
		Vertex():m_pQuantities(0) {}
		~Vertex() { if (m_pQuantities!=0) delete [] m_pQuantities; }
	};*/

	Scene* loadSceneFromFile(const base::String& filename,Renderer &rRenderer,Colorscale *pColorscale)
	{
		base::Localfile lf;
		bool opened=lf.open(filename,"rb");
		if (!opened) return 0;
		
		Vertexformat vf;
		vf.addEntry(VertexFormatEntry_Position,VertexFormatSemantic_Position);
		vf.addEntry(VertexFormatEntry_Texcoord1D,VertexFormatSemantic_Factor);
		vf.addEntry(VertexFormatEntry_Normal,VertexFormatSemantic_Normal);

		ovis_uint64 numTriangles,numVertices;
		ovis_uint32 numQuantitysets,numFaceattributearrays;
		double minQuantity,maxQuantity;
		
		lf >> numTriangles >> numVertices >> numQuantitysets >> numFaceattributearrays;
		lf >> minQuantity >> maxQuantity;
		
		base::logstream() << "Num triangles:             " << numTriangles << "\n";
		base::logstream() << "Num vertices:              " << numVertices << "\n";
		base::logstream() << "Num quantitysets:          " << numQuantitysets << "\n";
		base::logstream() << "Num face attribute arrays: " << numFaceattributearrays << "\n";
		base::logstream() << "Min quantity:              " << minQuantity << "\n";
		base::logstream() << "Max quantity:              " << maxQuantity << "\n";
		
		ovis_uint64 i;
		ovis_uint32 j;
		
		/*Vertex v;
		v.m_pQuantities=new double[numQuantitysets];*/
		
		Scene *pScene=new Scene(&rRenderer);
		if (pColorscale!=0) pScene->colorscale(*pColorscale);
		pScene->allocGeometryStreams(vf,numVertices,numTriangles*3);
		
		Vertexiterator vtx(pScene->memVertexstream());
		for (i=0;i<numVertices;++i) {
			float x,y,z;
			
			lf >> x >> y >> z;
			vtx.position(x,y,z);
			vtx.normal(1,0,0); // TODO:
			
			base::logstream() << "Vtx #" << i << " x " << x << " y " << y << " z " << z;
			
			double q;
			lf >> q;
			base::logstream() << " q " << q << "\n";
			q=(q-minQuantity)/(maxQuantity-minQuantity);
			vtx.texcoord1D(0,((float)q)); // TODO: change to float quantities & store additional quantities
			if (numQuantitysets>1) { for (j=1;j<numQuantitysets;++j) lf >> q; /* Temporary. Skips additional quantities.*/ }
			
			++vtx;
		}
		
		Indexiterator idx(pScene->memIndexstream());
		for (i=0;i<numTriangles;++i) {
			ovis_uint64 ii;

			lf >> ii; idx=ii; ++idx;
			lf >> ii; idx=ii; ++idx;
			lf >> ii; idx=ii; ++idx;
		}
		
		for (i=0;i<numTriangles;++i) {
			ovis_uint32 attr;
			
			lf >> attr; pScene->attributebuffer()[i]=attr;
		}

		lf.close();

		pScene->initGeometrystreams();

		return pScene;
	}

}
}
