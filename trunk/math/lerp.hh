#ifndef OVIS_MATH_LERP_HH_INCLUDED
#define OVIS_MATH_LERP_HH_INCLUDED

#include "interpolator.hh"

namespace ovis {
namespace math {

	class OVIS_API LinearInterpolator:public Interpolator
	{
	public:
		LinearInterpolator();
		~LinearInterpolator();

		void calculateInterpolations(const float position);
	};

}
}

#endif
