#version 330

out vec4 out_Color;

uniform sampler2D colourTexture;
uniform sampler2D normalsTexture;

uniform vec2 windowSize;

uniform float sampleDist;
uniform float sampleStrength;

//  Computing it once so we don't have to do it every time
vec2 texelCoord = vec2(gl_FragCoord.xy / windowSize);

vec4 radialBlur(in vec2 coords) {
  // Sample positions to do it faster
  float samples[10] = float[](-0.08,-0.05,-0.03,-0.02,-0.01,0.01,0.02,0.03,0.05,0.08);

  vec2 dir = 0.5 - coords;
  float distance = length(dir);

  dir = normalize(dir);

  vec4 colour =  texture2D(colourTexture, coords);
  vec4 blurredColour = colour;

  for (int i = 0; i < 10; i++) {
    blurredColour += texture2D( colourTexture, coords + dir * samples[i] * sampleDist );
  }

  // we have taken eleven samples
  blurredColour *= 1.0/11.0;

  // weighten the blur effect with the distance to the
  // center of the screen ( further out is blurred more)
  float t = distance * sampleStrength;
  t = clamp( t ,0.0,1.0);

  //Blend the original color with the averaged pixels
  return mix( colour, blurredColour, t );
}

void main (void){
  out_Color = radialBlur(texelCoord);
}
