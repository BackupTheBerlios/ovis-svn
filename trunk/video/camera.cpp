#include "camera.hh"

namespace ovis {
namespace video {

	Camera::Camera():m_FrustumUpToDate(false)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::update()
	{
		if (!m_FrustumUpToDate) { m_Frustum.extract(m_Projmatrix,m_Viewmatrix); m_FrustumUpToDate=true; }
	}

	const math::Matrix4f& Camera::viewmatrix() const
	{
		return m_Viewmatrix;
	}

	const math::Matrix4f& Camera::projmatrix() const
	{
		return m_Projmatrix;
	}

	void Camera::viewmatrix(const math::Matrix4f& matrix)
	{
		m_FrustumUpToDate=false;
		m_Viewmatrix=matrix;
	}

	void Camera::projmatrix(const math::Matrix4f& matrix)
	{
		m_FrustumUpToDate=false;
		m_Projmatrix=matrix;
	}

	math::Matrix4f& Camera::viewmatrix()
	{
		m_FrustumUpToDate=false;
		return m_Viewmatrix;
	}

	math::Matrix4f& Camera::projmatrix()
	{
		m_FrustumUpToDate=false;
		return m_Projmatrix;
	}

	const math::Frustum& Camera::frustum() const
	{
		if (!m_FrustumUpToDate) { m_Frustum.extract(m_Projmatrix,m_Viewmatrix); m_FrustumUpToDate=true; }
		return m_Frustum;
	}

}
}
