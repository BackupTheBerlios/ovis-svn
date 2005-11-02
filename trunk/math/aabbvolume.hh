#ifndef OVIS_MATH_AABBVOLUME_HH_INCLUDED
#define OVIS_MATH_AABBVOLUME_HH_INCLUDED

#include "../base/dll.hh"
#include "vectors.hh"
#include "volume.hh"

namespace ovis {
namespace math {

class SphereVolume;
class OBBVolume;
class Frustum;
class Rectangle;

class OVIS_API AABBVolume:public Volume
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

	inline const Vector3f& extents() const { return m_Extents; }
	inline void extents(const Vector3f &v) { m_Extents=v; }

	inline Vector3f minCorner() const { return m_Center-m_Extents*0.5f; }
	inline Vector3f maxCorner() const { return m_Center+m_Extents*0.5f; }
protected:
	Vector3f m_Center,m_Extents;
};

}
}

#endif


				  
