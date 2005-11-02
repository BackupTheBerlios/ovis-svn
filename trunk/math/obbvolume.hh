#ifndef OVIS_MATH_OBBVOLUME_HH_INCLUDED
#define OVIS_MATH_OBBVOLUME_HH_INCLUDED

#include "../base/dll.hh"
#include "vectors.hh"
#include "volume.hh"

namespace ovis {
namespace math {
class Matrix4f;
class SphereVolume;
class AABBVolume;
class Frustum;
class Rectangle;

class OVIS_API OBBVolume:public Volume
{
public:
	inline bool intersectsWith(const Volume &volume) const { return volume.intersectsWith(*this); }
	inline bool intersectsWith(const SphereVolume& sphere) const { return false; }
	inline bool intersectsWith(const AABBVolume& aabb) const { return false; }
	inline bool intersectsWith(const OBBVolume& obb) const { return false; }
	inline bool intersectsWith(const Frustum& frustum) const { return false; }

	void projectToRectangle(const Matrix4f& projectionMatrix,Rectangle& rectangle) const;

	inline const Vector3f& center() const { return m_Center; }
	inline void center(const Vector3f &v) { m_Center=v; m_BoxChanged=true; }

	inline const Vector3f& extents() const { return m_Extents; }
	inline void extents(const Vector3f &v) { m_Extents=v; m_BoxChanged=true; }

	inline const Vector3f& axis(const int i) const { return m_Axis[i]; }
	inline void axis(const int i,const Vector3f &v) { m_Axis[i]=v; m_BoxChanged=true; }

	inline const Vector4f& transformedCorner(const int i) const { return m_TransformedCorners[i]; }

	void transform(const Matrix4f& m);

	OBBVolume():m_BoxChanged(true) {}
protected:
	void calcCorners();

	bool m_BoxChanged;
	Vector3f m_Center,m_Extents,m_Axis[3]; // Axis order: 0 = x  1 = y  2 = z
	Vector3f m_Corners[8];
	Vector4f m_TransformedCorners[8];
};

}
}

#endif
