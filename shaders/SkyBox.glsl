/*
 *          SkyBox.glsl
 *
 */

//------------------------------------------------------------------------------


-- Vertex

// IN
layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec3 inNormal;

// OUT
out vec3 vTexCoord;

// UNIFORM
uniform mat4 uModelViewProjMatrix;


void main()
{
  gl_Position = uModelViewProjMatrix * inPosition;

  vTexCoord = inPosition.xyz;
}


--

//------------------------------------------------------------------------------


-- Fragment

// IN
in vec3 vTexCoord;

// OUT
layout(location = 0) out vec4 fragColor;

// UNIFORM
uniform samplerCube uCubemap;

void main()
{  
  fragColor = texture( uCubemap, vTexCoord);
}

