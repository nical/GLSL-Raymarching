#version 330
#define MAX_STEPS 70

//in vec3 pass_Color;

out vec4 out_Color;

uniform float fuffaTime;
uniform vec2 windowSize;

layout(location = 0) out vec4 outputColour;
layout(location = 1) out vec4 outputNormals;

#define epsilon 0.01

float PlaneDistance(in vec3 point, in vec3 normal, in float pDistance)
{
	return dot(point - (normal * pDistance), normal);
}

float SphereDistance(vec3 point, vec3 center, float radius)
{
  point.z = mod(point.z+15, 230.0)-15;
  point.x = mod(point.x+15, 230.0)-15;
  //point.y = mod(point.y, 30.0);
  return length(point - center) - radius;
}

float SphereDistanceNormal(vec3 point, vec3 center, float radius, out vec3 normal)
{
  point.z = mod(point.z+15, 230.0)-15;
  point.x = mod(point.x+15, 230.0)-15;
  //point.y = mod(point.y, 30.0);
  vec3 d = point - center;
  normal = normalize(d);
  return length(d) - radius;
}

float Building1Distance(in vec3 point)
{
    vec3 size = vec3(2.0, 3.0, 2.0);
    float tempz = point.z;
    float tempx = point.x;
    point.z = mod (point.z+10, 17.0)-10;
    point.x = mod (point.x+10, 27.0)-10;
    size.y =  2+abs(sin(floor(tempx/27) + sin( 1.3*floor(tempz/17))) + sin(0.5*floor(tempz/17)) ) * 3;
    point -= vec3(5.0, 3.0, 3.0); // center
    return max(max(abs(point.x) - size.x, abs(point.y) - size.y), abs(point.z) - size.z);
}
/*
vec3 Building1Normal(in vec3 point)
{
    point.z = mod (point.z+10, 17.0)-10;
    point.x = mod (point.x+10, 27.0)-10;
    point -= vec3(5.0, 3.0, 3.0);//center
    point /= vec3(2.0, 3.0, 2.0);
    if( abs(point.x) > abs(point.z) )
    {
        if(abs(point.y) > abs(point.x))
            return vec3(0.0,1.0,0.0);
        else
            return faceforward( vec3(-1.0,0.0,0.0), point, vec3(1.0,0.0,0.0) );
    }
    else
    {
        if(abs(point.y) > abs(point.z))
            return vec3(0.0,1.0,0.0);
        else
            return faceforward( vec3(0.0,0.0,-1.0), point, vec3(0.0,0.0,1.0) );
    }
    return vec3(1.0,1.0,1.0);
}
*/
float CubeDistance(in vec3 point, in vec3 center, in vec3 size) {
  point.z = mod (point.z+10, 20.0)-10;
  point.x = mod (point.x+10, 20.0)-10;
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






void applyFog( in float distance, inout vec3 rgb ){

    float fogAmount =  (1.0 - clamp(distance*0.00004,0.0,1.0) );
    //float fogAmount = exp( -distance* 0.006 );
    vec3  fogColor  = vec3(0.9,0.95,1);
    rgb = mix( fogColor, rgb, fogAmount );
}

vec3 AmbiantOcclusion2( in vec3 position, in vec3 direction )
{
    vec3 newPos = position + direction * 0.5;
    float sphereDist        = 2*clamp( SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0), 0, 0.5 );
    float planeDist         = 2*clamp( PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0) , 0, 0.5 );
    float building1Dist     = 2*clamp( Building1Distance(newPos)                       , 0, 0.5 );
    newPos += direction*2;
    float sphereDist2       = 0.5*clamp( SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0), 0, 2.5 );
    float planeDist2        = 0.5*clamp( PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0) , 0, 2.5 );
    float building1Dist2    = 0.5*clamp( Building1Distance(newPos)                       , 0, 2.5 );
    newPos += direction*2;
    float sphereDist3       = 0.2*clamp( SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0), 0, 4.5 );
    float planeDist3        = 0.2*clamp( PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0) , 0, 4.5 );
    float building1Dist3    = 0.2*clamp( Building1Distance(newPos)                       , 0, 4.5 );
    newPos += direction*3;
    float sphereDist4       = 0.15*clamp( SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0), 0, 7.5 );
    float planeDist4        = 0.15*clamp( PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0) , 0, 7.5 );
    float building1Dist4    = 0.15*clamp( Building1Distance(newPos)                       , 0, 7.5 );

    float sphereFactor = (sphereDist + 0.5 * sphereDist2 + 0.25 * sphereDist3 + 0.10 * sphereDist4);

    return
        (
          vec3(0.9,0.7,0.5) * (planeDist + 0.5 * planeDist2 + 0.25 * planeDist3 + 0.10 * planeDist4)
        + vec3(0.9,0.7,0.5) * (building1Dist + 0.5 * building1Dist2 + 0.25 * building1Dist3 + 0.10 * building1Dist4)
        - vec3(-0.3,1.0,1.0) * (1-sphereFactor)
        ) * 0.2;
}

float AmbiantOcclusion( in vec3 position, in vec3 direction )
{
    vec3 newPos = position + direction * 0.5;
    float sphereDist = SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0);
    float planeDist  = PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0);
    float building1Dist1 = Building1Distance(newPos);
    newPos += direction*2;
    float sphereDist2 = SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0);
    float planeDist2  = PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0);
    float building1Dist2 = Building1Distance(newPos);
    newPos += direction*2;
    float sphereDist3 = SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0);
    float planeDist3  = PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0);
    float building1Dist3 = Building1Distance(newPos);
    newPos += direction*3;
    float sphereDist4 = SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0);
    float planeDist4  = PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0);
    float building1Dist4 = Building1Distance(newPos);

    float occlusion =(
        1-min(min( sphereDist, planeDist ), building1Dist1) * 0.5
        + (3- min(building1Dist2,min( sphereDist2, planeDist2 )))*0.25
        + (5- min(building1Dist3,min( sphereDist3, planeDist3 )))*0.15
        + (8- min(building1Dist3,min( sphereDist3, planeDist3 )))*0.10
    )/3.0;

    return clamp(1.0-occlusion,0.0,1.0)*0.6+0.4 ;
}


