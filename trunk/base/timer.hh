#ifndef OVIS_BASE_TIMER_HH_INCLUDED
#define OVIS_BASE_TIMER_HH_INCLUDED

#include "itypes.hh"
#include "dll.hh"

namespace ovis {
namespace base {

	//! Timer handling class.
	/**
	* This class handles precise timing. Actual implementation varies with the platform.
	* In Windows, the performance timer is used, in unix, gettimeofday() is used.
	* Additionally, it includes utilities like the pivot time.
	* @note Timestamps are not necessarily in a known unit. If the timestamp is needed
	* in seconds, divide the timestamp with the frequency() return value.
	*/
	class OVIS_API Timer {
	protected:
		ovis_int64 m_PivotTime,m_LastTime,m_CurrTime,m_Frequency,m_Difference;
		
		void getTime();
	public:
		//! Creates a timer, with optional pivot time adjustment.
		/**
		* For more about the pivot time, look at pivotTime().
		* @param adjust_PivotTime true if the pivot time is to be adjusted (e.g. set to
		* the current time)
		*/
		Timer(bool adjust_PivotTime=true);
		virtual ~Timer() {}
		
		//! Updates the current time and calculates the time difference to the last time it was called.
		/**
		* The timer contains two timestamps; m_CurrTime and m_LastTime. When update() is called,
		* the m_CurrTime value is copied to m_LastTime, and a new m_CurrTime value is retrieved
		* from the system timer. Then, the difference between m_CurrTime and m_LastTime is calculated.
		* currTime() returns the m_CurrTime value, lastTime() the m_LastTime one, and difference()
		* returns the difference between them (making it equal to currTime()-lastTime()).
		*/
		virtual void update();

		//! Adjusts the pivot time to match the current time.
		/**
		* This is equivalent to pivotTime(currTime()).
		* For more about the pivot time, look at pivotTime().
		*/
		inline void adjustPivotTime() { m_PivotTime=m_CurrTime; }
		//! Sets the pivot time.
		/**
		* The pivot time is a chosen timestamp which acts as an offset.
		* lastTimeRel() and currTimeRel() equal lastTime() and currTime(), except that
		* the pivot time is subtracted from the returned values. The pivot time is
		* similar to the zero button in a chronometer, which sets the current time to
		* zero. It is not necessary to use it, but useful in several occasions (profiling
		* for example).
		*/
		inline void pivotTime(const ovis_int64 newPivotTime) { m_PivotTime=newPivotTime; }

		//! Returns the pivot time.
		inline ovis_int64 pivotTime() const { return m_PivotTime; }
		//! Returns the last time.
		/**
		* See update() for details.
		*/
		inline ovis_int64 lastTime() const { return m_LastTime; }
		//! Returns the current time.
		/**
		* See update() for details.
		*/
		inline ovis_int64 currTime() const { return m_CurrTime; }
		//! Returns the last time, relative to the pivot time.
		/**
		* See update() and pivotTime() for details.
		*/
		inline ovis_int64 lastTimeRel() const { return m_LastTime-m_PivotTime; }
		//! Returns the last time, relative to the pivot time.
		/**
		* See update() and pivotTime() for details.
		*/
		inline ovis_int64 currTimeRel() const { return m_CurrTime-m_PivotTime; }

		//! Returns the frequency of the timer.
		/**
		* The timestamp values are relative to the timer frequency. The return
		* value of this method specifies which value equals a second.
		* Thus, if the difference is needed in seconds, one has to calculate
		* difference() / frequency().
		*/
		inline ovis_int64 frequency() const { return m_Frequency; }
		//! Returns the difference between the current and the last time.
		/**
		* See update() for details.
		*/
		inline ovis_int64 difference() const { return m_Difference; }
	};

}
}

#endif
