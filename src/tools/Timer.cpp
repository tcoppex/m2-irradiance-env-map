/**
 * 
 *        \file Timer.cpp
 * 
 * 
 */
 

#include "Timer.hpp"
#include <cstdlib>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <sys/time.h>
#endif




void Timer::start()
{
  m_startTime = getAbsoluteTime();  
  m_bEnable = true;
}

    
void Timer::update()
{
  if (!isEnable()) {
    return;
  }
  
  double lastFrameTime = m_frameTime;
  
  m_frameTime = getRelativeTime();
  m_elapsedTime = m_frameTime - lastFrameTime;  
  m_frameCount += 1u;
  
  if (m_frameTime - m_lastFPSTime >= 1000.0)
  {
    m_lastFPSTime = m_frameTime;
    m_currentFPS = m_frameCount;
    m_frameCount = 0u;
  }
}



double Timer::getAbsoluteTime()
{    
  static double appTime = 0.0;
    
  #ifdef _WIN32
  
    static LARGE_INTEGER ticksPerSecond;
    static LARGE_INTEGER t;
    
    QueryPerformanceFrequency( &ticksPerSecond );
    QueryPerformanceCounter( &t );
    
    appTime = (t.QuadPart / ticksPerSecond.QuadPart) * 1000.0;
  
  #else // UNIX && MACOSX
    
    static timeval t;
    gettimeofday( &t, NULL);
    appTime = t.tv_sec * 1000.0f + t.tv_usec * 0.001f;
    
  #endif
  
  return appTime;
}
