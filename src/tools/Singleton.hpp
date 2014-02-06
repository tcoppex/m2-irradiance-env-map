/**
 * 
 *    \file Singleton.hpp
 * 
 *    Abstract class used to define a Singleton object.
 *    Not thread safe.
 * 
 */

#pragma once

#ifndef SINGLETON_HPP
#define SINGLETON_HPP


template <class T>
class Singleton
{
  private:
    static T* sm_Instance;    
      
  public:
    inline
    static T& getInstance()
    {
      if (!sm_Instance) {
        sm_Instance = new T;
      }
      
      return *sm_Instance;
    }
    
    inline
    static void deleteInstance()
    {
      if (sm_Instance)
      {
        delete sm_Instance;
        sm_Instance = 0;
      }
    }
    
  protected:
    Singleton() {}
    ~Singleton() {deleteInstance();}
    
  private:
    Singleton(Singleton&);
};


template<class T> T* Singleton<T>::sm_Instance = 0;


#endif //SINGLETON_HPP
