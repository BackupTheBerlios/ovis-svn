#ifndef OVIS_MATH_VECTORS_HH_INCLUDED
#define OVIS_MATH_VECTORS_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/numericarrays.hh"
#include <math.h>

namespace ovis {
namespace math {

class OVIS_API Vector4f:public base::Float4 {
public:
	Vector4f() { m_Floats[0]=0.0f; m_Floats[1]=0.0f; m_Floats[2]=0.0f; m_Floats[2]=0.0f; }
	Vector4f(const float x,const float y,const float z,const float w) { m_Floats[0]=x; m_Floats[1]=y; m_Floats[2]=z; m_Floats[3]=w; }
	Vector4f(const Float4 &vec) { m_Floats[0]=vec[0]; m_Floats[1]=vec[1]; m_Floats[2]=vec[2]; m_Floats[3]=vec[3]; }
	Vector4f(const Vector4f &vec) { m_Floats[0]=vec[0]; m_Floats[1]=vec[1]; m_Floats[2]=vec[2]; m_Floats[3]=vec[3]; }
	
	inline Vector4f& operator += ( const Vector4f& v ) { m_Floats[0]+=v[0]; m_Floats[1]+=v[1]; m_Floats[2]+=v[2]; m_Floats[3]+=v[3]; return *this; }
	inline Vector4f& operator -= ( const Vector4f& v ) { m_Floats[0]-=v[0]; m_Floats[1]-=v[1]; m_Floats[2]-=v[2]; m_Floats[3]-=v[3]; return *this; }
	inline Vector4f& operator *= ( const float d ) { m_Floats[0]*=d; m_Floats[1]*=d; m_Floats[2]*=d; m_Floats[3]*=d; return *this; }
	inline Vector4f& operator /= ( const float d ) { m_Floats[0]/=d; m_Floats[1]/=d; m_Floats[2]/=d; m_Floats[3]/=d; return *this; }
	//inline float& operator[](int i) { return m_Floats[i]; }
	//inline float Vector4f::operator[](int i) const { return m_Floats[i]; }
	//inline float member(int i) const { return m_Floats[i]; }
	inline float& x() { return m_Floats[0]; }
	inline float x() const { return m_Floats[0]; }
	inline float& y() { return m_Floats[1]; }
	inline float y() const { return m_Floats[1]; }
	inline float& z() { return m_Floats[2]; }
	inline float z() const { return m_Floats[2]; }
	inline float& w() { return m_Floats[3]; }
	inline float w() const { return m_Floats[3]; }
	
	float lengthSquared() const { return m_Floats[0]*m_Floats[0]+m_Floats[1]*m_Floats[1]+m_Floats[2]*m_Floats[2]+m_Floats[3]*m_Floats[3]; }
	float length() const { return sqrtf(lengthSquared()); }
	Vector4f& normalize();
	Vector4f normalizedVector() const;

	friend Vector4f operator - (const Vector4f& v) { return Vector4f(-v[0],-v[1],-v[2],-v[3]); }
	friend Vector4f operator + (const Vector4f& a, const Vector4f& b) { return Vector4f(a[0]+b[0],a[1]+b[1],a[2]+b[2],a[3]+b[3]); }
	friend Vector4f operator - (const Vector4f& a, const Vector4f& b) { return Vector4f(a[0]-b[0],a[1]-b[1],a[2]-b[2],a[3]-b[3]); }
	friend Vector4f operator * (const Vector4f& a, const float d) { return Vector4f(a[0]*d,a[1]*d,a[2]*d,a[3]*d); }
	friend Vector4f operator * (const float d, const Vector4f& a) { return Vector4f(a[0]*d,a[1]*d,a[2]*d,a[3]*d); }

	friend float operator * (const Vector4f& a, const Vector4f& b) { return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3]; }    // dot product
	friend Vector4f operator / (const Vector4f& a, const float d) { float od=1.0f/d; return Vector4f(a[0]*od,a[1]*od,a[2]*od,a[3]*od); }
	friend int operator == (const Vector4f& a, const Vector4f& b) { return (a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]); }
	friend int operator != (const Vector4f& a, const Vector4f& b) { return (a[0] != b[0]) || (a[1] != b[1]) || (a[2] != b[2]) || (a[3] != b[3]); }

	friend void swap(Vector4f& a, Vector4f& b) { Vector4f tmp=a; a=b; b=tmp; }
};

class OVIS_API Vector3f:public base::Float3 {
public:
	Vector3f() { m_Floats[0]=0.0f; m_Floats[1]=0.0f; m_Floats[2]=0.0f; }
	Vector3f(const float x,const float y,const float z) { m_Floats[0]=x; m_Floats[1]=y; m_Floats[2]=z; }
	Vector3f(const Float3 &vec) { m_Floats[0]=vec[0]; m_Floats[1]=vec[1]; m_Floats[2]=vec[2];}
	Vector3f(const Vector3f &vec) { m_Floats[0]=vec[0]; m_Floats[1]=vec[1]; m_Floats[2]=vec[2]; }

	inline Vector3f& operator += ( const Vector3f& v ) { m_Floats[0]+=v[0]; m_Floats[1]+=v[1]; m_Floats[2]+=v[2]; return *this;  }
	inline Vector3f& operator -= ( const Vector3f& v ) { m_Floats[0]-=v[0]; m_Floats[1]-=v[1]; m_Floats[2]-=v[2]; return *this;  }
	inline Vector3f& operator *= ( const float d ) { m_Floats[0]*=d; m_Floats[1]*=d; m_Floats[2]*=d; return *this;  }
	inline Vector3f& operator /= ( const float d ) { m_Floats[0]/=d; m_Floats[1]/=d; m_Floats[2]/=d; return *this;  }
	//inline float member(int i) const { return m_Floats[i]; }
	//inline float& operator[](int i) { return m_Floats[i]; }
	//inline float Vector3f::operator[](int i) const { return m_Floats[i]; }
	inline float& x() { return m_Floats[0]; }
	inline float x() const { return m_Floats[0]; }
	inline float& y() { return m_Floats[1]; }
	inline float y() const { return m_Floats[1]; }
	inline float& z() { return m_Floats[2]; }
	inline float z() const { return m_Floats[2]; }
	
