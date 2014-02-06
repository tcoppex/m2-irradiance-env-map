/**
 * 
 *        \file Timer.hpp
 * 
 *
 *      Time is in millisecond.
 * 
 *      \todo # implements FPS control ?
 * 
 */
 

#pragma once

#ifndef TIMER_HPP
#define TIMER_HPP

#include "Singleton.hpp"


class Timer : public Singleton<Timer>
{
  friend class Singleton<Timer>;
  
  protected:
    double m_startTime;
    double m_frameTime;
    double m_elapsedTime;
    
    double m_lastFPSTime;
    unsigned int m_currentFPS;
    unsigned int m_frameCount;
    //unsigned int m_FPSLimit;
    
    bool m_bEnable;
    
  public:  
    Timer() : m_startTime(0.0),
              m_frameTime(0.0),
              m_elapsedTime(0.0),
              m_lastFPSTime(0.0),
              m_currentFPS(-1),
              m_frameCount(0u)
    {}
    
    /** Start the timer */
    void start();
    
    /** Update per-frame time values */
    void update();
    
    
    /** Get time from the system */
    double getAbsoluteTime();
    
    /** Get time from the start of the timer */
    inline
    double getRelativeTime() { return getAbsoluteTime() - m_startTime; }
    
    /** Get the frame start time (from the start of the timer) */
    inline
    double getFrameTime() { return m_frameTime; }
    
    /** Get time beetween two calls of update */
    inline
    double getElapsedTime() { return m_elapsedTime; }
    
    inline
    unsigned int getFPS() { return m_currentFPS; }
    
    //inline void setFPSLimit(int fpsLimit) { m_FPSLimit = fpsLimit; }


    inline
    void toggle() { m_bEnable = !m_bEnable; }
    
    inline
    bool isEnable() { return m_bEnable; }


  private:
    Timer(const Timer&);
};

#endif //TIMER_HPP
