#version 330
#define MAX_STEPS 70

out vec4 out_Color;

uniform sampler2D colourTexture;
uniform sampler2D normalTexture;

void main (void){
  gl_FragColor = texture2D(colourTexture, gl_TexCoord[0].st);
}
