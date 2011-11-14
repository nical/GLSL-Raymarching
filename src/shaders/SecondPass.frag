#version 330
#define MAX_STEPS 70

out vec4 out_Color;

uniform sampler2D colourTexture;
uniform sampler2D normalTexture;

void main (void){
  if (gl_FragCoord.x <= 400 && gl_FragCoord.y <= 300 ) {
    gl_FragColor = texture2D(colourTexture, gl_TexCoord[0].st);
  } else {
    gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0);
  }
    //out_Color = texture2D(colourTexture, gl_TexCoord[0].st);
}
