#include "lerp.hh"

namespace ovis {
namespace math {

	LinearInterpolator::LinearInterpolator()
	{
	}

	LinearInterpolator::~LinearInterpolator()
	{
	}

	void LinearInterpolator::calculateInterpolations(const float position)
	{
		for (unsigned long k=0;k<numKeyframearrays();++k) {
			Keyframearray& array=keyframearray(k);

			unsigned long k1,k2;
			array.nearestTwoKeyframes(position,k1,k2);

			float rng1=array.position(k1);
			float rng2=array.position(k2);

			float f=(position-rng1)/(rng2-rng1);

			array.clearResult();
			array.add(k1,1.0f-f);
			array.add(k2,f);
		}
	}

}
}
