#ifndef OVIS_MATH_MATRIX_HH_INCLUDED
#define OVIS_MATH_MATRIX_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/numericarrays.hh"
#include <math.h>
#include "vectors.hh"

namespace ovis {
namespace math {

	//! 4x4 transformation matrix class.
	/**
	* This class contains functionality for creating and manipulating 4x4 transformation
	* matrices.
	*/
	class OVIS_API Matrix4f:public base::Float4x4
	{
	public:
		//! Constructor which creates an identity matrix..
		Matrix4f( ) { loadIdentity(); }
		//! Copy constructor. Calls clone().
		/**
		* @param v The matrix to be copied
		*/
		Matrix4f(const Matrix4f& v) { clone(v); }
		//! Constructor which 16 parameters for each matrix member
		Matrix4f( float n11 , float n12 , float n13 , float n14,
			float n21 , float n22 , float n23 , float n24,
			float n31 , float n32 , float n33 , float n34,
			float n41 , float n42 , float n43 , float n44 ):
		Float4x4(n11,n12,n13,n14,
			n21,n22,n23,n24,
			n31,n32,n33,n34,
			n41,n42,n43,n44) {}

		//! Clones the matrix.
		/**
		* This copies all 16 values of the given matrix, effectively cloning it.
		* @param v The matrix to be cloned
		*/
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

		//! Adds the members of this matrix and matrix m and creates a new matrix out of the added members
		/**
		* @param m The matrix which shall be added to this one
		* @return The resulting matrix with the added members
		*/
		Matrix4f operator + ( const Matrix4f & m ) const
		{
			return Matrix4f( m_11 + m.m_11 , m_12 + m.m_12 , m_13 + m.m_13 , m_14 + m.m_14,
				m_21 + m.m_21 , m_22 + m.m_22 , m_23 + m.m_23 , m_24 + m.m_24,
				m_31 + m.m_31 , m_32 + m.m_32 , m_33 + m.m_33 , m_34 + m.m_34,
				m_41 + m.m_41 , m_42 + m.m_42 , m_43 + m.m_43 , m_44 + m.m_44
				);
		}

		//! Substracts the members of matrix m from this matrix and creates a new matrix out of the subtracted members
		/**
		* @param m The matrix which shall be subtracted from this one
		* @return The resulting matrix with the subtracted members
		*/
		Matrix4f operator - ( const Matrix4f & m ) const
		{
			return Matrix4f( m_11 - m.m_11 , m_12 - m.m_12 , m_13 - m.m_13 , m_14 - m.m_14,
				m_21 - m.m_21 , m_22 - m.m_22 , m_23 - m.m_23 , m_24 - m.m_24,
				m_31 - m.m_31 , m_32 - m.m_32 , m_33 - m.m_33 , m_34 - m.m_34,
				m_41 - m.m_41 , m_42 - m.m_42 , m_43 - m.m_43 , m_44 - m.m_44
				);
		}

		//! Multiplies all members with a scalar and returns a new matrix with the multiplied members.
		/**
		* @param scalar Scalar which is to be multiplied with all members
		* @return The resulting matrix with the multiplied members
		*/
		Matrix4f operator * ( const float scalar ) const
		{
			return Matrix4f( m_11 * scalar , m_12 * scalar , m_13 * scalar , m_14 * scalar,
				m_21 * scalar , m_22 * scalar , m_23 * scalar , m_24 * scalar,
				m_31 * scalar , m_32 * scalar , m_33 * scalar , m_34 * scalar,
				m_41 * scalar , m_42 * scalar , m_43 * scalar , m_44 * scalar
				);
		}

		//! Multiplies matrix m with this matrix and returns the resulting matrix.
		/**
		* @param m Matrix which shall be multiplied with this one
		* @return The resulting matrix
		*/
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

		//! Multiplies v with this matrix and returns the resulting 4D-float array.
		/**
		* @param v Vector to be multiplied with this matrix
		* @return The resulting vector
		*/
		base::Float4 operator * ( const base::Float4 & v ) const
		{
			return base::Float4(
				m_11 * v[0] + m_21 * v[1] + m_31 * v[2] + m_41 * v[3],
				m_12 * v[0] + m_22 * v[1] + m_32 * v[2] + m_42 * v[3],
				m_13 * v[0] + m_23 * v[1] + m_33 * v[2] + m_43 * v[3],
				m_14 * v[0] + m_24 * v[1] + m_34 * v[2] + m_44 * v[3]
				);
		}

