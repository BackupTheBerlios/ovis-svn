#ifndef OVIS_VIDEO_CAMERA_HH_INCLUDED
#define OVIS_VIDEO_CAMERA_HH_INCLUDED

#include "../base/dll.hh"
#include "../math/frustum.hh"

namespace ovis {
namespace video {

	class OVIS_API Camera
	{
	public:
		Camera();
		~Camera();

		void update();

		const math::Matrix4f& viewmatrix() const;
		const math::Matrix4f& projmatrix() const;
		void viewmatrix(const math::Matrix4f& matrix);
		void projmatrix(const math::Matrix4f& matrix);
		math::Matrix4f& viewmatrix();
		math::Matrix4f& projmatrix();

		const math::Frustum& frustum() const;

	protected:

		math::Matrix4f m_Viewmatrix,m_Projmatrix;
		mutable math::Frustum m_Frustum;

		mutable bool m_FrustumUpToDate;
	};

}
}

#endif
