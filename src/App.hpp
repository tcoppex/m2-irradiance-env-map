/**
 * 
 *    \file App.hpp  
 * 
 */
 

#pragma once

#ifndef APP_HPP
#define APP_HPP

#include <vector>
#include <GL/glew.h>
#include <GLType/ProgramShader.hpp>
#include "SkyBox.hpp"

class TCamera;
class Mesh;

class App
{
  protected:
    bool m_bInitialized;
    
    TCamera *m_pCamera;
    
    SkyBox m_skyBox;
    ProgramShader m_envMapProgram;
    Mesh *m_Mesh;
    
  
  public:
    App();
    ~App();
    
    void init(TCamera *camera);
    void update();
    void render();
    void keyEvent(unsigned char);

  protected:
    void _renderScene();
};


#endif //APP_HPP