		//! Multiplies v with this matrix and returns the resulting 3D-float array.
		/**
		* @param v Vector to be multiplied with this matrix
		* @return The resulting vector
		*/
		base::Float3 operator * ( const base::Float3 & v ) const
		{
			return base::Float3(
				m_11 * v[0] + m_21 * v[1] + m_31 * v[2] + m_41,
				m_12 * v[0] + m_22 * v[1] + m_32 * v[2] + m_42,
				m_13 * v[0] + m_23 * v[1] + m_33 * v[2] + m_43
				//m_14 * v[0] + m_24 * v[1] + m_34 * v[2] + m_44
				);
		}


		//! Calculates and returns an inverted version of this matrix.
		/**
		* @return The inverted matrix.
		* @note This next inversion function is from Intel's developer pages,
		* many thanks to them for providing this fast matrix inverse.
		*
		* @note Orthogonal matrices (like all pure rotation matrices) can be inverted simply by transposing them.
		* Since transposing is much faster, one should consider this.
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
		
		
		//! Copies the contents of v into this matrix.
		/**
		* Behaves exactly like clone(v).
		* @param v The matrix to be copied
		* @return A reference to this matrix. Useful for concatenation of operations.
		*/
		Matrix4f& operator =(const Matrix4f &v)
		{
			clone(v);
			return *this;
		}

		//! Copies the contents of v into this matrix.
		/**
		* Copies all 16 values of the 4x4 floatarray into this matrix.
		* @param v The 4x4 floatarray to be copied
		* @return A reference to this matrix. Useful for concatenation of operations.
		*/
		Matrix4f& operator =(const Float4x4& v)
		{
			for (int i=0;i<16;++i) m_Floats[i]=v[i];
			return *this;
		}

		//! Adds the members of v to this matrix.
		/**
		* @param v Matrix whose members shall be added to this matrix' ones.
		* @return A reference to this matrix. Useful for concatenation of operations.
		*/
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

		//! Multiplies all members with a scalar.
		/**
		* @param scalar Scalar which shall be multiplied with all members.
		* @return A reference to this matrix. Useful for concatenation of operations.
		*/
		Matrix4f & operator *= ( const float scalar )
		{
			m_11 *= scalar;
			m_12 *= scalar;
			m_13 *= scalar;
			m_14 *= scalar;

			m_21 *= scalar;
			m_22 *= scalar;
			m_23 *= scalar;
			m_24 *= scalar;

			m_31 *= scalar;
			m_32 *= scalar;
			m_33 *= scalar;
			m_34 *= scalar;

			m_41 *= scalar;
			m_42 *= scalar;
			m_43 *= scalar;
			m_44 *= scalar;

			return * this;
		}



		//! Multiplies this matrix with matrix m.
		/**
		* @param m Matrix to be multiplied with this one.
		* @return A reference to this matrix. Useful for concatenation of operations.
		*/
		Matrix4f & operator *= ( const Matrix4f & m )
		{
			*this=*this*m;

			return *this;
		}		



		//! Sets the member values to the identity matrix values.
	    	inline void loadIdentity()
		{
			m_Floats[0] = m_Floats[5] = m_Floats[10] = m_Floats[15] = 1.0f;
			m_Floats[1] = m_Floats[2] = m_Floats[3] = m_Floats[4] =  m_Floats[6] = m_Floats[7] = m_Floats[8] = m_Floats[9] = m_Floats[11] = m_Floats[12] = m_Floats[13] = m_Floats[14] = 0.0f;
		}

		//! Calculates a matrix for rotation around the X axis by a given angle.
		/**
		* @param angle Rotation angle around the X axis, in rad
		* @note This does not reset all members. You have to call loadIdentity() first if the
		* matrix wasn't an identity matrix before.
		*/
    		inline void rotateX(const float angle)
		{
			float c = (float)cos(angle);
			float s = (float)sin(angle);

			m_Floats[5] = c;
			m_Floats[6] = s;
			m_Floats[9] = -s;
			m_Floats[10] = c;
		}

