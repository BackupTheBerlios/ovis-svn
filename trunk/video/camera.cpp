#include "camera.hh"

namespace ovis {
namespace video {

	Camera::Camera(const base::String& identifier):Transformnode(identifier),m_FrustumUpToDate(false)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::spawn(base::Streamable& datasource)
	{
	}

	void Camera::serialize(base::Streamable& datasink) const
	{
	}

	const char* Camera::type() const
	{
		return "Camera";
	}

	/*void Camera::invalidateHWMatrices()
	{
		m_UpToDate=false;
		Transformnode::invalidateHWMatrices();
	}*/

	void Camera::update()
	{
		if (!worldmatrixIsValid()) m_Viewmatrix=worldmatrix().invert();
		if (!m_FrustumUpToDate) { m_Frustum.extract(m_Projmatrix,m_Viewmatrix); m_FrustumUpToDate=true; }
	}

	const math::Matrix4f& Camera::worldmatrix() const
	{
		bool b=worldmatrixIsValid();
		const math::Matrix4f& retm=Transformnode::worldmatrix();
		if (!b) m_Viewmatrix=retm.invert();

		return retm;
	}

	const math::Matrix4f& Camera::viewmatrix() const
	{
		if (!worldmatrixIsValid()) m_Viewmatrix=worldmatrix().invert();
		if (!m_FrustumUpToDate) { m_Frustum.extract(m_Projmatrix,m_Viewmatrix); m_FrustumUpToDate=true; }
		return m_Viewmatrix;
	}

	const math::Matrix4f& Camera::projmatrix() const
	{
		return m_Projmatrix;
	}

	math::Matrix4f& Camera::projmatrix()
	{
		m_FrustumUpToDate=false;
		return m_Projmatrix;
	}

	void Camera::projmatrix(const math::Matrix4f& matrix)
	{
		m_FrustumUpToDate=false;
		m_Projmatrix=matrix;
	}

	const math::Frustum& Camera::frustum() const
	{
		if (!m_FrustumUpToDate) { m_Frustum.extract(m_Projmatrix,m_Viewmatrix); m_FrustumUpToDate=true; }
		return m_Frustum;
	}

	bool Camera::isValid() const
	{
		return true;
	}

}
}
