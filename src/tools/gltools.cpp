/**
 * 
 *      gltools.cpp
 * 
 *      few utility function to check for OpenGL / GLSL errors
 *      + a function to create shader objects using GLSW.
 * 
 */


#include "gltools.hpp"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <glsw/glsw.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace
{
  #ifndef GLENUM_TO_STR
    #define  GLENUM_TO_STR(x)   (#x)
  #endif
  
  //#define PRINTERROR
  
  
  const unsigned int BUFFER_SIZE = 512u;  
  char buffer[ BUFFER_SIZE ];

} // namespace


namespace gltools
{  
  const char* getGLErrorString(GLenum err)
  {
    switch (err)
    {
      // [GetError]
      case GL_NO_ERROR:
        return GLENUM_TO_STR(GL_NO_ERROR);
        
      case GL_INVALID_ENUM:
        return GLENUM_TO_STR(GL_INVALID_ENUM);
        
      case GL_INVALID_VALUE:
        return GLENUM_TO_STR(GL_INVALID_VALUE);
        
      case GL_INVALID_OPERATION:
        return GLENUM_TO_STR(GL_INVALID_OPERATION);
        
      case GL_STACK_OVERFLOW:
        return GLENUM_TO_STR(GL_STACK_OVERFLOW);
        
      case GL_STACK_UNDERFLOW:
        return GLENUM_TO_STR(GL_STACK_UNDERFLOW);
        
      case GL_OUT_OF_MEMORY:
        return GLENUM_TO_STR(GL_OUT_OF_MEMORY);
        
      case GL_TABLE_TOO_LARGE:
        return GLENUM_TO_STR(GL_TABLE_TOO_LARGE);
        
                
      // [CheckFramebufferStatus]
      case GL_FRAMEBUFFER_COMPLETE:
        return GLENUM_TO_STR(GL_FRAMEBUFFER_COMPLETE);
        
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        return GLENUM_TO_STR(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
        
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        return GLENUM_TO_STR(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
        
      case GL_FRAMEBUFFER_UNSUPPORTED:
        return GLENUM_TO_STR(GL_FRAMEBUFFER_UNSUPPORTED);
        
        
      default:
        return "Unknown GL Enum";
    }  
    
    return "";
  }
  
  
  void checkGLError(const char *file, const int line, const char *errMsg, bool bExitOnFail)
  {
    GLenum err = glGetError();
    
    if (err != GL_NO_ERROR)
    {
      std::cerr << "OpenGL error @ " 
                << file << "[" << line << "] : "
                << errMsg 
                << " [ " << getGLErrorString( err ) << " ] "
                << std::endl;
      
      if (bExitOnFail) {
        exit( EXIT_FAILURE );
      }
    }
  }


  GLint getShaderError( GLuint shader, GLenum pname )
  {
    GLint params;    
    glGetShaderiv( shader, pname, &params);
    return params;
  }

  GLvoid printShaderLog(GLuint shader)
  {
    glGetShaderInfoLog( shader, BUFFER_SIZE, NULL, buffer);
    if (buffer[0] != '\0')
    {
      std::cerr << "#################" << std::endl
                << buffer
                << "#################" << std::endl;
    }
  }
  
  GLint getProgramError( GLuint program, GLenum pname )
  {
    GLint params;
    
    glGetProgramiv( program, pname, &params);
    return params;
  }

  GLvoid printProgramLog( GLuint program )
  {
    // Most of the time, the buffer size will not be sufficient
    // for the Program log.
    glGetProgramInfoLog( program, BUFFER_SIZE, NULL, buffer);
    if (buffer[0] != '\0')
    {
      std::cerr << "#################" << std::endl
                << buffer
                << "#################" << std::endl;
    }
  }
  
  
  //------------------------
  
  
  GLuint createShader( GLenum shaderType, const char *shaderTag)
  {
    assert( glswGetError() == 0 );
    
    const GLchar *source = glswGetShader( shaderTag );
    
    if (0 == source) {
      fprintf( stderr, "Error : shader \"%s\" not found, check your directory.\n", shaderTag);
      fprintf( stderr, "Execution terminated.\n");
      exit( 1 );
    }

    GLuint shader = glCreateShader( shaderType );
    glShaderSource( shader, 1, (const GLchar**)&source, 0);
    glCompileShader( shader );
    
    GLint status = 0;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &status);
    
    if (status != GL_TRUE)
    {
      fprintf(stderr, "%s compilation failed.\n", shaderTag);
      printShaderLog( shader );
      exit( EXIT_FAILURE );
    }
    
    #ifndef NDEBUG
    fprintf(stderr, "%s compiled.\n", shaderTag);
    #endif
    
    return shader;
  }
  
  
  
  void setUniform( GLuint program, const char *name, int value)
  {
    GLint loc = glGetUniformLocation( program, name);
  
    if (loc != -1) {
      glUniform1i( loc, value);
    } else {
      #ifdef PRINTERROR
				fprintf( stderr, "%s : can't found \"%s\".\n", __FUNCTION__, name);
    	#endif
		}
  }
  
  void setUniform( GLuint program, const char *name, float value)
  {
    GLint loc = glGetUniformLocation( program, name);
  
    if (loc != -1) {
      glUniform1f( loc, value);
    } else {
      #ifdef PRINTERROR
				fprintf( stderr, "%s : can't found \"%s\".\n", __FUNCTION__, name);
    	#endif
		}
  }
  
  void setUniform( GLuint program, const char *name, const glm::vec3 &value)
  {
    GLint loc = glGetUniformLocation( program, name);
  
    if (loc != -1) {
      glUniform3fv( loc, 1, glm::value_ptr(value));
    } else {
      #ifdef PRINTERROR
				fprintf( stderr, "%s : can't found \"%s\".\n", __FUNCTION__, name);
    	#endif
		}
  }
  
  void setUniform( GLuint program, const char *name, const glm::vec4 &value)
  {
    GLint loc = glGetUniformLocation( program, name);
  
    if (loc != -1) {
      glUniform4fv( loc, 1, glm::value_ptr(value));
    } else {
      #ifdef PRINTERROR
				fprintf( stderr, "%s : can't found \"%s\".\n", __FUNCTION__, name);
    	#endif
		}
  }
  
  void setUniform( GLuint program, const char *name, const glm::mat3 &value)
  {
    GLint loc = glGetUniformLocation( program, name);
  
    if (loc != -1) {
      glUniformMatrix3fv( loc, 1, GL_FALSE, glm::value_ptr(value));
    } else {
      #ifdef PRINTERROR
				fprintf( stderr, "%s : can't found \"%s\".\n", __FUNCTION__, name);
    	#endif
		}
  }
  
  void setUniform( GLuint program, const char *name, const glm::mat4 &value)
  {
    GLint loc = glGetUniformLocation( program, name);
  
    if (loc != -1) {
      glUniformMatrix4fv( loc, 1, GL_FALSE, glm::value_ptr(value));
    } else {
      #ifdef PRINTERROR
				fprintf( stderr, "%s : can't found \"%s\".\n", __FUNCTION__, name);
    	#endif
		}
  }
  
    

  void checkValue(const char *file, const int line, const char *name, bool value)
  {
    if (value) {
      return;
    }
    
    std::cerr << file << ":" 
              << line << ":"
              << "Expression "
              << "'" << name << "' failed."
              << std::endl;
  }

} // namespace gltools
