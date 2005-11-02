#ifndef OVIS_MATH_MATRIX_HH_INCLUDED
#define OVIS_MATH_MATRIX_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/numericarrays.hh"
#include <math.h>
#include "vectors.hh"

namespace ovis {
namespace math {

class OVIS_API Matrix4f:public base::Float4x4
{
public:
	Matrix4f( ) { loadIdentity(); }
	Matrix4f(const Matrix4f& v) { clone(v); }
	Matrix4f( float n11 , float n12 , float n13 , float n14,
		float n21 , float n22 , float n23 , float n24,
		float n31 , float n32 , float n33 , float n34,
		float n41 , float n42 , float n43 , float n44 ):
	Float4x4(n11,n12,n13,n14,
		n21,n22,n23,n24,
		n31,n32,n33,n34,
		n41,n42,n43,n44) {}

	void clone(const Matrix4f& v)
	{
		m_11 = v.m_11;
		m_12 = v.m_12;
		m_13 = v.m_13;
		m_14 = v.m_14;

		m_21 = v.m_21;
		m_22 = v.m_22;
		m_23 = v.m_23;
		m_24 = v.m_24;

		m_31 = v.m_31;
		m_32 = v.m_32;
		m_33 = v.m_33;
		m_34 = v.m_34;

		m_41 = v.m_41;
		m_42 = v.m_42;
		m_43 = v.m_43;
		m_44 = v.m_44;	}

	/* Fast Operations */
	Matrix4f operator + ( const Matrix4f & m ) const
	{
		return Matrix4f( m_11 + m.m_11 , m_12 + m.m_12 , m_13 + m.m_13 , m_14 + m.m_14,
			m_21 + m.m_21 , m_22 + m.m_22 , m_23 + m.m_23 , m_24 + m.m_24,
			m_31 + m.m_31 , m_32 + m.m_32 , m_33 + m.m_33 , m_34 + m.m_34,
			m_41 + m.m_41 , m_42 + m.m_42 , m_43 + m.m_43 , m_44 + m.m_44
			);
	}

	Matrix4f operator - ( const Matrix4f & m ) const
	{
		return Matrix4f( m_11 - m.m_11 , m_12 - m.m_12 , m_13 - m.m_13 , m_14 - m.m_14,
			m_21 - m.m_21 , m_22 - m.m_22 , m_23 - m.m_23 , m_24 - m.m_24,
			m_31 - m.m_31 , m_32 - m.m_32 , m_33 - m.m_33 , m_34 - m.m_34,
			m_41 - m.m_41 , m_42 - m.m_42 , m_43 - m.m_43 , m_44 - m.m_44
			);
	}

	Matrix4f operator * ( const float scale ) const
	{
		return Matrix4f( m_11 * scale , m_12 * scale , m_13 * scale , m_14 * scale,
			m_21 * scale , m_22 * scale , m_23 * scale , m_24 * scale,
			m_31 * scale , m_32 * scale , m_33 * scale , m_34 * scale,
			m_41 * scale , m_42 * scale , m_43 * scale , m_44 * scale
			);
	}

	Matrix4f operator * ( const Matrix4f & m ) const
	{
		return Matrix4f( m_11 * m.m_11 + m_12 * m.m_21 + m_13 * m.m_31 + m_14 * m.m_41,
			m_11 * m.m_12 + m_12 * m.m_22 + m_13 * m.m_32 + m_14 * m.m_42,
			m_11 * m.m_13 + m_12 * m.m_23 + m_13 * m.m_33 + m_14 * m.m_43,
			m_11 * m.m_14 + m_12 * m.m_24 + m_13 * m.m_34 + m_14 * m.m_44,

			m_21 * m.m_11 + m_22 * m.m_21 + m_23 * m.m_31 + m_24 * m.m_41,
			m_21 * m.m_12 + m_22 * m.m_22 + m_23 * m.m_32 + m_24 * m.m_42,
			m_21 * m.m_13 + m_22 * m.m_23 + m_23 * m.m_33 + m_24 * m.m_43,
			m_21 * m.m_14 + m_22 * m.m_24 + m_23 * m.m_34 + m_24 * m.m_44,

			m_31 * m.m_11 + m_32 * m.m_21 + m_33 * m.m_31 + m_34 * m.m_41,
			m_31 * m.m_12 + m_32 * m.m_22 + m_33 * m.m_32 + m_34 * m.m_42,
			m_31 * m.m_13 + m_32 * m.m_23 + m_33 * m.m_33 + m_34 * m.m_43,
			m_31 * m.m_14 + m_32 * m.m_24 + m_33 * m.m_34 + m_34 * m.m_44,

			m_41 * m.m_11 + m_42 * m.m_21 + m_43 * m.m_31 + m_44 * m.m_41,
			m_41 * m.m_12 + m_42 * m.m_22 + m_43 * m.m_32 + m_44 * m.m_42,
			m_41 * m.m_13 + m_42 * m.m_23 + m_43 * m.m_33 + m_44 * m.m_43,
			m_41 * m.m_14 + m_42 * m.m_24 + m_43 * m.m_34 + m_44 * m.m_44
			);
	}

