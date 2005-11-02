#ifndef OVIS_MATH_SPHEREVOLUME_HH_INCLUDED
#define OVIS_MATH_SPHEREVOLUME_HH_INCLUDED

#include "../base/dll.hh"
#include "vectors.hh"
#include "volume.hh"

namespace ovis {
namespace math {

class AABBVolume;
class OBBVolume;
class Frustum;
class Rectangle;

class OVIS_API SphereVolume:public Volume
{
public:
	inline bool intersectsWith(const Volume &volume) const { return volume.intersectsWith(*this); }
	bool intersectsWith(const SphereVolume& sphere) const;
	bool intersectsWith(const AABBVolume& aabb) const;
	bool intersectsWith(const OBBVolume& obb) const;
	bool intersectsWith(const Frustum& frustum) const;

	void projectToRectangle(const Matrix4f& projectionMatrix,Rectangle& rectangle) const;

	inline const Vector3f& center() const { return m_Center; }
	inline void center(const Vector3f &v) { m_Center=v; }

	inline float radius() const { return m_Radius; }
	inline void radius(const float radius) { m_Radius=radius; }
protected:
	Vector3f m_Center;
	float m_Radius;
};

}
}

#endif
