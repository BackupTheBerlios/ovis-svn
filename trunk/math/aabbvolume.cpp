#include "frustum.hh"
#include "aabbvolume.hh"

namespace ovis {
namespace math {

bool AABBVolume::intersectsWith(const SphereVolume& sphere) const
{
	return false;
}

bool AABBVolume::intersectsWith(const AABBVolume& aabb) const
{
	return false;
}

bool AABBVolume::intersectsWith(const OBBVolume& obb) const
{
	return false;
}

bool AABBVolume::intersectsWith(const Frustum& frustum) const
{
	Vector3f extentsHalf=m_Extents*0.5f;
	return frustum.checkAABB(m_Center-extentsHalf,m_Center+extentsHalf)!=Frustum::NotInFrustum;
}

void AABBVolume::projectToRectangle(const Matrix4f& projectionMatrix,Rectangle& rectangle) const
{
}

}
}
