#version 330

out vec4 out_Color;

uniform sampler2D colourTexture;
uniform sampler2D normalsTexture;

uniform vec2 windowSize;

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
  return clamp(clamp((ddx + ddy - 0.5)*0.01,0.0,1.0)/(depth0 * 0.01), -1.0, 1.0);
}

void main (void){
  out_Color = texture2D(colourTexture, vec2(gl_FragCoord.x/windowSize.x, gl_FragCoord.y/windowSize.y));
  out_Color = mix(out_Color, vec4(0.5, 0.0, 0.0, 1.0), edgeDetection(gl_FragCoord.xy));
}
