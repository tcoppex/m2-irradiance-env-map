/**
 * 
 *    \file Texture.hpp  
 * 
 *    (Work in Progress)
 *    OpenGL Texture handler for 2D / Cubemap target
 * 
 *    look at nvidia-texture-tools for special processing.
 * 
 *    todo : # methods to change sampler parameters
 *           # handle mipmapping
 *           # handle anistropic filtering
 *           # keep image data in the same structure ?
 *           # better loading process (for cubemaps)
 *           # replace some 'assert' by a more appropriate error handler
 *           # handle 1D / 3D ? ( ~ :S )
 *           # auto 'generate()' in 'load()' ?
 * 
 */
 

#pragma once

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>


/** TEXTURE ----------------------------------------- */

class Texture
{
  protected:
    GLuint m_id;  
  
  public:
    Texture() : m_id(0u) {}
    //explicit Texture(const std::string &name) { Texture(); load(name); }
    
    virtual ~Texture() {destroy();}
    
    
    /** Return the texture target */
    virtual GLenum getTarget() const { return GL_INVALID_ENUM; }
    
    /** Generate the texture id */
    void generate();
    
    /** Destroy the texture id */
    void destroy();
    
    /** Load the texture data from a file */
    virtual bool load(const std::string &name)  { return false; }
    
    /** Return the texture id */
    GLuint getId() const {return m_id;}
  
    /** Bind the texture to the specified unit */
    void bind(GLuint unit=0u) const;
    
    /** Unbind the texture from the specified unit */
    void unbind(GLuint unit=0u) const;
    
    //void setParameteri( GLenum pname, GLint param);
    //void setParameterf( GLenum pname, GLfloat param);
    
    //mipmapping
    //anisotropic filtering


  private:
    //Texture(const Texture &) {}
    //Texture& operator =(const Texture &) const {}
};



/** TEXTURE 2D -------------------------------------- */

class Texture2D : public Texture
{
  protected:
    
  public:
    Texture2D() : Texture() {}    
    virtual GLenum getTarget() const { return GL_TEXTURE_2D; }
    virtual bool load(const std::string &name);
};



/** TEXTURE CUBEMAP --------------------------------- */

class TextureCubemap : public Texture
{
  protected:
    /** Precomputed Spherical Harmonics coefficients matrix
     *  diagonal matrix <=> only 3 * 10 values are used.
     *  Alternatively we can store only the coefficient.*/
    glm::mat4 m_shMatrix[3];
    // This is a temporary solution, ultimately the matrices will be computed
    // outside the object, but as the textures are deleted after the loading, 
    // it is not practical atm.
    
    bool m_bIrradiancePrecomputed;
  
  public:
    TextureCubemap() : Texture(), m_bIrradiancePrecomputed(false) {}
    
    virtual GLenum getTarget() const { return GL_TEXTURE_CUBE_MAP; }    
    virtual bool load(const std::string &name);
    
    bool hasSphericalHarmonics() {return m_bIrradiancePrecomputed;}
    glm::mat4* getSHMatrices() { return m_shMatrix; }
};


#endif //TEXTURE_HPP
