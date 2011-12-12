#version 330
out vec4 out_Color;

uniform sampler2D colourTexture;
uniform vec2 windowSize;
uniform float alpha;

vec2 texelCoord = vec2(gl_FragCoord.xy / windowSize);

void main (void)
{
    out_Color = vec4(texture2D(colourTexture, texelCoord ).rgb, alpha);
}