vec3 rayCast(in vec3 position, in vec3 direction, inout vec3 hitColor){
  int i = 0;
  float nextDistance;
  float lastCubeDistance;
  float lastSphereDistance;
  float lastPlaneDistance;
  vec3 origin = position;
  vec3 sphereNormal = vec3(0.0,1.0,0.0);
  bool hit = false;
  for (i = 0; i < MAX_STEPS ; ++i){
    //lastCubeDistance = CubeDistance(position, vec3(5.0, 2.0, 0.0), vec3(4.0, 4.0, 4.0));
    lastCubeDistance = Building1Distance(position);
    lastSphereDistance = SphereDistanceNormal(position, vec3(0.0, 3.0, 5.0), 5.0, sphereNormal);
    lastPlaneDistance = PlaneDistance(position, vec3(0.0, 1.0, 0.0), 0.0);

    int selected = 0;

    nextDistance = lastPlaneDistance;

    if( lastSphereDistance < lastPlaneDistance )
    {
        selected = 2;
        nextDistance = lastSphereDistance;
    }
    if( lastCubeDistance < nextDistance )
    {
        selected = 1;
        nextDistance = lastCubeDistance;
    }

    if (nextDistance < 0.1) {
      hit = true;
      if (selected == 2) { // spheres
        hitColor = vec3(1.0, 0.0, 0.05) * (1.0+dot(sphereNormal, vec3(0.4,0.4,0.0))) * AmbiantOcclusion(position, sphereNormal*1.1 );
        //hitColor.b = 0.9;
        applyFog( dot(position-origin,position-origin), hitColor );
      } else if (selected == 0) { // plane
        //hitColor = vec3(0.6, 0.7, 0.9) * AmbiantOcclusion(position, vec3(0.0,1.0,0.0) );
        //hitColor = mix(vec3(0.2, 0.4, 0.8), vec3(0.9, 0.9, 1.0), AmbiantOcclusion(position, vec3(0.0,1.0,0.0)) );
        hitColor = 0.5*(vec3(0.2, 0.4, 0.8) + AmbiantOcclusion2(position, vec3(0.0,1.0,0.0)));
        hitColor = AmbiantOcclusion2(position, vec3(0.0,1.0,0.0));
        applyFog( dot(position-origin,position-origin), hitColor );
      } else if (selected == 1) { // cubes
        vec3 buildingNormal
            = normalize( vec3( Building1Distance( position + vec3(epsilon,0,0) ) - Building1Distance( position - vec3(epsilon,0,0) )
                , Building1Distance( position + vec3(0,epsilon,0) ) - Building1Distance( position - vec3(0,epsilon,0) )
                , Building1Distance( position + vec3(0,0,epsilon) ) - Building1Distance( position - vec3(0,0,epsilon) )
            ) );
        hitColor = (/*vec3(1.0, 1.0, 1.0) +*/ AmbiantOcclusion2(position, buildingNormal )) * (1.0+dot(buildingNormal, vec3(0.4,0.4,0.0)));
        hitColor.b = 1.0;
        hitColor = buildingNormal*0.5 + vec3(0.5);
        applyFog( dot(position-origin,position-origin), hitColor );
        //hitColor = vec3(0.5) + 0.5*Building1Normal(position);
      }
      break;
    }
    position += nextDistance * direction;
  }
  // quick hack to catch the fragments that went out of steps but should be on the plane
  if(!hit && direction.y < 0 ){
    hitColor = 0.5*(vec3(0.2, 0.4, 0.8) + AmbiantOcclusion2(position, vec3(0.0,1.0,0.0)));
    hitColor = AmbiantOcclusion2(position, vec3(0.0,1.0,0.0));
    applyFog( dot(position-origin,position-origin), hitColor );
  }

  return position;
}

float darkenCorners(in vec2 screenPos)
{
    return  1.2-clamp(1.5*dot(screenPos,screenPos)+0.045, 0.2, 1.0);
}

void main(void)
{
  float windowRatio = windowSize.x / windowSize.y;

  vec3 color;
  vec3 skyColor = vec3(0.9, 0.95, 1);
  vec3 hitColor = skyColor;

  vec2 screenPos;
  screenPos.x = (gl_FragCoord.x/windowSize.x - 0.5)*windowRatio;
  screenPos.y =  gl_FragCoord.y/windowSize.y - 0.5;

  vec3 direction = normalize(vec3(screenPos.x,screenPos.y, 1.0));
  vec3 position = vec3(5*sin(fuffaTime*0.01), 12.0, fuffaTime);

  vec3 landingPixel = rayCast (position, direction, hitColor) * 0.1;

  //scanlines
  if( mod(gl_FragCoord.y * 0.5, 2.0) < 0.5 )
    hitColor *= 0.95;

  outputColour = vec4(hitColor, 1.0);

  out_Color = vec4(hitColor, 1.0) * darkenCorners(screenPos);
  gl_FragData[0] = vec4(1.0, 1.0, 0.0, 1.0);
  gl_FragData[1] = vec4(1.0, 1.0, 0.0, 1.0);
}
