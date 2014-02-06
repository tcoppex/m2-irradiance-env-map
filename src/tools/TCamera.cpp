
#include "TCamera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <cmath>
#include <algorithm>
#include <stdio.h>

#ifndef M_PI
  #define M_PI    3.14159265358979323846
#endif
#ifndef M_PI_2
  #define M_PI_2  1.57079632679489661923
#endif



TCamera::TCamera()
{
  // Default projection parameters
  m_fov   = 45.0f;
  m_zNear = 0.1f;
  m_zFar  = 1000.0f;
  setProjectionParams( m_fov, 1.0f, m_zNear, m_zFar);

  // Default view parameters
  m_position = glm::vec3(0.0f);
  m_target   = glm::vec3(0.0f, 0.0f, -1.0f);

  // Camera control
  m_pitchAngle = 0.0f;
  m_yawAngle = 0.0f;
  
  m_moveCoef = 0.5f;
  m_rotationCoef = 0.01f;
  m_inertiaCoef = 0.85f;
  
  m_bLimitPitchAngle = true;
  m_bInvertPitch = false;
  m_bInvertYaw = false;

  for (int i=0; i<NUM_CAMERAKEYS; ++i) {
    m_keydown[i] = false;
  }
  m_bHasMoved = false;
  m_bHasLooked = false;
  
  m_cursorPos = glm::vec2(0.0f);
  m_cursorOldPos = glm::vec2(0.0f);
  m_cursorDelta = glm::vec2(0.0f);
  
  m_bEnableRotation = true;
  m_bEnableMove = true;
}

void TCamera::setProjectionParams( float fov, float aspect, float zNear, float zFar)
{
  m_fov = fov;
  m_zNear = zNear;
  m_zFar = zFar;
  
  m_projectionMatrix = glm::perspective( m_fov, aspect, m_zNear, m_zFar);
  m_viewProjMatrix = m_projectionMatrix * m_viewMatrix;      
}


void TCamera::setViewParams(const glm::vec3 &pos, const glm::vec3 &target)
{
  m_position = pos;
  m_target = target;

  // .Compute the view direction 
  m_direction = m_target - m_position;
  m_direction = glm::normalize( m_direction );
  
  // .Compute the UP vector
  // treat the case where the direction vector is parallel to the Y Axis
  if ((fabs(m_direction.x) < FLT_EPSILON) && (fabs(m_direction.z) < FLT_EPSILON))
  {
    if (m_direction.y > 0.0f) {
      m_up = glm::vec3( 0.0f, 0.0f,  1.0f);
    } else {
      m_up = glm::vec3( 0.0f, 0.0f, -1.0f);
    }
  }
  else
  {
    m_up = glm::vec3( 0.0f, 1.0f, 0.0f);
  }
  
  glm::vec3 left = glm::cross( m_up, m_direction);
  left = glm::normalize( left );
  
  m_up = glm::cross( m_direction, left);
  m_up = glm::normalize( m_up );
  
  // .Create the matrix
  m_viewMatrix = glm::lookAt( m_position, m_target, m_up);
  m_viewProjMatrix = m_projectionMatrix * m_viewMatrix;


  // .Retrieve the yaw & pitch angle  
  glm::vec3 zAxis = -m_direction;  // (it's also the third row of the viewMatrix)
  
  m_yawAngle = atan2f( zAxis.x, zAxis.z);
      
  float len = sqrtf( zAxis.x*zAxis.x + zAxis.z*zAxis.z );
  m_pitchAngle = - atan2f( zAxis.y, len);
}




void TCamera::keyboardHandler(CameraKeys key, bool bPressed)
{
  if (!m_bEnableMove) {
    return;
  }
  
  m_keydown[key] = bPressed;
  
  // Setting it to true will set a m_moveVelocity to the null vector
  //when the key will be released.
  m_bHasMoved = true;  
}

void TCamera::motionHandler(int x, int y, bool bClicked)
{  
  if (bClicked)
  {
    m_cursorPos = glm::vec2( x, y);
    m_cursorDelta = glm::vec2(0.0f);
  }
  else if (m_bEnableRotation)
  {
    m_cursorOldPos = m_cursorPos;
    m_cursorPos = glm::vec2( x, y);
    m_bHasLooked = true;
  }
}

void TCamera::update(float deltaT)
{ 
  /**
  if (!m_bHasMoved && !m_bHasLooked) {
    return;
  }
  */
  
    
  // .Update camera velocity  
  if (m_bHasMoved)
  {
    m_bHasMoved = false;
    
    glm::vec3 direction = glm::vec3(0.0f);
    if (m_keydown[MOVE_RIGHT])    direction.x += 1.0f;
    if (m_keydown[MOVE_LEFT])     direction.x -= 1.0f;
    if (m_keydown[MOVE_UP])       direction.y += 1.0f;
    if (m_keydown[MOVE_DOWN])     direction.y -= 1.0f;
    if (m_keydown[MOVE_BACKWARD]) direction.z += 1.0f;
    if (m_keydown[MOVE_FORWARD])  direction.z -= 1.0f;
    
    if ((direction.x != 0.0f)||(direction.y != 0.0f)||(direction.z != 0.0f)) {
      direction = glm::normalize(direction);
    }
  
    m_moveVelocity = m_moveCoef * direction;
  }
  
  
  float inertia = m_rotationVelocity.x*m_rotationVelocity.x +
                  m_rotationVelocity.y*m_rotationVelocity.y;
  
  // .Update camera rotation
  if (m_bHasLooked || (inertia > FLT_EPSILON))
  {    
    if (m_bHasLooked)
    {
      // interpolate to avoid jaggies
      m_cursorDelta = 0.70f*m_cursorDelta + 0.30f*(m_cursorPos - m_cursorOldPos);
      
      m_rotationVelocity = m_rotationCoef * m_cursorDelta;      
    }
    else
    {
      // if the camera stop to move, add inertia.
      m_rotationVelocity *= m_inertiaCoef;
    }

    m_bHasLooked = false;

    float yawDelta = m_rotationVelocity.x;
    m_yawAngle += (m_bInvertYaw)? yawDelta : -yawDelta;
    
    float pitchDelta = m_rotationVelocity.y;
    m_pitchAngle += (m_bInvertPitch)? pitchDelta : -pitchDelta;
    
    if (m_bLimitPitchAngle)
    {
      m_pitchAngle = std::max( m_pitchAngle, float(-M_PI_2));
      m_pitchAngle = std::min( m_pitchAngle, float(+M_PI_2));
    }
  }
  
  // Compute the Rotate matrix
  // Ry(yaw) * Rx(pitch) * Rz(0.f)
  glm::mat3 cameraRotate = glm::mat3( glm::yawPitchRoll( m_yawAngle, m_pitchAngle, 0.0f) );
  
  
  
  const glm::vec3 front( 0.0f, 0.0f, -1.0f);
  m_direction = cameraRotate * front;  
  m_direction = glm::normalize( m_direction );
  
  const glm::vec3 left( -1.0f, 0.0f, 0.0f);  
  glm::vec3 worldLeft = cameraRotate * left;
            worldLeft = glm::normalize( worldLeft );  
  
  
  // .Compute the new view parameters
  m_position += cameraRotate * m_moveVelocity;  
  m_target = m_position + m_direction;
  m_up = glm::cross( m_direction, worldLeft);
  
  m_viewMatrix = glm::lookAt( m_position, m_target, m_up);
  m_viewProjMatrix = m_projectionMatrix * m_viewMatrix;
  
  /**/
  
  //Having the camera model matrix can be helpful + it holds position / target & up in
  // its columns
  //camera = view⁻¹
}

