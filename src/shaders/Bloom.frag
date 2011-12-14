#version 330

out vec4 out_Color;

uniform sampler2D inputImage;
uniform sampler2D fragmentInfo;
uniform float bloomCoefficient;

uniform vec2 windowSize;

//  Computing it once so we don't have to do it every time
vec2 texelCoord = vec2(gl_FragCoord.xy / windowSize);

vec4 bloomEffect(in vec2 coords) {
   
  vec4 bloom = vec4(0.0);
  int j;
  int i;

  for( i = -4 ; i < 4; i++) {
    for (j = -3; j < 3; j++) {
      bloom += texture2D(inputImage, coords + vec2((j * 1.0 / windowSize.x), (i * 1.0 / windowSize.y))) * (bloomCoefficient * 0.01);
    }
  }
    
  if (texture2D(inputImage, coords).r < 0.3) {
    bloom = bloom * bloom * 0.012;
  } else if (texture2D(inputImage, coords).r < 0.5) {
    bloom = bloom * bloom * 0.009;
  } else {
    bloom = bloom * bloom *0.0075;
  }
  
  return bloom + texture2D(inputImage, coords);
}

void main (void){
  out_Color = bloomEffect(texelCoord);
}
