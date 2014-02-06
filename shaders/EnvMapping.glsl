/*
 *          EnvMapping.glsl
 *
 *
 *  The irradiance algorithm is based on 
 *     
 *     "An Efficient Representation for Irradiance Environment Maps"
 *                               by Ravi Ramamoorthi & Pat Hanrahan
 *
 */


//------------------------------------------------------------------------------


-- Vertex

// IN
layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec3 inNormal;

// OUT
out vec3 vNormalWS;
out vec3 vViewDirWS;
out vec3 vIrradiance;

// UNIFORM
uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;
uniform vec3 uEyePosWS;
uniform mat4 uIrradianceMatrix[3];  // RGB irradiance coefficient matrices
uniform mat3 uInvSkyboxRotation;    // tp question


vec3 computeIrradiance( vec3 normal, mat4 M[3])
{
  vec4 n = vec4( normal, 1.0f);
  
  vec3 output;  
  output.r = dot( n, M[0] * n);
  output.g = dot( n, M[1] * n);
  output.b = dot( n, M[2] * n);
    
  return output;
}

void main()
{
  // Clip Space position
  gl_Position = uModelViewProjMatrix * inPosition;

  // World Space normal
  vNormalWS = normalize( uNormalMatrix * inNormal );
  
  // World Space view direction from world space position
  vec3 posWS = vec3(uModelMatrix * inPosition);
  vViewDirWS = normalize(posWS - uEyePosWS);
  
  // Irradiance color for RGB components
  vec3 normalWS = uInvSkyboxRotation * vNormalWS;
  vIrradiance = computeIrradiance( normalWS, uIrradianceMatrix);
}


--

//------------------------------------------------------------------------------


-- Fragment

// IN
in vec3 vNormalWS;
in vec3 vViewDirWS;
in vec3 vIrradiance;

// OUT
layout(location = 0) out vec4 fColor;

// UNIFORM
uniform samplerCube uEnvmap;


void main()
{  
  // Normal color for debugging
  //fColor.rgb = 0.5f*normal + 0.5f;
 
  // Compute the WS reflection vector from normalized EyeDir & Normal
  vec3 r = reflect( vViewDirWS, vNormalWS);
  
  vec3 reflectColor = texture( uEnvmap, r).rgb; 
    
  /*
  vec3 refractColor; // some refraction with chromatic aberration
  refractColor.r = texture( uEnvmap, refract( vViewDirWS, vNormalWS, 0.21f)).r;
  refractColor.g = texture( uEnvmap, refract( vViewDirWS, vNormalWS, 0.20f)).g; 
  refractColor.b = texture( uEnvmap, refract( vViewDirWS, vNormalWS, 0.22f)).b; 
  */

  vec3 envColor = reflectColor;


  /// Final composition
  
  //fColor.rgb = envColor;
  //fColor.rgb = mix( envColor, vIrradiance, 0.5f );
  
  fColor.rgb = vIrradiance;

  // change to get transparency
  fColor.a = 1.0f;
}

