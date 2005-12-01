#include "../base/itypes.hh"
#include "../base/streamable.hh"
#include "vertexformat.hh"

namespace ovis {
namespace video {

	Vertexformat::Vertexformat():m_Stride(0),
	m_pVertexFormat(new std::vector <Entry>)
	{
	}

	Vertexformat::Vertexformat(const Vertexformat& src):m_Stride(0),m_pVertexFormat(0)
	{
		clone(src);
	}

	void Vertexformat::operator =(const Vertexformat& src)
	{
		clone(src);
	}

	void Vertexformat::clone(const Vertexformat& src)
	{
		m_Stride=src.stride();
		if (m_pVertexFormat) delete m_pVertexFormat;
		m_pVertexFormat=new std::vector <Entry>( *(src.m_pVertexFormat) );
	}

	Vertexformat::~Vertexformat()
	{
		delete m_pVertexFormat;
	}

	void Vertexformat::addEntry(const VertexFormatEntry entry,const ovis_uint32 semantic)
	{
		Entry newentry;
		newentry.m_Type=entry;
		newentry.m_Semantic=semantic;
		m_pVertexFormat->push_back(newentry);
		m_Stride=0;
	}

	VertexFormatEntry Vertexformat::entry(unsigned long index) const
	{
		return m_pVertexFormat->at(index).m_Type;
	}

	ovis_uint32 Vertexformat::entrySemantic(unsigned long index) const
	{
		return m_pVertexFormat->at(index).m_Semantic;
	}

	unsigned long Vertexformat::numEntries() const
	{
		return static_cast<unsigned long>(m_pVertexFormat->size());
	}

	unsigned long Vertexformat::stride() const
	{
		if (m_Stride!=0) return m_Stride;

		for (std::vector<Entry>::iterator itentry=m_pVertexFormat->begin();
		itentry!=m_pVertexFormat->end();++itentry) {
			m_Stride+=vertexFormatEntrySizeLookup((*itentry).m_Type);
		}

		return m_Stride;
	}

	bool Vertexformat::contains(const VertexFormatEntry entry,const unsigned long searchStart) const
	{
		if (searchStart>=m_pVertexFormat->size()) return false;

		for (std::vector<Entry>::iterator itentry=m_pVertexFormat->begin()+searchStart;
		itentry!=m_pVertexFormat->end();++itentry) {
			VertexFormatEntry candidate=(*itentry).m_Type;
			if (candidate==entry) return true;
		}
		
		return false;
	}

	bool Vertexformat::contains(const VertexFormatEntry entry,const ovis_uint32 semantic,const unsigned long searchStart) const
	{
		if (searchStart>=m_pVertexFormat->size()) return false;

		for (std::vector<Entry>::iterator itentry=m_pVertexFormat->begin()+searchStart;
		itentry!=m_pVertexFormat->end();++itentry) {
			VertexFormatEntry candidatetype=(*itentry).m_Type;
			ovis_uint32 candidatesemantic=(*itentry).m_Semantic;
			if ((candidatetype==entry) && (candidatesemantic==semantic)) return true;
		}
		
		return false;
	}

	unsigned long vertexFormatEntrySizeLookup(const VertexFormatEntry entry)
	{
		switch (entry) {
			case VertexFormatEntry_Position:return 3*sizeof(float);
			case VertexFormatEntry_Normal:return 3*sizeof(float);
			case VertexFormatEntry_Diffuse:return 1*sizeof(ovis_uint32);
			case VertexFormatEntry_Specular:return 1*sizeof(ovis_uint32);
			case VertexFormatEntry_Texcoord1D:return 1*sizeof(float);
			case VertexFormatEntry_Texcoord2D:return 2*sizeof(float);
			case VertexFormatEntry_Texcoord3D:return 3*sizeof(float);
			case VertexFormatEntry_Boneweight:return 0; // TODO: solve this
		}

		return 0;
	}

}
}
