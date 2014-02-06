/**
 * 
 *    \file VertexBuffer.cpp  
 * 
 */
 

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "VertexBuffer.hpp"


    
void VertexBuffer::generate()
{
  if (!m_vao) glGenVertexArrays( 1, &m_vao);
  if (!m_vbo) glGenBuffers( 1, &m_vbo);
}

void VertexBuffer::destroy()
{
  if (m_vao) glDeleteVertexArrays( 1, &m_vao);
  if (m_vbo) glDeleteBuffers( 1, &m_vbo);
  
  cleanData();
}

void VertexBuffer::cleanData()
{
  m_position.clear();
  m_normal.clear();
  m_texcoord.clear();
}

void VertexBuffer::complete(GLenum usage)
{
  assert( m_vao && m_vbo );
  
  
  m_positionSize = m_normalSize = m_texcoordSize = 0;  
  if (!m_position.empty()) m_positionSize = m_position.size() * sizeof(glm::vec3);
  if (!m_normal.empty()) m_normalSize = m_normal.size() * sizeof(glm::vec3);
  if (!m_texcoord.empty()) m_texcoordSize  = m_texcoord.size() * sizeof(glm::vec2);  
  
  GLsizeiptr bufferSize = m_positionSize + m_normalSize + m_texcoordSize;
    
  
  bind();
  {    
    glBufferData( GL_ARRAY_BUFFER, bufferSize, 0, usage);
      
    m_offset = 0;
    
    if (!m_position.empty())
    {
      glBufferSubData( GL_ARRAY_BUFFER, m_offset, m_positionSize, &m_position[0]);
      glVertexAttribPointer( VATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void*)(m_offset));
      m_offset += m_positionSize;
    }
    
    if (!m_normal.empty())
    {
      glBufferSubData( GL_ARRAY_BUFFER, m_offset, m_normalSize, &m_normal[0]);
      glVertexAttribPointer( VATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, (void*)(m_offset));
      m_offset += m_normalSize;
    }
    
    if (!m_texcoord.empty())
    {
      glBufferSubData( GL_ARRAY_BUFFER, m_offset, m_texcoordSize, &m_texcoord[0]);
      glVertexAttribPointer( VATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, (void*)(m_offset));  
      m_offset += m_texcoordSize;
    }
  }
  unbind();
}


void VertexBuffer::bind() const
{
  glBindVertexArray( m_vao );
  glBindBuffer( GL_ARRAY_BUFFER, m_vbo);
}

void VertexBuffer::unbind()
{
  glBindBuffer( GL_ARRAY_BUFFER, 0u);
  glBindVertexArray( 0u );
}

void VertexBuffer::enable() const
{  
  bind();
  
  if (m_positionSize != 0)  glEnableVertexAttribArray( VATTRIB_POSITION );
  if (m_normalSize != 0)    glEnableVertexAttribArray( VATTRIB_NORMAL );
  if (m_texcoordSize != 0)  glEnableVertexAttribArray( VATTRIB_TEXCOORD );
}

void VertexBuffer::disable()
{    
 	glDisableVertexAttribArray( VATTRIB_POSITION );
  glDisableVertexAttribArray( VATTRIB_NORMAL );
  glDisableVertexAttribArray( VATTRIB_TEXCOORD );
  
  unbind();
}
