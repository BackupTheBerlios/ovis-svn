#ifndef OVIS_MATH_PLANE_HH_INCLUDED
#define OVIS_MATH_PLANE_HH_INCLUDED

#include "../base/dll.hh"
#include "vectors.hh"

namespace ovis {
namespace math {

class OVIS_API Plane {
protected:
	Vector3f m_Normal;
	float m_Distance;
	
public:
	Plane():m_Distance(0) {}
	Plane(const Vector3f& normal,const float distance):m_Normal(normal),m_Distance(distance) {}
	Plane(const Plane& srcplane):m_Normal(srcplane.normal()),m_Distance(srcplane.distance()) {}
		
	inline Vector3f normal() const { return m_Normal; }
	inline Vector3f& normal() { return m_Normal; }
	inline void normal(const Vector3f& vec) { m_Normal=vec; }
		
	inline float a() const { return m_Normal.x(); }
	inline void a(const float newA) { m_Normal.x()=newA; }
	
	inline float b() const { return m_Normal.y(); }
	inline void b(const float newB) { m_Normal.y()=newB; }
	
	inline float c() const { return m_Normal.z(); }
	inline void c(const float newC) { m_Normal.z()=newC; }

	inline float d() const { return m_Distance; }
	inline void d(const float dist) { m_Distance=dist; }
	inline float distance() const { return m_Distance; }
	inline void distance(const float dist) { m_Distance=dist; }
	
	inline float distanceFromPlane(const Vector3f& vec) const { return (a()*vec.x()+b()*vec.y()+c()*vec.z())-m_Distance; }
};

}
}

#endif
