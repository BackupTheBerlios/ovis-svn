#ifndef OVIS_MATH_INTERPOLATOR_HH_INCLUDED
#define OVIS_MATH_INTERPOLATOR_HH_INCLUDED

#include "../base/dll.hh"

namespace ovis {
namespace math {

	class OVIS_API Keyframearray
	{
	public:
		virtual ~Keyframearray();

		virtual unsigned long numKeyframes() const=0;
		virtual float position(const unsigned long keyframenr)=0;
		void nearestTwoKeyframes(const float frameposition,unsigned long &keyframe1,unsigned long &keyframe2);

		// These are for the spline interaction with the keyframes
		// For example, linear interpolation between two frames:
		// position 0.7   keyframe1 at 0.3 keyframe2 at 0.8
		// (0.7-0.3) / (0.8-0.3) = 0.4/0.5 = 0.8
		// clearResult();
		// add(keyframe1,1-0.8);
		// add(keyframe2,0.8);

		virtual void clearResult()=0;
		virtual void add(const unsigned long keyframenr,const float weight)=0;

	protected:
		Keyframearray();
	};

	class OVIS_API Interpolator
	{
	public:
		virtual ~Interpolator();

		void addKeyframearray(Keyframearray& array);
		void removeKeyframearray(Keyframearray& array);
		unsigned long numKeyframearrays() const;
		Keyframearray& keyframearray(const unsigned long index);
		const Keyframearray& keyframearray(const unsigned long index) const;

		virtual void calculateInterpolations(const float position)=0;

	protected:
		struct Internaldata;
		Internaldata *m_pInternaldata;

		Interpolator();
	};

}
}

#endif
