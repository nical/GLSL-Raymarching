#version 330

// Declare all the semantics
#define ATTR_POSITION	0
#define ATTR_COLOR		3
#define ATTR_TEXCOORD	4
#define VERT_POSITION	0
#define VERT_COLOR		3
#define VERT_TEXCOORD	4
#define VERT_INSTANCE	7
#define FRAG_COLOR		0
#define FRAG_RED		  0
#define FRAG_GREEN		1
#define FRAG_BLUE		  2
#define FRAG_ALPHA		3

out vec4 out_Color;

uniform sampler2D colourTexture;
uniform sampler2D normalsTexture;

uniform vec2 windowSize;

float edgeDetection (in vec2 coords){
  float dxtex = 1.0 / windowSize.x;
  float dytex = 1.0 / windowSize.y;

  float depth0 = texture2D(normalsTexture,coords).a;
  float depth1 = texture2D(normalsTexture,coords + vec2(dxtex,0.0)).a;
  float depth2 = texture2D(normalsTexture,coords + vec2(0.0,-dytex)).a;
  float depth3 = texture2D(normalsTexture,coords + vec2(-dxtex,0.0)).a;
  float depth4 = texture2D(normalsTexture,coords + vec2(0.0,dytex)).a;

  float ddx = abs((depth1 - depth0) - (depth0 - depth3));
  float ddy = abs((depth2 - depth0) - (depth0 - depth4));
  return clamp((ddx + ddy - 0.01)*100.0,0.0,1.0);
}

void main (void){

    if (gl_FragCoord.x < (windowSize.x * 0.5) ) {
      out_Color = texture2D(colourTexture, vec2(gl_FragCoord.x/windowSize.x, gl_FragCoord.y/windowSize.y));
    } else {
      out_Color = vec4(texture2D(normalsTexture, vec2(gl_FragCoord.x/windowSize.x, gl_FragCoord.y/windowSize.y)).rgb, 1.0);
    }
    //out_Color = texture2D(colourTexture, gl_TexCoord[0].st);
}