	base::Float4 operator * ( const base::Float4 & v ) const
	{
		return base::Float4(
			m_11 * v[0] + m_21 * v[1] + m_31 * v[2] + m_41 * v[3],
			m_12 * v[0] + m_22 * v[1] + m_32 * v[2] + m_42 * v[3],
			m_13 * v[0] + m_23 * v[1] + m_33 * v[2] + m_43 * v[3],
			m_14 * v[0] + m_24 * v[1] + m_34 * v[2] + m_44 * v[3]
			);
	}

	base::Float3 operator * ( const base::Float3 & v ) const
	{
		return base::Float3(
			m_11 * v[0] + m_21 * v[1] + m_31 * v[2] + m_41,
			m_12 * v[0] + m_22 * v[1] + m_32 * v[2] + m_42,
			m_13 * v[0] + m_23 * v[1] + m_33 * v[2] + m_43
			//m_14 * v[0] + m_24 * v[1] + m_34 * v[2] + m_44
			);
	}


	/* JT: This next inversion function is from Intel's developer pages,
	*	 many thanks to them for providing this fast matrix inverse.
	*/
	Matrix4f invert( ) const
	{
		Matrix4f dst;

		float tmp[12]; /* temp array for pairs */
		float src[16]; /* array of transpose source matrix */
		float det; /* determinant */
		/* transpose matrix */
		for ( int i = 0; i < 4; i ++ )
		{
			src[i] = m_Floats[i * 4];
			src[i + 4] = m_Floats[i * 4 + 1];
			src[i + 8] = m_Floats[i * 4 + 2];
			src[i + 12] = m_Floats[i * 4 + 3];
		}
		/* calculate pairs for first 8 elements (cofactors) */
		tmp[0] = src[10] * src[15];
		tmp[1] = src[11] * src[14];
		tmp[2] = src[9] * src[15];
		tmp[3] = src[11] * src[13];
		tmp[4] = src[9] * src[14];
		tmp[5] = src[10] * src[13];
		tmp[6] = src[8] * src[15];
		tmp[7] = src[11] * src[12];
		tmp[8] = src[8] * src[14];
		tmp[9] = src[10] * src[12];
		tmp[10] = src[8] * src[13];
		tmp[11] = src[9] * src[12];
		/* calculate first 8 elements (cofactors) */
		dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
		dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
		dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
		dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
		dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
		dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
		dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
		dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
		dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
		dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
		dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
		dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
		dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
		dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
		dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
		dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
		/* calculate pairs for second 8 elements (cofactors) */
		tmp[0] = src[2] * src[7];
		tmp[1] = src[3] * src[6];
		tmp[2] = src[1] * src[7];
		tmp[3] = src[3] * src[5];
		tmp[4] = src[1] * src[6];
		tmp[5] = src[2] * src[5];


		tmp[6] = src[0] * src[7];
		tmp[7] = src[3] * src[4];
		tmp[8] = src[0] * src[6];
		tmp[9] = src[2] * src[4];
		tmp[10] = src[0] * src[5];
		tmp[11] = src[1] * src[4];

		/* calculate second 8 elements (cofactors) */
		dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
		dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
		dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
		dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
		dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
		dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
		dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
		dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
		dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
		dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
		dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
		dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
		dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
		dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
		dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
		dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

		/* calculate determinant */
		det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];

