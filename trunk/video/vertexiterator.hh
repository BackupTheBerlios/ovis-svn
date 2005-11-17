#ifndef OVIS_VIDEO_VERTEXITERATOR_HH_INCLUDED
#define OVIS_VIDEO_VERTEXITERATOR_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/itypes.hh"
#include "../math/vectors.hh"
#include <vector>
#include "vertexformat.hh"

namespace ovis {
namespace video {

	class Vertexstream;

	struct VertexiteratorPointers
	{
		//! Points to the position member of the vertex.
		math::Vector3f *m_pPosition;
		//! Points to the normal member of the vertex (if present).
		math::Vector3f *m_pNormal;

		//! Color members (ARGB order, 8 bit per value)
		ovis_uint32 *m_pDiffuseColor,*m_pSpecularColor;

		//! Array of pointers pointing to the 1D texcoord members (if present).
		/*! Size of this array = m_Num1DTexcoords. */
		float **m_pp1DTexcoords;
		//! The amount of 1D texcoords in the structure.
		ovis_uint32 m_Num1DTexcoords;
		//! Array of pointers pointing to the 2D texcoord members (if present).
		/*! Size of this array = m_Num2DTexcoords. */
		math::Vector2f **m_pp2DTexcoords;
		//! The amount of 2D texcoords in the structure.
		ovis_uint32 m_Num2DTexcoords;
		//! Array of pointers pointing to the 3D texcoord members (if present).
		/*! Size of this array = m_Num3DTexcoords. */
		math::Vector3f **m_pp3DTexcoords;
		//! The amount of 3D texcoords in the structure.
		ovis_uint32 m_Num3DTexcoords;

		ovis_uint32 m_CurrentVtx;
	};

	class OVIS_API Vertexiterator
	{
	public:
		Vertexiterator(Vertexstream &rVertexstream);
		Vertexiterator(Vertexstream& rVertexstream,const ovis_uint32 position);
		~Vertexiterator();

		const Vertexstream& vertexstream() const;

		void positionInStream(const ovis_uint32 newPosition);
		ovis_uint32 positionInStream() const;

		Vertexiterator& operator ++();
		Vertexiterator& operator --();
		Vertexiterator& operator ++(const int);
		Vertexiterator& operator --(const int);
		Vertexiterator& operator +=(const int offset);
		Vertexiterator& operator -=(const int offset);

		const math::Vector3f& position() const;
		const math::Vector3f& normal() const;
		ovis_uint32 diffuseColor() const;
		ovis_uint32 specularColor() const;
		float texcoord1D(const ovis_uint32 texstage) const;
		const math::Vector2f& texcoord2D(const ovis_uint32 texstage) const;
		const math::Vector3f& texcoord3D(const ovis_uint32 texstage) const;

		void position(const float x,const float y,const float z);
		void position(const math::Vector3f& newPosition);
		void normal(const float x,const float y,const float z);
		void normal(const math::Vector3f& newNormal);
		void diffuseColor(const ovis_uint8 a,const ovis_uint8 r,const ovis_uint8 g,const ovis_uint8 b);
		void diffuseColor(const ovis_uint32 color);
		void specularColor(const ovis_uint8 a,const ovis_uint8 r,const ovis_uint8 g,const ovis_uint8 b);
		void specularColor(const ovis_uint32 color);
		void texcoord1D(const ovis_uint32 texstage,const float newTexcoord1D);
		void texcoord2D(const ovis_uint32 texstage,const float u,const float v);
		void texcoord2D(const ovis_uint32 texstage,const math::Vector2f& newTexcoord2D);
		void texcoord3D(const ovis_uint32 texstage,const float u,const float v,const float w);
		void texcoord3D(const ovis_uint32 texstage,const math::Vector3f& newTexcoord3D);

		ovis_uint32 num1DTexcoords() const;
		ovis_uint32 num2DTexcoords() const;
		ovis_uint32 num3DTexcoords() const;

	protected:
		Vertexstream& m_rVertexstream;
		ovis_uint32 m_Position;
	};

	bool operator ==(const Vertexiterator &a,const Vertexiterator &b);
	bool operator !=(const Vertexiterator &a,const Vertexiterator &b);

}
}

#endif
