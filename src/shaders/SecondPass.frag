#version 330

out vec4 out_Color;

uniform sampler2D colourTexture;
uniform sampler2D normalsTexture;
uniform sampler2D godRaysTexture;

uniform vec2 windowSize;

const float sampleDist = 1.0;
const float sampleStrength = 2.2;

float edgeDetection(in vec2 uncoords){
  float dxtex = 1.0 / windowSize.x;
  float dytex = 1.0 / windowSize.y;
 
  vec2 coords = (uncoords / windowSize);

  float depth0 = texture2D(normalsTexture,coords).a;
  float depth1 = texture2D(normalsTexture,coords + vec2(dxtex,0.0)).a;
  float depth2 = texture2D(normalsTexture,coords + vec2(0.0,-dytex)).a;
  float depth3 = texture2D(normalsTexture,coords + vec2(-dxtex,0.0)).a;
  float depth4 = texture2D(normalsTexture,coords + vec2(0.0,dytex)).a;
  
  float ddx = abs((depth1 - depth0) - (depth0 - depth3));
  float ddy = abs((depth2 - depth0) - (depth0 - depth4));
  return clamp((ddx + ddy - 0.005)*100.0,0.0,1.0)/(depth0 * 5);
}

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

    if (gl_FragCoord.x < (windowSize.x * 0.5) ) {
      out_Color = texture2D(colourTexture, vec2(gl_FragCoord.x/windowSize.x, gl_FragCoord.y/windowSize.y));
      out_Color = mix(out_Color, vec4(0.5, 0.0, 0.0, 1.0), edgeDetection(gl_FragCoord.xy));
      //out_Color = radialBlur(gl_FragCoord.xy/windowSize);
    } else {
      //out_Color = vec4(texture2D(normalsTexture, vec2(gl_FragCoord.x/windowSize.x, gl_FragCoord.y/windowSize.y)).rgb, 1.0);
      out_Color = vec4(texture2D(normalsTexture, vec2(gl_FragCoord.x/windowSize.x, gl_FragCoord.y/windowSize.y)).bbb, 1.0);
    }
    //out_Color = texture2D(colourTexture, gl_TexCoord[0].st);
}
