#ifndef OVIS_MATH_FRUSTUM_HH_INCLUDED
#define OVIS_MATH_FRUSTUM_HH_INCLUDED

#include "matrix.hh"
#include "plane.hh"
#include "vectors.hh"
#include <vector>
#include "volume.hh"

namespace ovis {
namespace math {
	
	//! View-frustum class.
	/*! A frustum is a pyramidal-shaped object which defines which part of the space is visible
	to the viewer. Several methods are available for testing whether geometric shapes are in the
	frustum, thus enabling view-frustum culling.
	*/

	class OVIS_API Frustum:public Volume
	{
	public:
		bool intersectsWith(const SphereVolume& sphere) const;
		bool intersectsWith(const AABBVolume& aabb) const;
		bool intersectsWith(const OBBVolume& obb) const;
		inline bool intersectsWith(const Frustum& frustum) const { return false; }

		void projectToRectangle(const Matrix4f& projectionMatrix,Rectangle& rectangle) const {}

		//! Results obtained from testing a geometric shape against the frustum.
		enum CullingResult {
			NotInFrustum, //!< Shape is not in the frustum and thus not visible.
			PartialIntersection, //!< Shape is partially in the frustum.
			FullInclusion //!< Shape is fully included in the frustum.
		};

		//! Checks whether the specified point is in the frustum or not.
		/*! @param Point The point to be checked for visibility. */
		CullingResult checkPoint(const Vector3f &Point) const;
		//! Checks whether the specified sphere is in the frustum or not.
		/*!
		@param center Center of the sphere.
		@param radius Radius of the sphere.
		*/
		CullingResult checkSphere(const Vector3f &center,const float radius) const;
		//! Checks whether the specified AABB is in the frustum or not.
		/*!
		@param vec1 The min corner of the AABB.
		@param vec2 The max corner of the AABB.
		*/
		CullingResult checkAABB(const Vector3f &vec1,const Vector3f &vec2) const;
		//! Checks whether the specified polygon is in the frustum or not.
		/*!
		@param points Array of points describing the polygon.
		@param numPoints Amount of points.
		*/
		CullingResult checkPolygon(const Vector3f *points,const int numPoints) const;

		//! Build a frustum from the specified projection- view- and worldmatrices.
		/*!
		@param projmatrix The projection matrix.
		@param viewmatrix The view matrix.
		*/
		void extract(const Matrix4f &projmatrix,const Matrix4f &view);
		//! Builds a frustum using a polygon, a view- and a worldmatrix.
		/*!
		@param points Array of points describing the polygon.
		@param numPoints Amount of points.
		@param viewmatrix The view matrix.
		*/
		void buildFromPolygon(const Vector3f *points,const int numPoints,const Matrix4f &view);

		Frustum();
		~Frustum();

		//! Amount of planes describing the frustum.
		inline size_t planeCount() { return planes->size(); }
		//! Retrieves a plane by its index.
		inline Plane& plane(int planeindex) { return (*planes)[planeindex]; }
		//! Retrieves a plane by its index.
		inline const Plane& plane(int planeindex) const { return (*planes)[planeindex]; }

	protected:
		std::vector <Plane> *planes;
	};

}
}

#endif
