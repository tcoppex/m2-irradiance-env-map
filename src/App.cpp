/**
 * 
 *    \file App.cpp
 * 
 */


#include <cstdio>
#include <vector>

#include <GL/glew.h>
#include <tools/gltools.hpp>
#include <tools/TCamera.hpp>
#include <GLType/Texture.hpp>
#include "Mesh.hpp"

#include "App.hpp"



App::App()
{
  m_bInitialized = false;
  m_Mesh = 0;
}

App::~App()
{
  if (!m_bInitialized) {
    return;
  } 
  
  if (m_Mesh) delete m_Mesh;
}

void App::init(TCamera *pCamera)
{
  assert( !m_bInitialized );
  
  m_pCamera = pCamera;  
  
  /// Init SkyBox
  m_skyBox.init();
  m_skyBox.addCubemap( "data/cubemap/test/*.bmp" );
  //m_skyBox.addCubemap( "data/cubemap/GamlaStan2/*.png" );
  //m_skyBox.addCubemap( "data/cubemap/Rusted/rusted_*.bmp");
  m_skyBox.addCubemap( "data/cubemap/MountainPath/*.jpg" );  //2048 !   
  //m_skyBox.addCubemap( "data/cubemap/Grace/grace_*.bmp" );
  m_skyBox.setCubemap( 0u );  
  
  /// Init Environment mapping Program
  m_envMapProgram.generate();
    m_envMapProgram.addShader( GL_VERTEX_SHADER, "EnvMapping.Vertex");
    m_envMapProgram.addShader( GL_FRAGMENT_SHADER, "EnvMapping.Fragment");
  m_envMapProgram.link();  
  
  /// Init mesh
  m_Mesh = new SphereMesh( 48, 5.0f);
  m_Mesh->init();
  
  
  m_bInitialized = true;
}

void App::update()
{
}

void App::render()
{
  m_skyBox.render( *m_pCamera );  
  
  _renderScene();  
  
  CHECKGLERROR();
}


void App::keyEvent(unsigned char k)
{
  switch (k)
  {
    case 49:
    case 50:
      m_skyBox.setCubemap( k-49u );
    break;
    
    case 'r':
      m_skyBox.toggleAutoRotate();
    break;
  }
}


void App::_renderScene()
{
  /**
   *  Render the scene in two passes to handle transparency.
   *  Here the scene consists of an unique convex object, so it simpler.
   */
  
  glEnable(GL_BLEND);
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);

  m_envMapProgram.bind();
  {
    // Vertex uniforms
    glm::mat4 mvp = m_pCamera->getViewProjMatrix() * m_Mesh->getModelMatrix();
    m_envMapProgram.setUniform( "uModelViewProjMatrix", mvp);
    m_envMapProgram.setUniform( "uModelMatrix", m_Mesh->getModelMatrix());
    m_envMapProgram.setUniform( "uNormalMatrix", m_Mesh->getNormalMatrix());
    m_envMapProgram.setUniform( "uEyePosWS", m_pCamera->getPosition());
    m_envMapProgram.setUniform( "uInvSkyboxRotation", m_skyBox.getInvRotateMatrix() );
    
    TextureCubemap *cubemap = m_skyBox.getCurrentCubemap();
    
    if (cubemap->hasSphericalHarmonics())
    {
      glm::mat4 *M = cubemap->getSHMatrices();
      m_envMapProgram.setUniform( "uIrradianceMatrix[0]", M[0]);
      m_envMapProgram.setUniform( "uIrradianceMatrix[1]", M[1]);
      m_envMapProgram.setUniform( "uIrradianceMatrix[2]", M[2]);
    }
    
    // Fragment uniforms
    m_envMapProgram.setUniform( "uEnvmap", 0);
    
    cubemap->bind( 0u );
      glCullFace( GL_FRONT );
      m_Mesh->draw();
      glCullFace( GL_BACK );
      m_Mesh->draw();
    cubemap->unbind( 0u );
  }
  m_envMapProgram.unbind();
  
  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}
