#include "../base/datablock.hh"
#include "vertexstream.hh"

namespace ovis {
namespace video {

	Vertexstream::Vertexstream(const Vertexformat& format,const ovis_uint32 _capacity,Vertexsource *pSource):
	Geometrystream(_capacity),m_Format(format),m_pSource(pSource),
		m_pp1DTexcoords(0),m_pp2DTexcoords(0),m_pp3DTexcoords(0)
	{
		initTexcoords();
	}

	Vertexstream::~Vertexstream()
	{
		if (m_pp1DTexcoords) delete [] m_pp1DTexcoords;
		if (m_pp2DTexcoords) delete [] m_pp2DTexcoords;
		if (m_pp3DTexcoords) delete [] m_pp3DTexcoords;
	}

	const Vertexformat& Vertexstream::vertexformat() const
	{
		return m_Format;
	}

	void Vertexstream::transferFrom(const Vertexstream& from,const ovis_uint32 numVertices)
	{
		Vertexiterator vfrom((Vertexstream&)from),vto(*this);

		Vertexformat usedformat;

		bool *usedEntries=new bool[from.vertexformat().numEntries()];
		ovis_uint32 entrynr;
		for (entrynr=0;entrynr<from.vertexformat().numEntries();++entrynr) usedEntries[entrynr]=false;

		for (entrynr=0;entrynr<vertexformat().numEntries();++entrynr) {
			for (ovis_uint32 entrynr2=0;entrynr2<from.vertexformat().numEntries();++entrynr2) {
				if (usedEntries[entrynr2]) continue;

				if (from.vertexformat().entry(entrynr2)==vertexformat().entry(entrynr)) {
					usedformat.addEntry(from.vertexformat().entry(entrynr2),from.vertexformat().entrySemantic(entrynr2));
					usedEntries[entrynr2]=true;
				}
			}
		}

		delete [] usedEntries;

		for (ovis_uint32 vtx=0;vtx<numVertices;++vtx) {
			ovis_uint32 numtexcoords[3]={0,0,0};
			for (entrynr=0;entrynr<usedformat.numEntries();++entrynr) {
				int texcoordD=-1;
				switch (usedformat.entry(entrynr)) {
					case VertexFormatEntry_Position:vto.position(vfrom.position()); break;
					case VertexFormatEntry_Normal:vto.normal(vfrom.normal()); break;
					case VertexFormatEntry_Diffuse:vto.diffuseColor(vfrom.diffuseColor()); break;
					case VertexFormatEntry_Specular:vto.specularColor(vfrom.specularColor()); break;
					case VertexFormatEntry_Texcoord1D:
						texcoordD=0;
						vto.texcoord1D(numtexcoords[texcoordD],vfrom.texcoord1D(numtexcoords[texcoordD]));
						//*(vto.m_pp1DTexcoords[numtexcoords[texcoordD]])=*(vfrom.m_pp1DTexcoords[numtexcoords[texcoordD]]);
					case VertexFormatEntry_Texcoord2D:if (texcoordD<0) {
						texcoordD=1;
						vto.texcoord2D(numtexcoords[texcoordD],vfrom.texcoord2D(numtexcoords[texcoordD]));
						//*(vto.m_pp2DTexcoords[numtexcoords[texcoordD]])=*(vfrom.m_pp2DTexcoords[numtexcoords[texcoordD]]);
					}
					case VertexFormatEntry_Texcoord3D:if (texcoordD<0) {
						texcoordD=2;
						vto.texcoord3D(numtexcoords[texcoordD],vfrom.texcoord3D(numtexcoords[texcoordD]));
						//*(vto.m_pp3DTexcoords[numtexcoords[texcoordD]])=*(vfrom.m_pp3DTexcoords[numtexcoords[texcoordD]]);
					}
					++numtexcoords[texcoordD];
					break;
					case VertexFormatEntry_Boneweight:break; // TODO: solve this
				}
			}

			++vfrom;
			++vto;
		}
	}

	void Vertexstream::initTexcoords()
	{
		if (m_pp1DTexcoords) delete [] m_pp1DTexcoords;
		if (m_pp2DTexcoords) delete [] m_pp2DTexcoords;
		if (m_pp3DTexcoords) delete [] m_pp3DTexcoords;

		m_Num1DTexcoords=0;
		m_Num2DTexcoords=0;
		m_Num3DTexcoords=0;

		for (ovis_uint32 entrynr=0;entrynr<vertexformat().numEntries();++entrynr) {
			switch (vertexformat().entry(entrynr)) {
				case VertexFormatEntry_Texcoord1D:++m_Num1DTexcoords; break;
				case VertexFormatEntry_Texcoord2D:++m_Num2DTexcoords; break;
				case VertexFormatEntry_Texcoord3D:++m_Num3DTexcoords; break;
				default:break;
			};
		}

		if (m_Num1DTexcoords) { m_pp1DTexcoords=new float*[m_Num1DTexcoords]; }
		if (m_Num2DTexcoords) { m_pp2DTexcoords=new math::Vector2f*[m_Num2DTexcoords]; }
		if (m_Num3DTexcoords) { m_pp3DTexcoords=new math::Vector3f*[m_Num3DTexcoords]; }
	}

	ovis_uint32 Vertexstream::num1DTexcoords() const { return m_Num1DTexcoords; }
	ovis_uint32 Vertexstream::num2DTexcoords() const { return m_Num2DTexcoords; }
	ovis_uint32 Vertexstream::num3DTexcoords() const { return m_Num3DTexcoords; }

}
}
