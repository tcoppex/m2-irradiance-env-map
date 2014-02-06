/**
 * 
 *    \file Logger.hpp
 * 
 *    \todo : # add date / time, 
 *            # add suffix to the output name,
 *            # use color, 
 *            # specify if ERR / STDOUT
 *            # m_fd = stderr by default ?
 *            # hashmap to handle perFrame msg one single time ?
 */


#pragma once

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdio>
#include <string>
//#include <boost/time>
#include "Singleton.hpp"


class Logger : public Singleton<Logger>
{
  friend class Singleton<Logger>;
  
  protected:
    FILE *m_fd;    
    unsigned int m_fileSize;
    
    static const unsigned int MAX_LOGGER_FILESIZE = 32u * 1024u * 1024u; // 32 Mo    
        
  public:
    Logger() 
      : m_fd(0),
        m_fileSize(0u)
    {}
    
    ~Logger() { close(); }    
    
    void open( const std::string filename );
    void close();
    
    void write( const std::string &s, ...);
    
    /** Like write, but disable when NDEBUG not define */
    void debug( const std::string &s, ...);    
  
  private:
    Logger(const Logger&);
    Logger& operator =(const Logger&) const;
    
    void _checkFileSize();
};

// Colored terminal output
//printf("\x1b[%i;3%im COLOR \n\x1b[0m\n", intensity, color);


#endif //LOGGER_HPP
