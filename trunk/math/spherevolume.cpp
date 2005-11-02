#include "frustum.hh"
#include "spherevolume.hh"

namespace ovis {
namespace math {

bool SphereVolume::intersectsWith(const SphereVolume& sphere) const
{
	float squaredRadSum=radius()+sphere.radius();
	squaredRadSum*=squaredRadSum;

	float squaredDist=(sphere.center()-center()).lengthSquared();

	return (squaredDist<squaredRadSum);
}

bool SphereVolume::intersectsWith(const AABBVolume& aabb) const
{
	return false;
}

bool SphereVolume::intersectsWith(const OBBVolume& obb) const
{
	return false;
}

bool SphereVolume::intersectsWith(const Frustum& frustum) const
{
	return frustum.checkSphere(center(),radius())!=Frustum::NotInFrustum;
}

void SphereVolume::projectToRectangle(const Matrix4f& projectionMatrix,Rectangle& rectangle) const
{
}

}
}