	float lengthSquared() const { return m_Floats[0]*m_Floats[0]+m_Floats[1]*m_Floats[1]+m_Floats[2]*m_Floats[2]; }
	float length() const { return sqrtf(lengthSquared()); }
	Vector3f& normalize();
	Vector3f normalizedVector() const;

	Vector3f rotatedX(const float angle) const;
	Vector3f rotatedY(const float angle) const;
	Vector3f rotatedZ(const float angle) const;

	friend Vector3f operator - (const Vector3f& v) { return Vector3f(-v[0],-v[1],-v[2]); }
	friend Vector3f operator + (const Vector3f& a, const Vector3f& b) { return Vector3f(a[0]+b[0],a[1]+b[1],a[2]+b[2]); }
	friend Vector3f operator - (const Vector3f& a, const Vector3f& b) { return Vector3f(a[0]-b[0],a[1]-b[1],a[2]-b[2]); }
	friend Vector3f operator * (const Vector3f& a, const float d) { return Vector3f(a[0]*d,a[1]*d,a[2]*d); }
	friend Vector3f operator * (const float d, const Vector3f& a) { return Vector3f(a[0]*d,a[1]*d,a[2]*d); }

	friend float operator * (const Vector3f& a, const Vector3f& b) { return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]; }    // dot product
	friend Vector3f operator / (const Vector3f& a, const float d) { float od=1.0f/d; return Vector3f(a[0]*od,a[1]*od,a[2]*od); }
	friend Vector3f operator ^ (const Vector3f& a, const Vector3f& b) { return Vector3f(a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], a[0]*b[1]-a[1]*b[0]); }
	friend int operator == (const Vector3f& a, const Vector3f& b) { return (a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]); }
	friend int operator != (const Vector3f& a, const Vector3f& b) { return (a[0] != b[0]) || (a[1] != b[1]) || (a[2] != b[2]); }

	friend void swap(Vector3f& a, Vector3f& b) { Vector3f tmp=a; a=b; b=tmp; }
};

class OVIS_API Vector2f:public base::Float2 {
public:
	Vector2f() { m_Floats[0]=0.0f; m_Floats[1]=0.0f; }
	Vector2f(const float x,const float y) { m_Floats[0]=x; m_Floats[1]=y; }
	Vector2f(const Float2 &vec) { m_Floats[0]=vec[0]; m_Floats[1]=vec[1]; }
	Vector2f(const Vector2f &vec) { m_Floats[0]=vec[0]; m_Floats[1]=vec[1]; }

	inline Vector2f& operator += ( const Vector2f& v ) { m_Floats[0]+=v[0]; m_Floats[1]+=v[1]; return *this;  }
	inline Vector2f& operator -= ( const Vector2f& v ) { m_Floats[0]-=v[0]; m_Floats[1]-=v[1]; return *this;  }
	inline Vector2f& operator *= ( const float d ) { m_Floats[0]*=d; m_Floats[1]*=d; return *this;  }
	inline Vector2f& operator /= ( const float d ) { m_Floats[0]/=d; m_Floats[1]/=d; return *this;  }
	//inline float member(int i) const { return m_Floats[i]; }
	//inline float& operator[](int i) { return m_Floats[i]; }
	//inline float Vector2f::operator[](int i) const { return m_Floats[i]; }
	inline float& x() { return m_Floats[0]; }
	inline float x() const { return m_Floats[0]; }
	inline float& y() { return m_Floats[1]; }
	inline float y() const { return m_Floats[1]; }
	
	float lengthSquared() const { return m_Floats[0]*m_Floats[0]+m_Floats[1]*m_Floats[1]; }
	float length() const { return sqrtf(lengthSquared()); }
	Vector2f& normalize();
	Vector2f normalizedVector() const;

	friend Vector2f operator - (const Vector2f& v) { return Vector2f(-v[0],-v[1]); }
	friend Vector2f operator + (const Vector2f& a, const Vector2f& b) { return Vector2f(a[0]+b[0],a[1]+b[1]); }
	friend Vector2f operator - (const Vector2f& a, const Vector2f& b) { return Vector2f(a[0]-b[0],a[1]-b[1]); }
	friend Vector2f operator * (const Vector2f& a, const float d) { return Vector2f(a[0]*d,a[1]*d); }
	friend Vector2f operator * (const float d, const Vector2f& a) { return Vector2f(a[0]*d,a[1]*d); }

	friend float operator * (const Vector2f& a, const Vector2f& b) { return a[0]*b[0]+a[1]*b[1]; }    // dot product
	friend Vector2f operator / (const Vector2f& a, const float d) { float od=1.0f/d; return Vector2f(a[0]*od,a[1]*od); }
	friend Vector3f operator ^ (const Vector2f& a, const Vector2f& b) { return Vector3f(0.0f, 0.0f, a[0] * b[1] - b[0] * a[1]); }
	friend int operator == (const Vector2f& a, const Vector2f& b) { return (a[0] == b[0]) && (a[1] == b[1]); }
	friend int operator != (const Vector2f& a, const Vector2f& b) { return (a[0] != b[0]) || (a[1] != b[1]); }

	friend void swap(Vector2f& a, Vector2f& b) { Vector2f tmp=a; a=b; b=tmp; }
};

}
}

#endif
