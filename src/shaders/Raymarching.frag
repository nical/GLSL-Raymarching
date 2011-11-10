#version 330
#define MAX_STEPS 100

in vec3 pass_Color;

out vec4 out_Color;

float PlaneDistance(vec3 point, vec3 normal, float distance)
{
	return dot(point - (normal * distance), normal);
}

float SphereDistance(vec3 point, vec3 center, float radius)
{
	vec3 d = point - center;
	return sqrt(d.x * d.x + d.y * d.y + d.z * d.z) - radius;
}

float CubeDistance(vec3 point, vec3 center, vec3 size) {
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


vec3 rayCast(vec3 position, vec3 direction){
  int i;
  float nextDistance;

  for (i = 0; (i < MAX_STEPS) ; i++){
    //nextDistance = min(planeDistance (position, vec4(0.0, 1.0, 0.0, 0.0)), planeDistance(position, vec4(1.0, 0.0, 0.0, 25.0)));
    nextDistance = min(SphereDistance(position, vec3(10.0, 10.0, 50.0), 3.0), CubeDistance(position, vec3(10.0, -15.0, 100.0), vec3(3.0)));
    nextDistance = min(nextDistance, TorusDistance(position, vec3(0.0, -20.0, 60.0), 1.0, 5.0));
    position += nextDistance * direction;
  }

  return position;
}

void main(void)
{
  float windowRatio = 8.0/6.0;

  vec2 normalizedPosition;

  vec3 colour;
  vec3 skyColour = vec3(0.4, 0.6, 0.9);
  //vec3 skyColour = vec3(1.0, 0.0, 0.0);
  vec3 groundColour = vec3(0.42, 0.88, 0.11);

  normalizedPosition.x = (gl_FragCoord.x * windowRatio / 800.0) - 0.5;
  normalizedPosition.y = ((gl_FragCoord.y)/ 600.0) - 0.5;

  vec3 direction = normalize(vec3(normalizedPosition, 1.0));
  vec3 landingPixel = rayCast (vec3(0.0, 0.0, 0.0), direction) * 0.1;
  float fogFactor = 1.0 - (1.0 / exp(landingPixel.z * 0.06));

  colour = mix (groundColour, skyColour, fogFactor);

  out_Color = vec4(colour, 1.0);

  /*if (landingPixel.z <= 350.0) {
    discard;
  }*/
}
