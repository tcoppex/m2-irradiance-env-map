/**
 *
 *        \file gltools.hpp
 * 
 *        Some utility function for OpenGL
 *
 */


#pragma once

#ifndef __GLTOOLS_HPP__
#define __GLTOOLS_HPP__

#include <GL/glew.h>
#include <glm/glm.hpp>


#ifndef NDEBUG
  #define CHECKGLERROR()    gltools::checkGLError( __FILE__, __LINE__, "", true);
  #define CHECKVALUE(x)     gltools::checkValue( __FILE__, __LINE__, #x, x);
#else
  #define CHECKGLERROR()
  #define CHECKVALUE(x)
#endif


namespace gltools
{
  const   char* getGLErrorString(GLenum err);
  void    checkGLError(const char *file, const int line, const char *errMsg="", bool bExitOnFail=false);
  GLint   getShaderError( GLuint shader, GLenum pname );
  GLvoid  printShaderLog(GLuint shader);
  GLint   getProgramError( GLuint program, GLenum pname );
  GLvoid  printProgramLog( GLuint program );  
    
  GLuint  createShader( GLenum shaderType, const char *shaderTag);
  
  
  // Handle some glUniform function
  void    setUniform( GLuint program, const char *name, int value);
  void    setUniform( GLuint program, const char *name, float value);
  void    setUniform( GLuint program, const char *name, const glm::vec3 &value);
  void    setUniform( GLuint program, const char *name, const glm::vec4 &value);
  void    setUniform( GLuint program, const char *name, const glm::mat3 &value);
  void    setUniform( GLuint program, const char *name, const glm::mat4 &value);
  
  void    checkValue(const char *file, const int line, const char *name, bool value);
}

#endif //__GLTOOLS_HPP__
