/**
 * 
 *      \file irradianceEnvMap.cpp
 * 
 */
 

#include "irradianceEnvMap.hpp"

#include <cstring>
#include <cmath>
#include <iostream>


namespace IrradianceEnvMap {


#define RED     0u
#define GREEN   1u
#define BLUE    2u

#define Y0(n)   (0.282095f)                             /* L_00 */
#define Y1(n)   (0.488603f*n.y)                         /* L_1-1 */
#define Y2(n)   (0.488603f*n.z)                         /* L_10 */
#define Y3(n)   (0.488603f*n.x)                         /* L_11 */
#define Y4(n)   (1.092548f*n.x*n.y)                     /* L_2-2 */
#define Y5(n)   (1.092548f*n.y*n.z)                     /* L_2-1 */
#define Y6(n)   (0.315392f*(3.0f*n.z*n.z - 1.0f))       /* L_20 */
#define Y7(n)   (1.092548f*n.x*n.z)                     /* L_21 */
#define Y8(n)   (0.546274f*(n.x*n.x - n.y*n.y))         /* L_22 */

#define IEM_TEST  0x00

#if IEM_TEST

/// Coefficients from the paper to test the implementation
float test_coeffs[3][9] =
{
  {.79, .39, -.34, -.29, -.11, -.26, -.16, .56, .21},
  {.44, .35, -.18, -.06, -.05,-.22, -.09, .21, -.05},
  {.54, .60, -.27, .01, -.12, -.47, -.15, .14,- .30}
};

#endif


static
void setIrradianceMatrices( const float shCoeff[3][9], glm::mat4 M[3]);

static
void getTexelAttrib( const int texId, const float u, const float v, const float texelSize,
                     glm::vec3 *direction, float *solidAngle);
                       
                       


void prefilter( const Image_t envmap[6], glm::mat4 M[3])
{
/**
 * Computes Spherical Harmonics coefficients for standard unsigned byte 
 * environment map (cf. equation 10 from the paper).
 * 
 * Note : alternatively, retrieving the coefficients can be interesting
 *        (cf. equation 13).
 */

  float shCoeff[3][9];  
  memset( shCoeff[RED],   0, 9u*sizeof(float));
  memset( shCoeff[GREEN], 0, 9u*sizeof(float));
  memset( shCoeff[BLUE],  0, 9u*sizeof(float));

  /// Precompute generals attribs needed inside the loop
  const int texRes = envmap[0].width;
  const float texelSize = 1.0f / float(texRes);
  const float dColor = 1.0f / float( (sizeof(unsigned char) << 8) - 1 ); //    
  
  // Take care of the internal format (ugly!)
  const int nc = (envmap[0].internalFormat==GL_RGBA)?4:3; //


  float sumWeight = 0.0f;  
  for (int texId=0; texId<6; ++texId)
  {
    float u, v;
    unsigned char *pixels = envmap[texId].data;    
    
    for (int i=0; i<texRes; ++i)
    {
      // map value to [-1, 1]
      v = 2.0f * ((i+0.5f) * texelSize) - 1.0f;
      
      for (int j=0; j<texRes; ++j)
      {
        u = 2.0f * ((j+0.5f) * texelSize) - 1.0f;        
        
        glm::vec3 dir; float solidAngle;
        getTexelAttrib( texId, u, v, texelSize, &dir, &solidAngle);
        sumWeight += solidAngle;
        
        float lambda;
        
        lambda = (pixels[RED] * dColor) * solidAngle;  
        shCoeff[RED][0] += lambda * Y0( dir );
        shCoeff[RED][1] += lambda * Y1( dir );
        shCoeff[RED][2] += lambda * Y2( dir );
        shCoeff[RED][3] += lambda * Y3( dir );
        shCoeff[RED][4] += lambda * Y4( dir );
        shCoeff[RED][5] += lambda * Y5( dir );
        shCoeff[RED][6] += lambda * Y6( dir );
        shCoeff[RED][7] += lambda * Y7( dir );
        shCoeff[RED][8] += lambda * Y8( dir );
        
        lambda = (pixels[GREEN] * dColor) * solidAngle;
        shCoeff[GREEN][0] += lambda * Y0( dir );
        shCoeff[GREEN][1] += lambda * Y1( dir );
        shCoeff[GREEN][2] += lambda * Y2( dir );
        shCoeff[GREEN][3] += lambda * Y3( dir );
        shCoeff[GREEN][4] += lambda * Y4( dir );
        shCoeff[GREEN][5] += lambda * Y5( dir );
        shCoeff[GREEN][6] += lambda * Y6( dir );
        shCoeff[GREEN][7] += lambda * Y7( dir );
        shCoeff[GREEN][8] += lambda * Y8( dir );        
        
        lambda = (pixels[BLUE] * dColor) * solidAngle;
        shCoeff[BLUE][0] += lambda * Y0( dir );
        shCoeff[BLUE][1] += lambda * Y1( dir );
        shCoeff[BLUE][2] += lambda * Y2( dir );
        shCoeff[BLUE][3] += lambda * Y3( dir );
        shCoeff[BLUE][4] += lambda * Y4( dir );
        shCoeff[BLUE][5] += lambda * Y5( dir );
        shCoeff[BLUE][6] += lambda * Y6( dir );
        shCoeff[BLUE][7] += lambda * Y7( dir );
        shCoeff[BLUE][8] += lambda * Y8( dir );
        
        pixels += nc;
      }
    }
  }
  
  /**/
  const float dnorm = 2.0f * M_PI / sumWeight;
  for (int i=0; i<9; ++i)
  {
    shCoeff[RED][i]   *= dnorm;
    shCoeff[GREEN][i] *= dnorm;
    shCoeff[BLUE][i]  *= dnorm;
  }
  /**/
  
  #if IEM_TEST
  setIrradianceMatrices( test_coeffs, M);
  #else
  setIrradianceMatrices( shCoeff, M); 
  #endif
}

static
void getTexelAttrib( const int texId, const float u, const float v, const float texelSize,
                     glm::vec3 *direction, float *solidAngle)
{
  enum
  { 
    POSITIVE_X, NEGATIVE_X,
    POSITIVE_Y, NEGATIVE_Y,
    POSITIVE_Z, NEGATIVE_Z
  };
  
  
  /// Compute the texel direction  
  switch (texId)
  {
    case POSITIVE_X:
      *direction = glm::vec3( +1.0f, -v, -u);      
    break;
    
    case NEGATIVE_X:
      *direction = glm::vec3( -1.0f, -v, +u);
    break;
    
    case POSITIVE_Y:
      *direction = glm::vec3( +u, +1.0f, +v);
    break;
    
    case NEGATIVE_Y:
      *direction = glm::vec3( +u, -1.0f, -v);
    break;    
    
    case POSITIVE_Z:
      *direction = glm::vec3( +u, -v, +1.0f);
    break;
    
    case NEGATIVE_Z:
      *direction = glm::vec3( -u, -v, -1.0f);
    break;
  }
  
  *direction = glm::normalize( *direction );
  
  
  /// compute the solid angle  
  // [First try] I was looking for 'sin(theta)'
  //*solidAngle = sqrtf( 1.0f - direction->z * direction->z );
  // <=> sin( acos( z ) );
  
  /**
   * I'm really not sure for the solid angle computation 
   * http://seblagarde.wordpress.com/ helps me but I'm still
   * confuse.
   * */
  float x0 = u - texelSize;
  float y0 = v - texelSize;
  float x1 = u + texelSize;
  float y1 = v + texelSize;
  
  #define AREA(x, y)  atan2f(x * y, sqrtf(x * x + y * y + 1.0))
    
  *solidAngle = (AREA(x0,y0) + AREA(x1,y1)) - (AREA(x0,y1) + AREA(x1,y0));
  
  #undef AREA  
}


static
void setIrradianceMatrices( const float shCoeff[3][9], glm::mat4 M[3] )
{
/**
 * 
 * (cf. equation 12 from the paper)
 * 
 *  Note that this is a symmetric matrix, improvement can thus be done in terms
 *  of memory / computation. 
 */
 
  const float c1 = 0.429043f;
  const float c2 = 0.511664f;
  const float c3 = 0.743125f;
  const float c4 = 0.886227f;
  const float c5 = 0.247708f;
  
  
  for (int c=0; c<3; ++c)
  {
    M[c][0][0] = c1 * shCoeff[c][8];
    M[c][0][1] = c1 * shCoeff[c][4];
    M[c][0][2] = c1 * shCoeff[c][7];
    M[c][0][3] = c2 * shCoeff[c][3];
    
    M[c][1][0] = c1 * shCoeff[c][4];
    M[c][1][1] = - c1 * shCoeff[c][8];
    M[c][1][2] = c1 * shCoeff[c][5];
    M[c][1][3] = c2 * shCoeff[c][1];
    
    M[c][2][0] = c1 * shCoeff[c][7];
    M[c][2][1] = c1 * shCoeff[c][5];
    M[c][2][2] = c3 * shCoeff[c][6];
    M[c][2][3] = c2 * shCoeff[c][2];
    
    M[c][3][0] = c2 * shCoeff[c][3];
    M[c][3][1] = c2 * shCoeff[c][1];
    M[c][3][2] = c2 * shCoeff[c][2];
    M[c][3][3] = c4 * shCoeff[c][0] - c5 * shCoeff[c][6];
  }
}

#undef IEM_TEST

#undef Y0
#undef Y1
#undef Y2
#undef Y3
#undef Y4
#undef Y5
#undef Y6
#undef Y7
#undef Y8

#undef RED
#undef GREEN
#undef BLUE  


} //namespace IrradianceEnvMap
