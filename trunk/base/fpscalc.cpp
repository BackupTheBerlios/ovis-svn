#include "fpscalc.hh"

namespace ovis {
namespace base {

	Fpscalc::Fpscalc():Timer(true),m_FPS(0),m_SpeedFactor(1)
	{
	}

	void Fpscalc::update()
	{
		Timer::update();
		double frequency=(double)m_Frequency;
		double diff=(double)m_Difference;
		
		m_FPS=(diff==0) ? -1 : ((float)(frequency/diff));
		m_SpeedFactor=((float)(diff/frequency));
	}

}
}