		//! Calculates a matrix for rotation around the Y axis by a given angle.
		/**
		* @param angle Rotation angle around the Y axis, in rad
		* @note This does not reset all members. You have to call loadIdentity() first if the
		* matrix wasn't an identity matrix before.
		*/
	    	inline void rotateY(const float angle)
		{
			float c = (float)cos(angle);
			float s = (float)sin(angle);

			m_Floats[0] = c;
			m_Floats[2] = -s;
			m_Floats[8] = s;
			m_Floats[10] = c;
		}

		//! Calculates a matrix for rotation around the Z axis by a given angle.
		/**
		* @param angle Rotation angle around the Z axis, in rad
		* @note This does not reset all members. You have to call loadIdentity() first if the
		* matrix wasn't an identity matrix before.
		*/
		inline void rotateZ(const float angle)
		{
			float c = (float)cos(angle);
			float s = (float)sin(angle);

			m_Floats[0] = c;
			m_Floats[1] = s;
			m_Floats[4] = -s;
			m_Floats[5] = c;
		}

		//! Calculates a scaling matrix with scales for X, Y and Z directions.
		/**
		* @param x Scale in X direction
		* @param y Scale in Y direction
		* @param z Scale in Z direction
		* @note This does not reset all members. You have to call loadIdentity() first if the
		* matrix wasn't an identity matrix before.
		*/
	    	inline void scale(const float x, const float y, const float z)
		{
			m_Floats[ 0] = x;
			m_Floats[ 5] = y;
			m_Floats[10] = z;
		}

		//! Calculates a scaling matrix with scales for X, Y and Z directions.
		/**
		* @param vector Vector containing scales for all 3 directions, X scale is in vector[0], Y scale in vector[1] etc.
		* @note This does not reset all members. You have to call loadIdentity() first if the
		* matrix wasn't an identity matrix before.
		*/
		inline void scale(const float* vector)
		{
			m_Floats[ 0] = vector[0];
			m_Floats[ 5] = vector[1];
			m_Floats[10] = vector[2];
		}

		//! Calculates a scaling matrix with scales for X, Y and Z directions.
		/**
		* @param vector Vector containing scales for all 3 directions, X scale is in vector[0], Y scale in vector[1] etc.
		* @note This does not reset all members. You have to call loadIdentity() first if the
		* matrix wasn't an identity matrix before.
		*/
		inline void scale(const base::Float3& vector)
		{
			m_Floats[ 0] = vector[0];
			m_Floats[ 5] = vector[1];
			m_Floats[10] = vector[2];
		}

		//! Calculates a translation matrix with translation values for X, Y and Z directions.
		/**
		* @param x Translation in X direction
		* @param y Translation in Y direction
		* @param z Translation in Z direction
		* @note This does not reset all members. You have to call loadIdentity() first if the
		* matrix wasn't an identity matrix before.
		*/
    		inline void translate(const float x, const float y, const float z)
		{
			m_Floats[12] = m_Floats[0] * x + m_Floats[4] * y + m_Floats[8]  * z + m_Floats[12];
			m_Floats[13] = m_Floats[1] * x + m_Floats[5] * y + m_Floats[9]  * z + m_Floats[13];
			m_Floats[14] = m_Floats[2] * x + m_Floats[6] * y + m_Floats[10] * z + m_Floats[14];
			m_Floats[15] = m_Floats[3] * x + m_Floats[7] * y + m_Floats[11] * z + m_Floats[15];
		}

		//! Calculates a translation matrix with translation values for X, Y and Z directions.
		/**
		* @param vector Vector containing translations for all 3 directions, X translation is in vector[0], Y translation in vector[1] etc.
		* @note This does not reset all members. You have to call loadIdentity() first if the
		* matrix wasn't an identity matrix before.
		*/
	    	inline void translate(const float* vector)
		{
			m_Floats[12] = m_Floats[0] * vector[0] + m_Floats[4] * vector[1] + m_Floats[8]  * vector[2] + m_Floats[12];
			m_Floats[13] = m_Floats[1] * vector[0] + m_Floats[5] * vector[1] + m_Floats[9]  * vector[2] + m_Floats[13];
			m_Floats[14] = m_Floats[2] * vector[0] + m_Floats[6] * vector[1] + m_Floats[10] * vector[2] + m_Floats[14];
			m_Floats[15] = m_Floats[3] * vector[0] + m_Floats[7] * vector[1] + m_Floats[11] * vector[2] + m_Floats[15];
		}

