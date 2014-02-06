/**
 * 
    \file Logger.cpp
 * 
 */

#include <cstdarg>
#include <cstdio>
#include <cassert>

#include "Logger.hpp"


void Logger::open( const std::string filename )
{
  if (0 != m_fd)
  {
    fprintf( stderr, "Logger : an output file is already specify.\n");
    return;
  }
  
  m_fd = fopen( filename.c_str(), "w");
  assert( 0 != m_fd );
}

void Logger::close()
{
  if (0 != m_fd)
  {
    fclose( m_fd );
    m_fd = 0;
  }
}

void Logger::write( const std::string &format, ...)
{
  int ret=0;
   
  va_list args;
  va_start( args, format );
  
  if (0 == m_fd) {
    fprintf( stderr, "Logger : no output specify.\n");
    ret = vfprintf( stderr, format.c_str(), args);     
  } else {
    ret = vfprintf( m_fd, format.c_str(), args);
  }
  
  m_fileSize += (ret>0)?(unsigned int)(ret):0u;
  _checkFileSize();
  
  va_end( args );
}

void Logger::debug( const std::string &format, ...)
{
  #ifndef NDEBUG
  
  int ret=0;
   
  va_list args;
  va_start( args, format );
  
  if (0 == m_fd) {
    fprintf( stderr, "Logger : no output specify.\n");
    ret = vfprintf( stderr, format.c_str(), args);     
  } else {
    ret = vfprintf( m_fd, format.c_str(), args);
  }
  
  m_fileSize += (ret>0)?(unsigned int)(ret):0u;
  _checkFileSize();
  
  va_end( args );
  
  #endif //NDEBUG
}

void Logger::_checkFileSize()
{
  if (m_fileSize >= MAX_LOGGER_FILESIZE)
  {
    fprintf( stderr, 
             "\x1b[%i;3%im Logger : beware, max log size reached. File closed.\n\x1b[0m\n", 
             1, 1);
    close();
  }
}
