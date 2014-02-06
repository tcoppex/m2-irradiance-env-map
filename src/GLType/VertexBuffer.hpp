/**
 * 
 *    \file VertexBuffer.hpp  
 * 
 *    \todo # allow index buffer
 *          # allow interleaving
 *          # add tangent ?
 */
 

#pragma once

#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>


enum VertexAttribLocation
{
  VATTRIB_POSITION = 0,
  VATTRIB_NORMAL,
  VATTRIB_TEXCOORD
};

class VertexBuffer
{
  protected:
    GLuint m_vao;
    GLuint m_vbo;    
    
    //GLuint m_ibo;
    //std::vector<GLuint> m_index;
    
    std::vector<glm::vec3> m_position;
    std::vector<glm::vec3> m_normal;
    std::vector<glm::vec2> m_texcoord;
        
    GLsizeiptr m_positionSize;
    GLsizeiptr m_normalSize;
    GLsizeiptr m_texcoordSize;
    
    GLintptr m_offset;
    

  public:
    VertexBuffer() 
      : m_vao(0u), m_vbo(0u),
        m_positionSize(0), m_normalSize(0), m_texcoordSize(0),
        m_offset(0)
    {}
                     
    virtual ~VertexBuffer() { destroy(); }
    
    void generate();
    void destroy();

    /** Destroy the client side memory (CPU) */
    void cleanData();

    /** Set the VAO parameters & send data to the GPU */
    void complete(GLenum usage);
    
    void bind() const;        
    static void unbind();
    
    /** Enable vertex attribs arrays (for rendering) */
    void enable() const;
    
    /** Disable vertex attribs arrays */
    static void disable();    
    
    
    GLuint getVBO() const {return m_vbo;}
    //GLuint getIBO() const {return m_ibo;}
    
    std::vector<glm::vec3>& getPosition() {return m_position;}
    std::vector<glm::vec3>& getNormal() {return m_normal;}
    std::vector<glm::vec2>& getTexcoord() {return m_texcoord;}
    
    GLintptr getOffset() const { return m_offset; }
};


/*******************************************************************

  // HOW TO USE :

  VertexBuffer vertexBuffer;

  std::vector<glm::vec3> &pos = vertexBuffer.getPosition();
  std::vector<glm::vec3> &nor = vertexBuffer.getNormal();
  std::vector<glm::vec2> &tex = vertexBuffer.getTexcoord();

  // Update pos, nor, tex
  // ..

  // Generate buffer's id
  m_vertexBuffer.generate();  

  // Send data to the GPU
  m_vertexBuffer.complete( GL_STATIC_DRAW );

  // Remove data from the CPU [optional]
  m_vertexBuffer.cleanData();

********************************************************************/

#endif //VERTEXBUFFER_HPP
