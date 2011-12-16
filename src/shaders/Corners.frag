#version 330

out vec4 out_Color;

uniform sampler2D inputImage;

uniform vec2 windowSize;

uniform vec3 cornerColor;

//uniform float offset;
//uniform float factor;

#define offset 0.6
#define factor 3.0


void main(void)
{
    vec2 texelCoord = vec2(gl_FragCoord.xy  / windowSize);
    vec2 screenSpace = texelCoord - vec2(0.5,0.5);
    float dist = clamp( dot(screenSpace,screenSpace) * factor - offset, 0.0,1.0);
    //out_Color = vec4(vec3(dist),1.0);
    out_Color = mix( texture2D(inputImage, texelCoord), vec4(cornerColor,1.0), dist);
}
