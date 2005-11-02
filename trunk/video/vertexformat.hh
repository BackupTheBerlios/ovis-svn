#ifndef OVIS_VIDEO_VERTEXFORMAT_HH_INCLUDED
#define OVIS_VIDEO_VERTEXFORMAT_HH_INCLUDED

#include "../base/dll.hh"
#include <vector>

namespace ovis {
namespace video {

	/*
	NOTE: The vertex format entry order in the Vertexformat class isn't strictly followed.
	For example, a normal vector can actually be located behind the texture coords, even if it is before
	these in the Vertexformat class.
	*/

	enum VertexFormatEntry {
		VertexFormatEntry_Position=0,
		VertexFormatEntry_Normal=1,
		VertexFormatEntry_Diffuse=2,
		VertexFormatEntry_Specular=3,
		VertexFormatEntry_Texcoord1D=4,
		VertexFormatEntry_Texcoord2D=5,
		VertexFormatEntry_Texcoord3D=6,
		VertexFormatEntry_Boneweight=7
	};

	enum VertexFormatSemantic {
		VertexFormatSemantic_None=0,
		VertexFormatSemantic_Position=1,
		VertexFormatSemantic_Tangent=2,
		VertexFormatSemantic_Binormal=3,
		VertexFormatSemantic_Normal=4,
		VertexFormatSemantic_Texcoord=5,
		VertexFormatSemantic_SHCoefficients=6
	};

	OVIS_API unsigned long vertexFormatEntrySizeLookup(const VertexFormatEntry entry);

	class OVIS_API Vertexformat
	{
	public:
		//const std::vector <VertexFormatEntry>& container() const { return *m_pVertexFormat; }

		void addEntry(const VertexFormatEntry entry,const ovis_uint32 semantic);
		VertexFormatEntry entry(unsigned long index) const;
		ovis_uint32 entrySemantic(unsigned long index) const;
		unsigned long numEntries() const;

		Vertexformat();
		Vertexformat(const Vertexformat& src);
		virtual ~Vertexformat();

		void operator =(const Vertexformat& src);
		void clone(const Vertexformat& src);

		bool contains(const VertexFormatEntry entry,const unsigned long searchStart);

		unsigned long stride() const;

	private:
		struct Entry {
			VertexFormatEntry m_Type;
			ovis_uint32 m_Semantic;
		};

		mutable unsigned long m_Stride;
		std::vector <Entry> *m_pVertexFormat;
	};

}
}

#endif