		//! Calculates a translation matrix with translation values for X, Y and Z directions.
		/**
		* @param vector Vector containing translations for all 3 directions, X translation is in vector[0], Y translation in vector[1] etc.
		* @note This does not reset all members. You have to call loadIdentity() first if the
		* matrix wasn't an identity matrix before.
		*/
		inline void translate(const base::Float3& vector)
		{
			m_Floats[12] = m_Floats[0] * vector[0] + m_Floats[4] * vector[1] + m_Floats[8]  * vector[2] + m_Floats[12];
			m_Floats[13] = m_Floats[1] * vector[0] + m_Floats[5] * vector[1] + m_Floats[9]  * vector[2] + m_Floats[13];
			m_Floats[14] = m_Floats[2] * vector[0] + m_Floats[6] * vector[1] + m_Floats[10] * vector[2] + m_Floats[14];
			m_Floats[15] = m_Floats[3] * vector[0] + m_Floats[7] * vector[1] + m_Floats[11] * vector[2] + m_Floats[15];
		}

		//! Calculates a transposed version of this matrix.
		/**
		* @note Orthogonal matrices (like all pure rotation matrices) can be inverted simply by transposing them.
		* Since transposing is much faster, one should consider this.
		*
		* @return The transposed version of this matrix.
		*/
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

		//! Calculates a perspective projection matrix.
		/**
		* @param left Position of the left frustum plane
		* @param left Position of the right frustum plane
		* @param bottom Position of the lower frustum plane
		* @param top Position of the upper frustum plane
		* @param znear Position of the near frustum clipping plane
		* @param zfar Position of the far frustum clipping plane
		*/
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

		//! Calculates a perspective projection matrix.
		/**
		* @param fov Field of view angle, in rad
		* @param aspect of the frustum
		* @param znear Position of the near frustum clipping plane
		* @param zfar Position of the far frustum clipping plane
		*/
		inline void perspective(const float fov,const float aspect,
			const float znear,const float zfar)
		{
			float top=znear*tanf(fov);
			float bottom=-top;
			float left=bottom*aspect;
			float right=top*aspect;

			perspective(left,right,bottom,top,znear,zfar);
		}

		//! Calculates a lookat matrix with a given origin, up and out vector.
		/**
		* This constructs a matrix suitable for view matrices of a camera. The camera would be located
		* at the origin, looking in direction of the out vector. worldUp indicates where "up" is (usually 0,1,0).
		* @param origin Location of the viewer
		* @param out Direction the viewer is looking
		* @param worldUp Up vector indicating which direction means "up"
		*/
		void lookatDirection(const Vector3f& origin,const Vector3f& out,const Vector3f& worldUp)
		{
			Vector3f right(out^worldUp); right.normalize();
			Vector3f up(right^out); up.normalize();

			m_11=right.x();	m_21=right.y();	m_31=right.z();	m_14=0.0f;
			m_12=up.x();	m_22=up.y();	m_32=up.z();	m_24=0.0f;
			m_13=-out.x();	m_23=-out.y();	m_33=-out.z();	m_34=0.0f;

			translate(-origin);
		}

		//! Calculates a lookat matrix with a given origin, up and a point the viewer is looking at.
		/**
		* This constructs a matrix suitable for view matrices of a camera. The camera would be located
		* at the origin, looking at the point given in the to vector. worldUp indicates where "up" is (usually 0,1,0).
		* @param origin Location of the viewer
		* @param to Point the viewer is looking at
		* @param worldUp Up vector indicating which direction means "up"
		*/
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

		//! Calculates a matrix from a quaternion.
		/**
		* @param quat 4D floatarray containing the quaternion
		*/
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
