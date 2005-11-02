#include "camera.hh"

namespace ovis {
namespace math {

	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::buildFromPolygon(const Vector3f *points,const int numPoints)
	{
		buildFromPolygon(points,numPoints,m_Viewmatrix);
	}

	void Camera::update()
	{
		Frustum::extract(m_Projmatrix,m_Viewmatrix);
	}

	void Camera::extract(const Matrix4f &projmatrix,const Matrix4f &view)
	{
		Frustum::extract(projmatrix,view);
	}

	void Camera::buildFromPolygon(const Vector3f *points,const int numPoints,const Matrix4f &view)
	{
		Frustum::buildFromPolygon(points,numPoints,view);
	}

	const Matrix4f& Camera::viewmatrix() const
	{
		return m_Viewmatrix;
	}

	Matrix4f& Camera::viewmatrix()
	{
		return m_Viewmatrix;
	}

	const Matrix4f& Camera::projmatrix() const
	{
		return m_Projmatrix;
	}

	Matrix4f& Camera::projmatrix()
	{
		return m_Projmatrix;
	}

}
}
