#version 330
#define MAX_STEPS 200

in vec3 pass_Color;

out vec4 out_Color;

uniform float fuffaTime;

float PlaneDistance(in vec3 point, in vec3 normal, in float pDistance)
{
	return dot(point - (normal * pDistance), normal);
}

float SphereDistance(vec3 point, vec3 center, float radius, out vec3 normal)
{
  point.z = mod(point.z+radius, 30.0)-radius;
  point.x = mod(point.x+radius, 30.0)-radius;
  //point.y = mod(point.y, 30.0);
  vec3 d = point - center;
  normal = normalize(d);
  return length(d) - radius;
}

float CubeDistance(in vec3 point, in vec3 center, in vec3 size) {
  point.z = mod (point.z+size.z, 20.0)+size.z;
  point.x = mod (point.x+size.x, 20.0)+size.x;
  vec3 d = point - center;
	return max(max(abs(d.x) - size.x, abs(d.y) - size.y), abs(d.z) - size.z);
}

float CylinderDistance(vec3 point, vec3 center, float radius, float height) {
	vec3 d = point - center;
	return max(sqrt(d.x * d.x + d.z * d.z) - radius, abs(d.y) - height);
}

float TorusDistance(vec3 point, vec3 center, float minorRadius, float majorRadius)
{
	vec3 d = point - center;
	float x = sqrt(d.x * d.x + d.z * d.z) - majorRadius;
	return sqrt(x * x + d.y * d.y) - minorRadius;
}






vec3 applyFog( in vec3 rgb, in float distance ){
    float fogAmount = exp( -distance* 0.006 );
    vec3  fogColor  = vec3(0.5,0.6,0.7);
    return mix( fogColor, rgb, fogAmount );
}




vec3 rayCast(in vec3 position, in vec3 direction, out vec3 hitColor){
  int i = 0;
  float nextDistance;
  float lastCubeDistance;
  float lastPlaneDistance;
  vec3 origin = position;
  vec3 sphereNormal = vec3(0.0,1.0,0.0);
  for (i = 0; i < MAX_STEPS ; ++i){
    //nextDistance = min(planeDistance (position, vec4(0.0, 1.0, 0.0, 0.0)), planeDistance(position, vec4(1.0, 0.0, 0.0, 25.0)));
    //nextDistance = min(SphereDistance(position, vec3(10.0, 10.0, 50.0), 3.0), CubeDistance(position, vec3(10.0, -15.0, 100.0), vec3(3.0)));
    //nextDistance = min(nextDistance, TorusDistance(position, vec3(0.0, -20.0, 60.0), 1.0, 5.0));
    //nextDistance = SphereDistance(position, vec3(10.0, 10.0, 50.0), 3.0);
    //nextDistance = min(PlaneDistance(position, vec3(0.0, 1.0, 0.0), -10.0), CubeDistance(position, vec3(0.0, 1.0, 0.0), vec3(1.0, 2.0, 1.0)));

    //lastCubeDistance = CubeDistance(position, vec3(5.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
    lastCubeDistance = SphereDistance(position, vec3(0.0, 0.0, 5.0), 3.0, sphereNormal);
    lastPlaneDistance = PlaneDistance(position, vec3(0.0, 1.0, 0.0), 0.0);
    nextDistance = min(lastCubeDistance, lastPlaneDistance);
    if (nextDistance < 0.2) {
      if (lastCubeDistance < lastPlaneDistance) {
        //hitColor = vec3(1.0, 1.0, 1.0) * (1.0+dot(sphereNormal, vec3(0.0,1.0,0.0))*0.5);
        hitColor = vec3(0.5,0.5,0.5)+sphereNormal*0.5;
      } else {
        hitColor = vec3(0.42, 0.88, 0.11);
      }
      break;
    }
    hitColor = vec3(0.4, 0.6, 0.9);
    position += nextDistance * direction;
  }

  float fogFactor = 1.0 - (1.0 / (dot(position-origin,position-origin)*0.00001) );
  hitColor = applyFog(hitColor, fogFactor );
  //hitColor = mix (hitColor, vec3(0.5,0.6,0.7), fogFactor);

  return position;
}

float darkenCorners(in vec2 screenPos)
{
    return  1.1-clamp(1.5*dot(screenPos,screenPos)+0.045, 0.1, 1.0);
}

void main(void)
{
out_Color=vec4(1.0,1.0,0.0, 1.0);

  vec2 resolution = vec2(640.0, 480.0);

  float windowRatio = resolution.x / resolution.y;

  vec3 colour;
  vec3 skyColour = vec3(0.4, 0.6, 0.9);
  vec3 hitColour = vec3(0.42, 0.88, 0.11);

  vec2 screenPos;
  screenPos.x = (gl_FragCoord.x/resolution.x - 0.5)*windowRatio;
  screenPos.y =  gl_FragCoord.y/resolution.y - 0.5;

  vec3 direction = normalize(vec3(screenPos.x,screenPos.y, 1.0));
  vec3 position = vec3(5*sin(fuffaTime*0.01), 6.0, fuffaTime);
  
  vec3 landingPixel = rayCast (position, direction, hitColour) * 0.1;
  
  out_Color = vec4(hitColour, 1.0);

//  if( (abs(screenPos.x) < 0.45) && (abs(screenPos.y) < 0.45) ) out_Color *= 0.;

  out_Color *= darkenCorners(screenPos);

  /*if (landingPixel.z <= 350.0) {
    discard;
  }*/
}