		/* calculate matrix inverse */
		det = 1 / det;
		for ( int j = 0; j < 16; j ++ )
			dst[j] *= det;

		return dst;
	}


	/* Assignment operators and variants */
	Matrix4f& operator =(const Matrix4f &v)
	{
		clone(v);
		return *this;
	}

	Matrix4f& operator =(const Float4x4& v)
	{
		for (int i=0;i<16;++i) m_Floats[i]=v[i];
		return *this;
	}

	Matrix4f & operator += ( const Matrix4f & v )
	{
		m_11 += v.m_11;
		m_12 += v.m_12;
		m_13 += v.m_13;
		m_14 += v.m_14;

		m_21 += v.m_21;
		m_22 += v.m_22;
		m_23 += v.m_23;
		m_24 += v.m_24;

		m_31 += v.m_31;
		m_32 += v.m_32;
		m_33 += v.m_33;
		m_34 += v.m_34;

		m_41 += v.m_41;
		m_42 += v.m_42;
		m_43 += v.m_43;
		m_44 += v.m_44;

		return * this;
	}

	Matrix4f & operator *= ( const float scale )
	{
		m_11 *= scale;
		m_12 *= scale;
		m_13 *= scale;
		m_14 *= scale;

		m_21 *= scale;
		m_22 *= scale;
		m_23 *= scale;
		m_24 *= scale;

		m_31 *= scale;
		m_32 *= scale;
		m_33 *= scale;
		m_34 *= scale;

		m_41 *= scale;
		m_42 *= scale;
		m_43 *= scale;
		m_44 *= scale;

		return * this;
	}

	/*
	Matrix4f( float n11 , float n12 , float n13 , float n14,
	float n21 , float n22 , float n23 , float n24,
	float n31 , float n32 , float n33 , float n34,
	float n41 , float n42 , float n43 , float n44 )
	: m_11( n11 ) , m_12( n12 ) , m_13( n13 ) , m_14( n14 ),
	m_21( n21 ) , m_22( n22 ) , m_23( n23 ) , m_24( n24 ),
	m_31( n31 ) , m_32( n32 ) , m_33( n33 ) , m_34( n34 ),
	m_41( n41 ) , m_42( n42 ) , m_43( n43 ) , m_44( n44 )
	{};
	
	  
		Matrix4f operator * ( const Matrix4f & m ) const
	{
	return Matrix4f( m_11 * m.m_11 + m_12 * m.m_21 + m_13 * m.m_31 + m_14 * m.m_41,
	m_11 * m.m_12 + m_12 * m.m_22 + m_13 * m.m_32 + m_14 * m.m_42,
	m_11 * m.m_13 + m_12 * m.m_23 + m_13 * m.m_33 + m_14 * m.m_43,
	m_11 * m.m_14 + m_12 * m.m_24 + m_13 * m.m_34 + m_14 * m.m_44,
	
	  m_21 * m.m_11 + m_22 * m.m_21 + m_23 * m.m_31 + m_24 * m.m_41,
	  m_21 * m.m_12 + m_22 * m.m_22 + m_23 * m.m_32 + m_24 * m.m_42,
	  m_21 * m.m_13 + m_22 * m.m_23 + m_23 * m.m_33 + m_24 * m.m_43,
	  m_21 * m.m_14 + m_22 * m.m_24 + m_23 * m.m_34 + m_24 * m.m_44,
	  
		m_31 * m.m_11 + m_32 * m.m_21 + m_33 * m.m_31 + m_34 * m.m_41,
		m_31 * m.m_12 + m_32 * m.m_22 + m_33 * m.m_32 + m_34 * m.m_42,
		m_31 * m.m_13 + m_32 * m.m_23 + m_33 * m.m_33 + m_34 * m.m_43,
		m_31 * m.m_14 + m_32 * m.m_24 + m_33 * m.m_34 + m_34 * m.m_44,
		
		  m_41 * m.m_11 + m_42 * m.m_21 + m_43 * m.m_31 + m_44 * m.m_41,
		  m_41 * m.m_12 + m_42 * m.m_22 + m_43 * m.m_32 + m_44 * m.m_42,
		  m_41 * m.m_13 + m_42 * m.m_23 + m_43 * m.m_33 + m_44 * m.m_43,
		  m_41 * m.m_14 + m_42 * m.m_24 + m_43 * m.m_34 + m_44 * m.m_44
		  );
		  }
		  
	*/

