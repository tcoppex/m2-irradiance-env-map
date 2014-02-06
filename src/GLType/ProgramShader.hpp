/**
 * 
 *    \file ProgramShader.hpp  
 * 
 * 
 *    \note depends on GLSW to load shaders
 * 
 *    \todo # Use a Shader type to avoid compile multiple time the same ones ?
 *          # Separate loading / compiling ?
 */
 

#pragma once

#ifndef PROGRAMSHADER_HPP
#define PROGRAMSHADER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>


class ProgramShader
{
  protected:
    GLuint m_id;
    //std::string m_name;    
    
  public:
    ProgramShader() : m_id(0u) {}
    virtual ~ProgramShader() {destroy();}
    
    /** Generate the program id */
    void generate();
    
    /** Destroy the program id */
    void destroy();        
    
    /** Add a shader and compile it */
    void addShader(GLenum shaderType, const std::string &tag);
    //void addShader(Shader *shader);    
    
    //bool compile(); //static (with param)?
    
    bool link(); //static (with param)?
    
    inline
    void bind() const { glUseProgram( m_id ); }
    
    static inline
    void unbind() { glUseProgram( 0u ); }
    
    /** Return the program id */
    inline
    GLuint getId() const {return m_id;}
    
    bool setUniform( const std::string &name, GLint v) const;
    bool setUniform( const std::string &name, GLfloat v) const;
    bool setUniform( const std::string &name, const glm::vec3 &v) const;
    bool setUniform( const std::string &name, const glm::vec4 &v) const;
    bool setUniform( const std::string &name, const glm::mat3 &v) const;
    bool setUniform( const std::string &name, const glm::mat4 &v) const;
    // add array type..  
  
  private:
    //ProgramShader(const ProgramShader &) {}
    //ProgramShader& operator =(const ProgramShader &) const {}
};


#endif //PROGRAMSHADER_HPP


/*******************************************************************************

// HOW TO USE IT :

ProgramShader program;

program.generate();
  program.addShader( GL_VERTEX_SHADER, "PassThrough.Vertex");
  program.addShader( GL_FRAGMENT_SHADER, "PassThrough.Fragment");
program.link();

program.bind();
  program.setUniform( "uModelViewProjMatrix", mvpMatrix);
  drawObject();
program.unbind();

program.destroy();

*******************************************************************************/
