/*
 *          PassThrough.glsl
 *
 */

//------------------------------------------------------------------------------


-- Vertex

// IN
layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// OUT
out vec3 vNormal;
out vec3 vColor;

// UNIFORM
uniform mat4 uModelViewProjMatrix;


void main()
{
  gl_Position = uModelViewProjMatrix * vec4( inPosition.xyz, 1.0f);

  vNormal = normalize(inNormal);
}


--

//------------------------------------------------------------------------------


-- Fragment

// IN
in vec3 vColor;
in vec3 vNormal;

// OUT
layout(location = 0) out vec4 outColor;

// UNIFORM
uniform vec4 uColor = vec4( 1.0f );


void main()
{
  outColor.rgb = vColor;
}

