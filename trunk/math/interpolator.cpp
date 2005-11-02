#include "interpolator.hh"
#include <algorithm>
#include <vector>

namespace ovis {
namespace math {

	Keyframearray::Keyframearray()
	{
	}

	Keyframearray::~Keyframearray()
	{
	}

	void Keyframearray::nearestTwoKeyframes(const float frameposition,unsigned long &keyframe1,unsigned long &keyframe2)
	{
		float framepos=frameposition;
		if (framepos<0) framepos=0;
		else if (framepos>1) framepos=1;

		unsigned long k=0,k2=numKeyframes()-1;

		for (unsigned long i=0;i<numKeyframes();++i) {
			float f=position(i);
			if (framepos>=f) {
				k=i;
				k2=(i+1)%numKeyframes();
				
				continue;
			}
		}

		keyframe1=k;
		keyframe2=k2;
	}



	struct Interpolator::Internaldata
	{
		std::vector<Keyframearray*> m_Keyframearrays;
	};

	Interpolator::Interpolator():m_pInternaldata(new Internaldata)
	{
	}

	Interpolator::~Interpolator()
	{
		delete m_pInternaldata;
	}

	void Interpolator::addKeyframearray(Keyframearray& array)
	{
		std::vector<Keyframearray*>::iterator it=
			std::find(m_pInternaldata->m_Keyframearrays.begin(),m_pInternaldata->m_Keyframearrays.end(),&array);
		if (it==m_pInternaldata->m_Keyframearrays.end())
			m_pInternaldata->m_Keyframearrays.push_back(&array);
	}

	void Interpolator::removeKeyframearray(Keyframearray& array)
	{
		std::vector<Keyframearray*>::iterator it=
			std::find(m_pInternaldata->m_Keyframearrays.begin(),m_pInternaldata->m_Keyframearrays.end(),&array);
		if (it!=m_pInternaldata->m_Keyframearrays.end()) m_pInternaldata->m_Keyframearrays.erase(it);
	}

	unsigned long Interpolator::numKeyframearrays() const
	{
		return (unsigned long)(m_pInternaldata->m_Keyframearrays.size());
	}

	Keyframearray& Interpolator::keyframearray(const unsigned long index)
	{
		return *(m_pInternaldata->m_Keyframearrays[index]);
	}

	const Keyframearray& Interpolator::keyframearray(const unsigned long index) const
	{
		return *(m_pInternaldata->m_Keyframearrays[index]);
	}



}
}