	Matrix4f & operator *= ( const Matrix4f & m )
	{
		*this=*this*m;
		
		return *this;
	}		

	/*inline void loadMatrix() {
		glLoadMatrixf(m_Floats);
	}

	inline void multMatrix() {
		glMultMatrixf(m_Floats);
	}*/

    inline void loadIdentity()
	{
		m_Floats[0] = m_Floats[5] = m_Floats[10] = m_Floats[15] = 1.0f;
		m_Floats[1] = m_Floats[2] = m_Floats[3] = m_Floats[4] =  m_Floats[6] = m_Floats[7] = m_Floats[8] = m_Floats[9] = m_Floats[11] = m_Floats[12] = m_Floats[13] = m_Floats[14] = 0.0f;
	}
	
    inline void rotateX(const float in_fAngle)
	{
		float c = (float)cos(in_fAngle);
		float s = (float)sin(in_fAngle);
		
		m_Floats[5] = c;
		m_Floats[6] = s;
		m_Floats[9] = -s;
		m_Floats[10] = c;
	}
	
    inline void rotateY(const float in_fAngle)
	{
		float c = (float)cos(in_fAngle);
		float s = (float)sin(in_fAngle);
		
		m_Floats[0] = c;
		m_Floats[2] = -s;
		m_Floats[8] = s;
		m_Floats[10] = c;
	}
	
	inline void rotateZ(const float in_fAngle)
	{
		float c = (float)cos(in_fAngle);
		float s = (float)sin(in_fAngle);
		
		m_Floats[0] = c;
		m_Floats[1] = s;
		m_Floats[4] = -s;
		m_Floats[5] = c;
	}

    inline void scale(const float in_fX, const float in_fY, const float in_fZ)
	{
		m_Floats[ 0] = in_fX;
		m_Floats[ 5] = in_fY;
		m_Floats[10] = in_fZ;
	}

	inline void scale(const float* in_pVector)
	{
		m_Floats[ 0] = in_pVector[0];
		m_Floats[ 5] = in_pVector[1];
		m_Floats[10] = in_pVector[2];
	}

	inline void scale(const base::Float3& in_Vector)
	{
		m_Floats[ 0] = in_Vector[0];
		m_Floats[ 5] = in_Vector[1];
		m_Floats[10] = in_Vector[2];
	}
		
    inline void translate(const float in_fX, const float in_fY, const float in_fZ)
	{
		m_Floats[12] = m_Floats[0] * in_fX + m_Floats[4] * in_fY + m_Floats[8]  * in_fZ + m_Floats[12];
		m_Floats[13] = m_Floats[1] * in_fX + m_Floats[5] * in_fY + m_Floats[9]  * in_fZ + m_Floats[13];
		m_Floats[14] = m_Floats[2] * in_fX + m_Floats[6] * in_fY + m_Floats[10] * in_fZ + m_Floats[14];
		m_Floats[15] = m_Floats[3] * in_fX + m_Floats[7] * in_fY + m_Floats[11] * in_fZ + m_Floats[15];
	}

    inline void translate(const float* in_pVector)
	{
		m_Floats[12] = m_Floats[0] * in_pVector[0] + m_Floats[4] * in_pVector[1] + m_Floats[8]  * in_pVector[2] + m_Floats[12];
		m_Floats[13] = m_Floats[1] * in_pVector[0] + m_Floats[5] * in_pVector[1] + m_Floats[9]  * in_pVector[2] + m_Floats[13];
		m_Floats[14] = m_Floats[2] * in_pVector[0] + m_Floats[6] * in_pVector[1] + m_Floats[10] * in_pVector[2] + m_Floats[14];
		m_Floats[15] = m_Floats[3] * in_pVector[0] + m_Floats[7] * in_pVector[1] + m_Floats[11] * in_pVector[2] + m_Floats[15];
	}

