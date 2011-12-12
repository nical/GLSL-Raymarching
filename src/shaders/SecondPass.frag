#version 330
#define PI    3.14159265

out vec4 out_Color;

uniform sampler2D colourTexture;
uniform sampler2D normalsTexture;
uniform sampler2D godRaysTexture;

uniform vec2 windowSize;

const float sampleDist = 1.0;
const float sampleStrength = 2.2;

//  Computing it once so we don't have to do it every time
vec2 texelCoord = vec2(gl_FragCoord.xy / windowSize);

//  DOF effect variables here. We'll have to translate them into uniforms, but that will come when it's working. :3

float focalDepth = 100.0;  //focal point. comes from external script, but you may use autofocus option below

float depthSamples = 3; //samples on the first ring
float depthRings = 5; //ring count

bool useAutoFocus = false; //use autofocus in shader?
float focalRange = 10000.0; //focal range
float maxBlur =  1.5;//clamp value of max blur

float highlightThreshold = 0.9; //highlight threshold;
float highlightGain = 1.0; //highlight gain;

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
	vec3 newColour = vec3(0.0);
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

	newColour.r = texture2D(colourTexture,coords + vec2(0.0,1.0) * blurringCoord).r;
	newColour.g = texture2D(colourTexture,coords + vec2(-0.866,-0.5) * blurringCoord).g;
	newColour.b = texture2D(colourTexture,coords + vec2(0.866,-0.5) * blurringCoord).b;

	vec3 lumcoeff = vec3(0.299,0.587,0.114);
	float lum = dot(newColour.rgb, lumcoeff);
	float thresh = max((lum - highlightThreshold) * highlightGain, 0.0);
	return newColour + mix(vec3(0.0), newColour, thresh * blur);
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
		float tmpDepth = texture2D(normalsTexture, clamp(coords + offset[i], 0.0, 1.0)).a;
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

	blur = clamp((abs(zDistance - focalDepth)/focalRange)*100.0, -maxBlur, maxBlur);
	//blur = clamp((abs(depth - focalDepth)/range)*(maxblur/float(rings)),-maxblur,maxblur);

	if (useAutoFocus)
	{
		//float fDepth = clamp(texture2D(normalsTexture, vec2(0.5,0.5)).a, 0.0, 1.0);
		float fDepth = texture2D(normalsTexture, vec2(0.5,0.5)).a;
		//float fDepth = 0.5;
		blur = clamp((abs(zDistance - fDepth)/focalRange)*100.0, -maxBlur, maxBlur);
	}

	vec2 noise = noiseGeneration(coords) * noiseAmount * blur;

	float w = (1.0/windowSize.x) * blur + noise.x;
	float h = (1.0/windowSize.y) * blur + noise.y;


	vec3 colour = texture2D(colourTexture, coords).rgb;
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
			colour += colorProcessing(coords + vec2(pw * w, ph * h), blur) * mix(1.0, i/depthRings, bokehBias);
			s += 1.0 * mix(1.0, i/depthRings, bokehBias);
		}
	}

	colour /= s;

	return colour;
}

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
  return clamp(clamp((ddx + ddy - 0.5) * 0.5,0.0,1.0)/(depth0 * 0.02), -1.0, 1.0);
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

vec4 bloomEffect(in vec2 coords) {
   
  vec4 bloom = vec4(0.0);
  int j;
  int i;

  for( i = -4 ; i < 4; i++) {
    for (j = -3; j < 3; j++) {
      bloom += texture2D(colourTexture, coords + vec2((j * 1.0 / windowSize.x), (i * 1.0 / windowSize.y))) * 0.25;
    }
  }
    
  if (texture2D(colourTexture, coords).r < 0.3) {
    bloom = bloom * bloom * 0.012;
  } else if (texture2D(colourTexture, coords).r < 0.5) {
    bloom = bloom * bloom * 0.009;
  } else {
    bloom = bloom * bloom *0.0075;
  }
  
  return bloom + texture2D(colourTexture, coords);
}

void main (void){

    float zDistance = texture2D(normalsTexture, texelCoord).a;

    if (gl_FragCoord.x < (windowSize.x * 0.5) ) {
      //out_Color = texture2D(colourTexture, texelCoord);
      //out_Color = mix(out_Color, vec4(0.5, 0.0, 0.0, 1.0), edgeDetection(gl_FragCoord.xy));
      //out_Color = radialBlur(gl_FragCoord.xy/windowSize);
        //out_Color = vec4(DOF(zDistance, texelCoord), 1.0);
        out_Color = bloomEffect(texelCoord);
    } else {
      //out_Color = vec4(texture2D(normalsTexture, vec2(gl_FragCoord.x/windowSize.x, gl_FragCoord.y/windowSize.y)).rgb, 1.0);
      //out_Color = vec4(texture2D(normalsTexture, texelCoord).aaa, 1.0);
      out_Color = texture2D(colourTexture, texelCoord);
      out_Color = mix(out_Color, vec4(0.5, 0.0, 0.0, clamp(mix(1.0, 0.0, zDistance/300.0), 0.0, 1.0)), edgeDetection(gl_FragCoord.xy));
      //out_Color = vec4(texture2D(godRaysTexture, vec2(gl_FragCoord.x/windowSize.x, gl_FragCoord.y/windowSize.y)).aaa, 1.0);
    }
    //out_Color = texture2D(colourTexture, gl_TexCoord[0].st);
}
