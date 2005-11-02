#ifdef WIN32
#define VC_EXTRALEAN
#include <windows.h>
#include <mmsystem.h>
#else
#include <sys/time.h>
#endif
#include "timer.hh"

namespace ovis {
namespace base {

	Timer::Timer(bool adjust_PivotTime):m_PivotTime(0),m_LastTime(0),m_CurrTime(0),m_Frequency(0),m_Difference(0)
	{
	#ifdef WIN32
		LARGE_INTEGER lint;
		QueryPerformanceFrequency(&lint);
		m_Frequency=(ovis_int64)lint.QuadPart;
	#else
		m_Frequency=(ovis_int64)1000000;
	#endif

		getTime();
		if (adjust_PivotTime) adjustPivotTime();
	}

	void Timer::getTime()
	{
	#ifndef WIN32
		struct timeval tv;
	#else
		LARGE_INTEGER lint;
	#endif

	#ifdef WIN32
		QueryPerformanceCounter(&lint);
		m_CurrTime=lint.QuadPart;
	#else
		gettimeofday(&tv,0);
		m_CurrTime=(((ovis_int64)tv.tv_sec)*(ovis_int64)1000000)+((ovis_int64)tv.tv_usec);
	#endif
	}

	void Timer::update()
	{
		m_LastTime=m_CurrTime;
		getTime();
		m_Difference=m_CurrTime-m_LastTime; if (m_Difference<0) m_Difference=0;	
	}

}
}
