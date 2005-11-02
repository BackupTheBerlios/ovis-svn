#ifndef OVIS_VIDEO_CAMERA_HH_INCLUDED
#define OVIS_VIDEO_CAMERA_HH_INCLUDED

#include "../base/dll.hh"
#include "../math/frustum.hh"
#include "transformnode.hh"

namespace ovis {
namespace video {

	class OVIS_API Camera:public Transformnode
	{
	public:
		Camera(const base::String& identifier);
		~Camera();

		void onVisit(const Camera& camera,Visitorqueue& visitorqueue) {}

		void spawn(base::Streamable& datasource);
		void serialize(base::Streamable& datasink) const;

		const char *type() const;

		//void invalidateHWMatrices();

		void update();

		const math::Matrix4f& worldmatrix() const;
		const math::Matrix4f& viewmatrix() const;
		const math::Matrix4f& projmatrix() const;
		math::Matrix4f& projmatrix();
		void projmatrix(const math::Matrix4f& matrix);

		const math::Frustum& frustum() const;

		bool isValid() const;

	protected:

		mutable math::Matrix4f m_Viewmatrix;
		math::Matrix4f m_Projmatrix;
		mutable math::Frustum m_Frustum;

		mutable bool m_FrustumUpToDate;
	};

}
}

#endif
