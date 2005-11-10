#ifndef OVIS_BASE_FPSCALC_HH_INCLUDED
#define OVIS_BASE_FPSCALC_HH_INCLUDED

#include "timer.hh"
#include "dll.hh"

namespace ovis {
namespace base {

	//! Simple fps calculator.
		
	class OVIS_API Fpscalc:public Timer {
	protected:
		float m_FPS,m_SpeedFactor;
	public:
		Fpscalc();

		//! Call this every frame in order to calculate the speed factor and update the fps.
		void update();

		//! Retrieves the amount frames per second.
		/**
		* The frames per second are calculated using the difference() value, and calculating
		* frequency()/difference(). This calculation is performed in Fpscalc::update().
		* if difference() returns zero, this method returns -1.
		* @return The frames per second.
		*/
		inline float fps() const { return m_FPS; }
		//! Retrieves the speed factor.
		/*! The speed factor is a value indicating the ratio between the time needed
		for the frame and the timer frequency. Smaller values indicate less time needed
		for a frame and thus more frames per second. Multiplying a movement value with
		this factor results in fps-independent movement.
		* @return The speed factor.
		*/
		inline float speedFactor() const { return m_SpeedFactor; }
	};

}
}

#endif

