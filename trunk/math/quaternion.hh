#ifndef OVIS_MATH_QUATERNION_HH_INCLUDED
#define OVIS_MATH_QUATERNION_HH_INCLUDED

#include <math.h>

#include "../base/dll.hh"
#include "../base/numericarrays.hh"
#include "vectors.hh"

namespace ovis {
namespace math {

class OVIS_API Quaternion:public base::Float4 {
public:
	Quaternion() { m_Floats[0]=0.0f; m_Floats[1]=0.0f; m_Floats[2]=0.0f; m_Floats[3]=1.0f; }
	Quaternion(const float x,const float y,const float z,const float w) { m_Floats[0]=x; m_Floats[1]=y; m_Floats[2]=z; m_Floats[3]=w; }
	Quaternion(const Float4 &vec) { m_Floats[0]=vec[0]; m_Floats[1]=vec[1]; m_Floats[2]=vec[2]; m_Floats[3]=vec[3]; }
	Quaternion(const Quaternion &quat) { m_Floats[0]=quat[0]; m_Floats[1]=quat[1]; m_Floats[2]=quat[2]; m_Floats[3]=quat[3]; }

	inline float& operator[](int i) { return m_Floats[i]; }
	inline float operator[](int i) const { return m_Floats[i]; }
	inline float member(int i) const { return m_Floats[i]; }
	inline float& x() { return m_Floats[0]; }
	inline float x() const { return m_Floats[0]; }
	inline float& y() { return m_Floats[1]; }
	inline float y() const { return m_Floats[1]; }
	inline float& z() { return m_Floats[2]; }
	inline float z() const { return m_Floats[2]; }
	inline float& w() { return m_Floats[3]; }
	inline float w() const { return m_Floats[3]; }

	friend Quaternion operator +(const Quaternion& a,const Quaternion& b) {
		return Quaternion(
			a.x()+b.x(),
			a.y()+b.y(),
			a.z()+b.z(),
			a.w()+b.w()
			);
	}

	friend Quaternion operator *(const Quaternion& a,const Quaternion& b) {
		return Quaternion(
			a.w()*b.x() + a.x()*b.w() + a.y()*b.z() - a.z()*b.y(),
			a.w()*b.y() + a.y()*b.w() + a.z()*b.x() - a.x()*b.z(),
			a.w()*b.z() + a.z()*b.w() + a.x()*b.y() - a.y()*b.x(),
			a.w()*b.w() - a.x()*b.x() - a.y()*b.y() - a.z()*b.z()
			);
	}
	
	friend Quaternion operator *(const Quaternion& quat,const float f) {
		return Quaternion(quat.x()*f,quat.y()*f,quat.z()*f,quat.w()*f);
	}

	friend Quaternion operator /(const Quaternion& quat,const float f) {
		if (fabsf(f)<0.0000001f) return Quaternion(0,0,0,0);
		else return Quaternion(quat.x()*f,quat.y()*f,quat.z()*f,quat.w()*f);
	}

	inline Quaternion& operator +=(const Quaternion& quat) { (*this)=(*this)+quat; return (*this); }
	inline Quaternion& operator *=(const Quaternion& quat) { (*this)=(*this)*quat; return (*this); }
	inline Quaternion& operator *=(const float f) { (*this)=(*this)*f; return (*this); }
	inline Quaternion& operator /=(const float f) { (*this)=(*this)/f; return (*this); }

	inline float magnitudeSquared() const { return x()*x()+y()*y()+z()*z()+w()*w(); }
	inline float magnitude() const { return sqrtf(magnitudeSquared()); }
	inline Quaternion conjugated() const { return Quaternion(-x(),-y(),-z(),w()); }
	inline Quaternion normalized() const { float f=1.0f/magnitude(); return Quaternion(x()*f,y()*f,z()*f,w()*f); }
	
	inline Quaternion inverted() const {
		float m=magnitude();
		if (m>0.0f) { m=1.0f/m; return Quaternion(-x()*m,-y()*m,-z()*m,w()*m); }
		else return Quaternion(0,0,0,0);
	}

	// NOTE: axis should be normalized!
	inline void fromRotAxisAndAngle(const Vector3f& axis,const float angle) {
		float sinangle=sinf(angle*0.5f);

		x()=axis.x()*sinangle;
		y()=axis.y()*sinangle;
		z()=axis.z()*sinangle;
		w()=cosf(angle*0.5f);
	}

	inline Vector4f rotateVector(const Vector4f& vec) const {
		Quaternion v(vec);
		return Vector4f( (*this)*v*inverted() );
	}

	inline Vector3f rotateVector(const Vector3f& vec) const {
		Quaternion v(vec.x(),vec.y(),vec.z(),0);
		v=(*this)*v*inverted();
		return Vector3f(v.x(),v.y(),v.z());
	}

	inline void fromMatrix(const base::Float4x4& matrix)
	{
		float t=matrix[0]+matrix[5]+matrix[10]+1.0f;

		if (t>0.00000001f) {
			float s=0.5f/sqrtf(t);
			x()=(matrix[9]-matrix[6])*s;
			y()=(matrix[2]-matrix[8])*s;
			z()=(matrix[4]-matrix[1])*s;
			w()=0.25f/s;
		} else {

			if ((matrix[0]>matrix[5]) && (matrix[0]>matrix[10]))  {	// Column 0: 
				float s=sqrtf(1.0f+matrix[0]-matrix[5]-matrix[10])*2.0f;
				float invs=1.0f/s;
				x()=0.25f*s;
				y()=(matrix[4]+matrix[1])*invs;
				z()=(matrix[2]+matrix[8])*invs;
				w()=(matrix[9]-matrix[6])*invs;
			} else if (matrix[5]>matrix[10])  {	// Column 1: 
				float s=sqrtf(1.0f+matrix[5]-matrix[0]-matrix[10])*2.0f;
				float invs=1.0f/s;
				x()=(matrix[4]+matrix[1])*invs;
				y()=0.25f*s;
				z()=(matrix[9]+matrix[6])*invs;
				w()=(matrix[2]-matrix[8])*invs;
			} else {	// Column 2: 
				float s=sqrtf(1.0f+matrix[10]-matrix[0]-matrix[5])*2.0f;
				float invs=1.0f/s;
				x()=(matrix[2]+matrix[8])*invs;
				y()=(matrix[9]+matrix[6])*invs;
				z()=0.25f*s;
				w()=(matrix[4]-matrix[1])*invs;
			}

		}

		float f=1.0f/magnitude();
		m_Floats[0]*=f;
		m_Floats[1]*=f;
		m_Floats[2]*=f;
		m_Floats[3]*=f;

	}
};

}
}

#endif
