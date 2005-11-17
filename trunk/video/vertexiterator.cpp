#include <assert.h>
#include "vertexiterator.hh"
#include "vertexstream.hh"

namespace ovis {
namespace video {

	Vertexiterator::Vertexiterator(Vertexstream& rVertexstream,const ovis_uint32 position):
	m_rVertexstream(rVertexstream),m_Position(position)
	{
	}

	Vertexiterator::Vertexiterator(Vertexstream &rVertexstream):
	m_rVertexstream(rVertexstream),m_Position(0)
	{
	}

	Vertexiterator::~Vertexiterator()
	{
	}

	const Vertexstream& Vertexiterator::vertexstream() const
	{
		return m_rVertexstream;
	}

	void Vertexiterator::positionInStream(const ovis_uint32 newPosition)
	{
		m_Position=newPosition;
		if (m_Position>=m_rVertexstream.capacity()) m_Position=m_rVertexstream.capacity();
	}

	ovis_uint32 Vertexiterator::positionInStream() const
	{
		return m_Position;
	}

	Vertexiterator& Vertexiterator::operator ++()
	{
		++m_Position;
		if (m_Position>=m_rVertexstream.capacity()) m_Position=m_rVertexstream.capacity();

		return *this;
	}

	Vertexiterator& Vertexiterator::operator --()
	{
		--m_Position;
		if (m_Position>=m_rVertexstream.capacity()) m_Position=m_rVertexstream.capacity();

		return *this;
	}

	Vertexiterator& Vertexiterator::operator ++(const int)
	{
		++m_Position;
		if (m_Position>=m_rVertexstream.capacity()) m_Position=m_rVertexstream.capacity();

		return *this;
	}

	Vertexiterator& Vertexiterator::operator --(const int)
	{
		--m_Position;
		if (m_Position>=m_rVertexstream.capacity()) m_Position=m_rVertexstream.capacity();

		return *this;
	}

	Vertexiterator& Vertexiterator::operator +=(const int offset)
	{
		m_Position+=offset;;
		if (m_Position>=m_rVertexstream.capacity()) m_Position=m_rVertexstream.capacity();

		return *this;
	}

	Vertexiterator& Vertexiterator::operator -=(const int offset)
	{
		m_Position-=offset;;
		if (m_Position>=m_rVertexstream.capacity()) m_Position=m_rVertexstream.capacity();

		return *this;
	}

	const math::Vector3f& Vertexiterator::position() const
	{
		return m_rVertexstream.position(m_Position);
	}

	const math::Vector3f& Vertexiterator::normal() const
	{
		return m_rVertexstream.normal(m_Position);
	}

	ovis_uint32 Vertexiterator::diffuseColor() const
	{
		return m_rVertexstream.diffuseColor(m_Position);
	}

	ovis_uint32 Vertexiterator::specularColor() const
	{
		return m_rVertexstream.specularColor(m_Position);
	}

	float Vertexiterator::texcoord1D(const ovis_uint32 texstage) const
	{
		return m_rVertexstream.texcoord1D(m_Position,texstage);
	}

	const math::Vector2f& Vertexiterator::texcoord2D(const ovis_uint32 texstage) const
	{
		return m_rVertexstream.texcoord2D(m_Position,texstage);
	}

	const math::Vector3f& Vertexiterator::texcoord3D(const ovis_uint32 texstage) const
	{
		return m_rVertexstream.texcoord3D(m_Position,texstage);
	}

	void Vertexiterator::position(const float x,const float y,const float z)
	{
		m_rVertexstream.position(m_Position,x,y,z);
	}

	void Vertexiterator::position(const math::Vector3f& newPosition)
	{
		m_rVertexstream.position(m_Position,newPosition);
	}

	void Vertexiterator::normal(const float x,const float y,const float z)
	{
		m_rVertexstream.normal(m_Position,x,y,z);
	}

	void Vertexiterator::normal(const math::Vector3f& newNormal)
	{
		m_rVertexstream.normal(m_Position,newNormal);
	}

	void Vertexiterator::diffuseColor(const ovis_uint8 a,const ovis_uint8 r,const ovis_uint8 g,const ovis_uint8 b)
	{
		m_rVertexstream.diffuseColor(m_Position,a,r,g,b);
	}

	void Vertexiterator::diffuseColor(const ovis_uint32 color)
	{
		m_rVertexstream.diffuseColor(m_Position,color);
	}

	void Vertexiterator::specularColor(const ovis_uint8 a,const ovis_uint8 r,const ovis_uint8 g,const ovis_uint8 b)
	{
		m_rVertexstream.specularColor(m_Position,a,r,g,b);
	}

	void Vertexiterator::specularColor(const ovis_uint32 color)
	{
		m_rVertexstream.specularColor(m_Position,color);
	}

	void Vertexiterator::texcoord1D(const ovis_uint32 texstage,const float newTexcoord1D)
	{
		m_rVertexstream.texcoord1D(m_Position,texstage,newTexcoord1D);
	}

	void Vertexiterator::texcoord2D(const ovis_uint32 texstage,const float u,const float v)
	{
		m_rVertexstream.texcoord2D(m_Position,texstage,u,v);
	}

	void Vertexiterator::texcoord2D(const ovis_uint32 texstage,const math::Vector2f& newTexcoord2D)
	{
		m_rVertexstream.texcoord2D(m_Position,texstage,newTexcoord2D);
	}

	void Vertexiterator::texcoord3D(const ovis_uint32 texstage,const float u,const float v,const float w)
	{
		m_rVertexstream.texcoord3D(m_Position,texstage,u,v,w);
	}

	void Vertexiterator::texcoord3D(const ovis_uint32 texstage,const math::Vector3f& newTexcoord3D)
	{
		m_rVertexstream.texcoord3D(m_Position,texstage,newTexcoord3D);
	}

	ovis_uint32 Vertexiterator::num1DTexcoords() const
	{
		return m_rVertexstream.num1DTexcoords();
	}

	ovis_uint32 Vertexiterator::num2DTexcoords() const
	{
		return m_rVertexstream.num2DTexcoords();
	}

	ovis_uint32 Vertexiterator::num3DTexcoords() const
	{
		return m_rVertexstream.num3DTexcoords();
	}

	 bool operator ==(const Vertexiterator &a,const Vertexiterator &b)
	 {
		 return (a.positionInStream()==b.positionInStream()) && (&a == &b);
	 }

	 bool operator !=(const Vertexiterator &a,const Vertexiterator &b)
	 {
		 return (a.positionInStream()!=b.positionInStream()) || (&a != &b);
	 }

}
}
