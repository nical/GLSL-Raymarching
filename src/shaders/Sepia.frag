#version 330

out vec4 out_Color;

uniform sampler2D colorTexture;
uniform float factor;

uniform vec2 windowSize;


vec2 texelCoord = vec2(gl_FragCoord.xy / windowSize);

float Luminance( in vec4 color )
{
    return (color.r + color.g + color.b ) / 3.0;
}

vec4 Sepia( in vec4 color )
{
    return vec4(
          clamp(color.r * 0.393 + color.g * 0.769 + color.b * 0.189, 0.0, 1.0)
        , clamp(color.r * 0.349 + color.g * 0.686 + color.b * 0.168, 0.0, 1.0)
        , clamp(color.r * 0.272 + color.g * 0.534 + color.b * 0.131, 0.0, 1.0)
        , color.a
    );
}

void main (void){
  out_Color = texture2D(colorTexture, texelCoord);
  out_Color = mix(out_Color, Sepia(out_Color), clamp(factor,0.0,1.0) );
}

