#ifndef OVIS_MATH_ARCBALL_HH_INCLUDED
#define OVIS_MATH_ARCBALL_HH_INCLUDED

#include "../base/dll.hh"
#include "quaternion.hh"
#include "vectors.hh"

namespace ovis {
namespace math {

class OVIS_API Arcball
{
protected:
	inline void mapToSphere(const float x,const float y,Vector3f& newvec) const;

public:
	Arcball();

	void click(const float x,const float y);
	void drag(const float x,const float y, Quaternion* newrot);

protected:
	Vector3f m_StVec,m_EnVec;
};

}
}

#endif
