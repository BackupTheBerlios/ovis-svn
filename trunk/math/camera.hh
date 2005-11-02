#ifndef OVIS_MATH_CAMERA_HH_INCLUDED
#define OVIS_MATH_CAMERA_HH_INCLUDED

#include "../base/dll.hh"
#include "frustum.hh"

namespace ovis {
namespace math {

	class OVIS_API Camera:public Frustum
	{
	public:
		Camera();
		~Camera();

		void buildFromPolygon(const Vector3f *points,const int numPoints);

		void update();

		const Matrix4f& viewmatrix() const;
		Matrix4f& viewmatrix();
		const Matrix4f& projmatrix() const;
		Matrix4f& projmatrix();

	protected:

		void extract(const Matrix4f &projmatrix,const Matrix4f &view);
		void buildFromPolygon(const Vector3f *points,const int numPoints,const Matrix4f &view);

		Matrix4f m_Viewmatrix,m_Projmatrix;
	};

}
}

#endif
