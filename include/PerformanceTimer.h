#ifndef _PERFORMANCETIMER_H_
#define _PERFORMANCETIMER_H_
//Dem debug things
#ifdef _WIN32
	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <malloc.h>
		#include <crtdbg.h>
		#ifndef DBG_NEW
			#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
			#define new DBG_NEW
	   #endif
	#endif
#endif

//Big nasty file full of evil delta time calculations.

//Evil Windows DT.
#ifdef _WIN32

#include <windows.h>
#include <winnt.h>
class Timer
{
public:
	Timer()
{
	QueryPerformanceCounter(&PrevTime);
}
	~Timer()
{
}

	double GetDT()
{
	QueryPerformanceCounter(&CurrTime);

	LARGE_INTEGER temp;
	LARGE_INTEGER temp2;
	QueryPerformanceFrequency(&temp);
	temp2.QuadPart = CurrTime.QuadPart - PrevTime.QuadPart;
	TimeElap = (double)((double)temp2.QuadPart / (double)temp.QuadPart);
	PrevTime = CurrTime;

	//TimeElap;

	return TimeElap;
}
protected:
	LARGE_INTEGER PrevTime;
	LARGE_INTEGER CurrTime;
	double TimeElap;
};
#endif

//Evil Linux DT.
#if defined(__linux__) || defined(__APPLE__)

#include <sys/time.h>

class Timer
{
public:
	Timer()
{
	gettimeofday(&StartTime,NULL);
}
	~Timer()
{
}

	double GetDT()
{
	gettimeofday(&TempTime, NULL);
	TimeElap = (TempTime.tv_sec - StartTime.tv_sec) * 1000000 + (TempTime.tv_usec - StartTime.tv_usec);

	StartTime = TempTime;

	TimeElap /= 1000000;

	return TimeElap;
}
protected:
	timeval TempTime;
	timeval StartTime;
	double TimeElap;
};

#endif

#endif
