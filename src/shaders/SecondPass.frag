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

void main (void){

    if (gl_FragCoord.x <= 320.0) {
      out_Color = texture2D(colourTexture, vec2(gl_FragCoord.x/windowSize.x, gl_FragCoord.y/windowSize.y));
    } else {
      out_Color = texture2D(normalsTexture, vec2(gl_FragCoord.x/windowSize.x, gl_FragCoord.y/windowSize.y));
    }
    //out_Color = texture2D(colourTexture, gl_TexCoord[0].st);
}