	inline void translate(const base::Float3& in_Vector)
	{
		m_Floats[12] = m_Floats[0] * in_Vector[0] + m_Floats[4] * in_Vector[1] + m_Floats[8]  * in_Vector[2] + m_Floats[12];
		m_Floats[13] = m_Floats[1] * in_Vector[0] + m_Floats[5] * in_Vector[1] + m_Floats[9]  * in_Vector[2] + m_Floats[13];
		m_Floats[14] = m_Floats[2] * in_Vector[0] + m_Floats[6] * in_Vector[1] + m_Floats[10] * in_Vector[2] + m_Floats[14];
		m_Floats[15] = m_Floats[3] * in_Vector[0] + m_Floats[7] * in_Vector[1] + m_Floats[11] * in_Vector[2] + m_Floats[15];
	}

	inline Matrix4f transpose() const
	{
		Matrix4f dst;

		for ( int i = 0; i < 4; i ++ )
		{
			dst[i] = m_Floats[i * 4];
			dst[i + 4] = m_Floats[i * 4 + 1];
			dst[i + 8] = m_Floats[i * 4 + 2];
			dst[i + 12] = m_Floats[i * 4 + 3];
		}

		return dst;
	}
	
	inline void perspective(const float left,const float right,
		const float bottom,const float top,
		const float znear,const float zfar)
	{
		loadIdentity();

		float zn=znear,zf=zfar;

		m_11=2*zn/(right-left);
		m_22=2*zn/(top-bottom);
		m_33=zf/(zf-zn);
		m_43=-1*zf*zn/(zf-zn);
		m_34=1;
		m_31=-(right+left)/(right-left);
		m_32=-(top+bottom)/(top-bottom);
		m_44=0;
	}
	
	inline void perspective(const float fov,const float aspect,
		const float znear,const float zfar)
	{
		float top=znear*tanf(fov*3.1415926535f/360.0f);
		float bottom=-top;
		float left=bottom*aspect;
		float right=top*aspect;
		
		perspective(left,right,bottom,top,znear,zfar);
	}

	void lookatDirection(const Vector3f& origin,const Vector3f& out,const Vector3f& worldUp)
	{
		Vector3f right(out^worldUp); right.normalize();
		Vector3f up(right^out); up.normalize();

		m_11=right.x();	m_21=right.y();	m_31=right.z();	m_14=0.0f;
		m_12=up.x();	m_22=up.y();	m_32=up.z();	m_24=0.0f;
		m_13=-out.x();	m_23=-out.y();	m_33=-out.z();	m_34=0.0f;

		translate(-origin);
	}

	void lookatLocation(const Vector3f& origin,const Vector3f& to,const Vector3f& worldUp)
	{
		Vector3f out(to-origin); out.normalize();
		Vector3f right(out^worldUp); right.normalize();
		Vector3f up(right^out); up.normalize();

		m_11=right.x();	m_21=right.y();	m_31=right.z();	m_14=0.0f;
		m_12=up.x();	m_22=up.y();	m_32=up.z();	m_24=0.0f;
		m_13=-out.x();	m_23=-out.y();	m_33=-out.z();	m_34=0.0f;

		translate(-origin);
	}
	
	inline void fromQuaternion(const base::Float4& quat)
	{
		float xx=quat[0]*quat[0];
		float xy=quat[0]*quat[1];
		float xz=quat[0]*quat[2];
		float xw=quat[0]*quat[3];
		float yy=quat[1]*quat[1];
		float yz=quat[1]*quat[2];
		float yw=quat[1]*quat[3];
		float zz=quat[2]*quat[2];
		float zw=quat[2]*quat[3];

		m_Floats[0]  = 1 - 2 * ( yy + zz );
		m_Floats[1]  =     2 * ( xy - zw );
		m_Floats[2]  =     2 * ( xz + yw );
		m_Floats[4]  =     2 * ( xy + zw );
		m_Floats[5]  = 1 - 2 * ( xx + zz );
		m_Floats[6]  =     2 * ( yz - xw );
		m_Floats[8]  =     2 * ( xz - yw );
		m_Floats[9]  =     2 * ( yz + xw );
		m_Floats[10] = 1 - 2 * ( xx + yy );
		m_Floats[3]  = m_Floats[7] = m_Floats[11] = m_Floats[12] = m_Floats[13] = m_Floats[14] = 0;
		m_Floats[15] = 1;
	}
};

}
}

#endif
