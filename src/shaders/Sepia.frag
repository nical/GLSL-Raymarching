#version 330

out vec4 out_Color;

uniform sampler2D colourTexture;
uniform float factor;

uniform vec2 windowSize;

vec2 texelCoord = vec2(gl_FragCoord.xy / windowSize);

float Luminance( in vec4 color )
{
    return (color.r + color.g + color.b ) / 3.0;
}

vec4 Sepia( in vec4 color )
{
    float l = Luminance( color );
    return vec4( vec3(0.8,0.3,0.2)*l, color.a );
}

void main (void){
  out_Color = texture2D(colourTexture, texelCoord);
  out_Color = mix(out_Color, Sepia(out_Color), clamp(factor,0.0,1.0) );
}

