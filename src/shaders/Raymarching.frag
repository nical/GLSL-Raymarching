#version 330
#define MAX_STEPS 100

in vec3 pass_Color;

out vec4 out_Color;

float planeDistance (vec3 position, vec4 planeEquation){
  return dot (planeEquation, vec4(position, 1.0));
}

float cubeDistance (vec3 position, vec3 centre, float edge){
  vec3 dist = centre - position;
  dist -= edge/2.0;
  return (max(max(dist.x, dist.y), dist.z));
}

vec3 rayCast(vec3 position, vec3 direction){
  int i;
  float nextDistance;

  for (i = 0; i < MAX_STEPS; i++){
    nextDistance = min(planeDistance (position, vec4(0.0, 1.0, 0.0, 0.0)), planeDistance(position, vec4(1.0, 0.0, 0.0, 25.0)));
    position += nextDistance * direction;
  }

  return position;
}

void main(void)
{
  float windowRatio = 8/6;

  vec3 colour;
  vec3 skyColour = vec3(0.4, 0.6, 0.9);
  vec3 groundColour = vec3(0.42, 0.88, 0.11);

  vec3 direction = normalize(vec3((gl_FragCoord.x * windowRatio / 800) - 0.5, ((600 - gl_FragCoord.y)/ 600) - 0.5, 1.0));
  vec3 landingPixel = rayCast (vec3(0.5, 0.5, 0.0), direction) * 0.1;
  float fogFactor = 1.0 - (1.0 / exp(landingPixel.z * 0.06));

  colour = mix (skyColour, groundColour, fogFactor);

  out_Color = vec4(colour, 1.0);
}
