#version 330

out vec4 out_Color;

uniform sampler2D inputImage;
uniform sampler2D normalsTexture;

uniform vec2 windowSize;

//  Computing it once so we don't have to do it every time
vec2 texelCoord = vec2(gl_FragCoord.xy / windowSize);

float edgeDetection(in vec2 coords){
  float dxtex = 1.0 / windowSize.x;     
  float dytex = 1.0 / windowSize.y;

  float depth0 = texture2D(normalsTexture,coords).a;
  float depth1 = texture2D(normalsTexture,coords + vec2(dxtex,0.0)).a;
  float depth2 = texture2D(normalsTexture,coords + vec2(0.0,-dytex)).a;
  float depth3 = texture2D(normalsTexture,coords + vec2(-dxtex,0.0)).a;
  float depth4 = texture2D(normalsTexture,coords + vec2(0.0,dytex)).a;

  float ddx = abs((depth1 - depth0) - (depth0 - depth3));
  float ddy = abs((depth2 - depth0) - (depth0 - depth4));
  return clamp(clamp((ddx + ddy - 0.5) * 0.5,0.0,1.0)/(depth0 * 0.02), -1.0, 1.0);
}

void main (void){
  out_Color = texture2D(inputImage, texelCoord);
  out_Color = mix(out_Color, vec4(0.5, 0.0, 0.0, 1.0), edgeDetection(texelCoord));
}
