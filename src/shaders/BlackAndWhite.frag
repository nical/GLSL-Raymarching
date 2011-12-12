#version 330

out vec4 out_Color;

uniform sampler2D colorTexture;
uniform float factor;

uniform vec2 windowSize;


vec2 texelCoord = vec2(gl_FragCoord.xy / windowSize);

float Luminance( in vec4 color )
{
    //return (color.r + color.g + color.b ) / 3.0;
    
    return (color.r * 0.2125 + color.g *0.7154 + color.b * 0.0721);
}

void main (void){
  out_Color = texture2D(colorTexture, texelCoord);
  out_Color = mix(
    out_Color, vec4(vec3(Luminance(out_Color)), 1.0)
    , clamp(factor,0.0,1.0) );
}

