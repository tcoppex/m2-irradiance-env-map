/**
 * 
 *    \file SkyBox.cpp
 * 
 * 
 */
 
#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tools/TCamera.hpp>
#include <tools/gltools.hpp>
#include <tools/Timer.hpp>
#include <GLType/ProgramShader.hpp>
#include <GLType/Texture.hpp>
#include "Mesh.hpp"
#include "SkyBox.hpp"

    
SkyBox::~SkyBox()
{
  if (0 != m_Program) delete m_Program;
  if (0 != m_CubeMesh) delete m_CubeMesh;
    
  if (false == m_cubemaps.empty())
  {
    std::vector<TextureCubemap*>::iterator it;
    for (it=m_cubemaps.begin(); it!=m_cubemaps.end(); ++it)
    {
      delete *it;
    }
  }
}

void SkyBox::init()
{
  assert( false == m_bInitialized );
  
  // Create & load the CubeMap program
  m_Program = new ProgramShader();  
  m_Program->generate();
    m_Program->addShader( GL_VERTEX_SHADER, "SkyBox.Vertex" );
    m_Program->addShader( GL_FRAGMENT_SHADER, "SkyBox.Fragment" );
  m_Program->link();
  
  // Create the cube mesh
  m_CubeMesh = new CubeMesh();
  m_CubeMesh->init();
  
  // [optionnal] clear the vector  
  m_cubemaps.clear();
  
  m_bInitialized = true;
}

void SkyBox::render(const TCamera& camera)
{
  assert( m_bInitialized );  
  
  if (m_cubemaps.empty())
  {
    fprintf(stderr, "ERROR : no cubemap specified\n");
    exit(0);
  }
  
  
  glDisable( GL_DEPTH_TEST );
  glDepthMask( GL_FALSE );  
  glDisable( GL_CULL_FACE );  
  
  glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );  
  
  m_Program->bind();
  {    
    //-------------------------------------------------
    if (m_bAutoRotation)
    {
      m_spin = fmodf( m_spin+0.1f, 360.0f);
      glm::vec3 axis = glm::vec3( 1.0f, 0.7f, -0.5f );
      
      m_rotateMatrix = glm::rotate( glm::mat4(1.0f), m_spin, axis);
      m_invRotateMatrix = glm::mat3( glm::rotate( glm::mat4(1.0f), -m_spin, axis) );
    }
    //-------------------------------------------------
    
    // Vertex uniform
    glm::mat4 followCamera = glm::translate( glm::mat4(1.0f), camera.getPosition());
    glm::mat4 model = m_CubeMesh->getModelMatrix() * followCamera * m_rotateMatrix;
    glm::mat4 mvp = camera.getViewProjMatrix() * model;    
    m_Program->setUniform( "uModelViewProjMatrix", mvp);
    
    // Fragment uniform
    m_Program->setUniform( "uCubemap", 0);
    
    m_cubemaps[m_curIdx]->bind( 0u );
      m_CubeMesh->draw();    
    m_cubemaps[m_curIdx]->unbind( 0u );
  }
  m_Program->unbind();
  
  glDisable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
     
  //glEnable( GL_CULL_FACE );
  glDepthMask( GL_TRUE );
  glEnable( GL_DEPTH_TEST ); 
  
  CHECKGLERROR();
}

void SkyBox::addCubemap( const std::string &name )
{
  assert( m_bInitialized );
  
  TextureCubemap *cubemap = new TextureCubemap();
  cubemap->generate();
  cubemap->load( name );  
  m_cubemaps.push_back( cubemap );
}

/*
void SkyBox::addCubemap( TextureCubemap *cubemap )
{
  assert( m_bInitialized && (0 != cubemap) );
  m_cubemaps.push_back( cubemap );
}
*/

bool SkyBox::setCubemap( size_t idx )
{
  assert( m_bInitialized );
  
  if (m_cubemaps.empty() || (idx >= m_cubemaps.size()))
  {
    fprintf( stderr, "SkyBox : Cubemap index out of range.\n");
    return false;
  }
  
  m_curIdx = idx;
  return true;
}
