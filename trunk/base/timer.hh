#ifndef OVIS_BASE_TIMER_HH_INCLUDED
#define OVIS_BASE_TIMER_HH_INCLUDED

#include "itypes.hh"
#include "dll.hh"

namespace ovis {
namespace base {

	class OVIS_API Timer {
	protected:
		ovis_int64 m_PivotTime,m_LastTime,m_CurrTime,m_Frequency,m_Difference;
		
		void getTime();
	public:
		Timer(bool adjust_PivotTime=true);
		virtual ~Timer() {}
		virtual void update();

		inline void adjustPivotTime() { m_PivotTime=m_CurrTime; }
		inline void pivotTime(const ovis_int64 newPivotTime) { m_PivotTime=newPivotTime; }

		inline ovis_int64 pivotTime() const { return m_PivotTime; }
		inline ovis_int64 lastTime() const { return m_LastTime; }
		inline ovis_int64 currTime() const { return m_CurrTime; }
		inline ovis_int64 lastTimeRel() const { return m_LastTime-m_PivotTime; }
		inline ovis_int64 currTimeRel() const { return m_CurrTime-m_PivotTime; }

		inline ovis_int64 frequency() const { return m_Frequency; }
		inline ovis_int64 difference() const { return m_Difference; }
	};

}
}

#endif
