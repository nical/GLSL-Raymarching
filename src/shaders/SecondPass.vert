#version 330

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 in_Position;

void main(void)
{
  gl_Position = projectionMatrix /* * viewMatrix * modelMatrix*/ * vec4(in_Position, 1.0);
  gl_TexCoord[0] = gl_MultiTexCoord0;
}
