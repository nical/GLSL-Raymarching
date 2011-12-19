#version 330
#define PI    3.14159265

out vec4 out_color;

uniform sampler2D inputImage;
uniform sampler2D fragmentInfo;

uniform vec2 windowSize;

//  Computing it once so we don't have to do it every time
vec2 texelCoord = vec2(gl_FragCoord.xy / windowSize);

//  DOF effect variables here. We'll have to translate them into uniforms, but that will come when it's working. :3

uniform float focalDepth;// = 150.0;  //focal point. comes from external script, but you may use autofocus option below

float depthSamples = 3; //samples on the first ring
float depthRings = 5; //ring count

bool useAutoFocus = false; //use autofocus in shader?
uniform float focalRange; //= 100.0; //focal range
float maxBlur =  1.5;//clamp value of max blur

float highlightThreshold = 0.7; //highlight threshold;
uniform float highlightGain; // = 0.5; //highlight gain;

float bokehBias = 0.8; //bokeh edge bias
float bokehFringe = 0.7; //bokeh chromatic aberration/fringing

bool useNoise = true; //use noise instead of pattern for sample dithering
float noiseAmount = 0.00001; //noise amount

bool useDepthBlur = false; //blur the depth buffer?
float depthBlurSize = 1.0; //depthblursize

//  Function used to generate either noise or patterns for dithering
vec2 noiseGeneration (in vec2 coord){
	float noiseX = ((fract(1.0-coord.s*(windowSize.x/2.0))*0.25)+(fract(coord.t*(windowSize.y/2.0))*0.75))*2.0-1.0;
	float noiseY = ((fract(1.0-coord.s*(windowSize.x/2.0))*0.75)+(fract(coord.t*(windowSize.y/2.0))*0.25))*2.0-1.0;

	if (useNoise)
	{
	    noiseX = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233))) * 43758.5453),0.0,1.0)*2.0-1.0;
	    noiseY = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233)*2.0)) * 43758.5453),0.0,1.0)*2.0-1.0;
	}
	return vec2(noiseX,noiseY);
}

//  Processing the texel to get highlights and blur
vec3 colorProcessing (vec2 coords, float blur) {
	vec3 newcolor = vec3(0.0);
	vec2 blurringCoord = texelCoord * bokehFringe * blur * 0.001;

  /*if(abs((coords + vec2(0.0,1.0) * blurringCoord).y) > 1.0)/*abs((coords + vec2(0.0,1.0) * blurringCoord).y) > 1.0){
    return (vec3(1.0, 0.0, 0.0));
  }*/

  /*if(abs((coords + vec2(-0.866,-0.5) * blurringCoord).x) > 1.0 || abs((coords + vec2(-0.866,-0.5) * blurringCoord).y) > 1.0){
    return (vec3(0.0, 1.0, 0.0));
  }*/

  /*if(abs((coords + vec2(0.866,-0.5) * blurringCoord).x) > 1.0 || abs((coords + vec2(0.866,-0.5) * blurringCoord).y) > 1.0){
    return (vec3(0.0, 0.0, 1.0));
  }*/

	newcolor.r = texture2D(inputImage,coords + vec2(0.0,1.0) * blurringCoord).r;
	newcolor.g = texture2D(inputImage,coords + vec2(-0.866,-0.5) * blurringCoord).g;
	newcolor.b = texture2D(inputImage,coords + vec2(0.866,-0.5) * blurringCoord).b;

	vec3 lumcoeff = vec3(0.299,0.587,0.114);
	float lum = dot(newcolor.rgb, lumcoeff);
	float thresh = max((lum - highlightThreshold) * highlightGain, 0.0);
	return newcolor + mix(vec3(0.0), newcolor, thresh * blur);
}

//  Depth-Based blurring
float depthBlurring(vec2 coords)
{
	float depth = 0.0;
	float kernel[9];
	vec2 offset[9];

	vec2 wh = texelCoord * depthBlurSize;

	offset[0] = vec2(-wh.x, -wh.y);
	offset[1] = vec2( 0.0, -wh.y);
	offset[2] = vec2( wh.x, -wh.y);

	offset[3] = vec2( -wh.x, 0.0);
	offset[4] = vec2( 0.0, 0.0);
	offset[5] = vec2( wh.x,  0.0);

	offset[6] = vec2( -wh.x, wh.y);
	offset[7] = vec2( 0.0, wh.y);
	offset[8] = vec2( wh.x, wh.y);

	kernel[0] = 1.0/16.0; 	kernel[1] = 2.0/16.0;	kernel[2] = 1.0/16.0;
	kernel[3] = 2.0/16.0;	kernel[4] = 4.0/16.0;	kernel[5] = 2.0/16.0;
	kernel[6] = 1.0/16.0;   kernel[7] = 2.0/16.0;	kernel[8] = 1.0/16.0;


	for( int i = 0; i < 9; i++ )
	{
		float tmpDepth = texture2D(fragmentInfo, clamp(coords + offset[i], 0.0, 1.0)).a;
		depth += tmpDepth * kernel[i];
	}

	return depth;
}

vec3 DOF (float zDistance, vec2 coords){
	float blur = 0.0;

	if (useDepthBlur)
	{
		zDistance = depthBlurring(coords);
	}

	blur = clamp((abs(zDistance - focalDepth)/focalRange), -maxBlur, maxBlur);
	//blur = clamp((abs(depth - focalDepth)/range)*(maxblur/float(rings)),-maxblur,maxblur);

	if (useAutoFocus)
	{
		//float fDepth = clamp(texture2D(fragmentInfo, vec2(0.5,0.5)).a, 0.0, 1.0);
		float fDepth = texture2D(fragmentInfo, vec2(0.5,0.5)).a;
		//float fDepth = 0.5;
		blur = clamp((abs(zDistance - fDepth)/focalRange)*100.0, -maxBlur, maxBlur);
	}

	vec2 noise = noiseGeneration(coords) * noiseAmount * blur;

	float w = (1.0/windowSize.x) * blur + noise.x;
	float h = (1.0/windowSize.y) * blur + noise.y;


	vec3 color = texture2D(inputImage, coords).rgb;
	float s = 1.0;

	float ringSamples;

	for (float i = 1.0; i <= depthRings; i ++)
	{
		ringSamples = i * depthSamples;

		for (float j = 0.0 ; j < ringSamples ; j ++)
		{
			float blurStep = PI*2.0 / ringSamples;
			float pw = (cos(j * blurStep) * i);
			float ph = (sin(j * blurStep) * i);
			color += colorProcessing(coords + vec2(pw * w, ph * h), blur) * mix(1.0, i/depthRings, bokehBias);
			s += 1.0 * mix(1.0, i/depthRings, bokehBias);
		}
	}

	color /= s;

	return color;
}

void main (void){
    float zDistance = texture2D(fragmentInfo, texelCoord).a;
    out_color = vec4(DOF(zDistance, texelCoord), 1.0);
}
