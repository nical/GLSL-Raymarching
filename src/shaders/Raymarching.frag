#version 330
#define MAX_STEPS 200

out vec4 out_Colour[3];

uniform float fuffaTime;
uniform vec2 windowSize;
uniform mat4 viewMatrix;
uniform vec3 shadowColor;
uniform vec3 buildingsColor;
uniform vec3 groundColor;
uniform vec3 redColor;
uniform vec3 skyColor;
uniform float fovyCoefficient;
uniform float shadowHardness;

#define epsilon 0.01
#define PI 3.14159265

#define NO_HIT 0
#define HAS_HIT 1
// materials
#define SKY_MTL 0
#define GROUND_MTL 1
#define BUILDINGS_MTL 2
#define RED_MTL 3

vec3 debugColor;

float PlaneDistance(in vec3 point, in vec3 normal, in float pDistance)
{
return dot(point - (normal * pDistance), normal);
}

void MeshTwist (inout vec3 point){
  float cosine = cos(0.002 * point.y);
  float sine = sin(0.002 * point.y);
  mat2  tempMatrix = mat2( cosine, -sine, sine, cosine);
  point = vec3(tempMatrix * point.xz, point.y);
}

float SphereDistance(vec3 point, vec3 center, float radius)
{
  point.z = mod(point.z+15, 230.0)-15;
  point.x = mod(point.x+15, 230.0)-15;
  //point.y = mod(point.y, 30.0);
  return length(point - center) - radius;
}


float CubeDistance2 (in vec3 point, in vec3 size) {

  return length(max(abs(point)-size, 0.0));
}

vec3 DistanceRepetition(in vec3 point, in vec3 repetition ) {
  vec3 q = mod(point, repetition)-0.5*repetition;
  return q;
}

float Rand2D(float x, float y)
{
    return 0.5 + (1.0/8.0) * (
        + sin( floor(x)     )
        + cos( 3.0*floor(x) )
        + sin( 3.0*floor(y) )
        + cos( 4.0*floor(y) )
    );
}

float RandomBuildingDistance(in vec3 point, in vec3 repetition, in float maxHeight )
{
    vec3 q = mod(point, repetition)-0.5*repetition;
    q.y = point.y;

    float height = Rand2D(point.x/repetition.x,point.z/repetition.z);

    height *= maxHeight;
    //debugColor = vec3(0.0,sin(height),0.0);
    return CubeDistance2 ( q,
        vec3 ( 1.0+2*Rand2D(point.x/repetition.x+100,point.z/repetition.z+100)
        , height
        , 1.0 + 2*Rand2D(point.x/repetition.x+50,point.z/repetition.z+400)));
}
float CubeRepetition(in vec3 point, in vec3 repetition ) {
    vec3 q = mod(point, repetition)-0.5*repetition;
    q.y = point.y;
    return CubeDistance2 ( q, vec3 (2.0, 10.0, 2.0));
}

float rand(vec2 co){
    return (fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453) + 1.0) * 0.5;
}

float CubeDistance(in vec3 point, in vec3 center, in vec3 size) {
  //point.z = mod (point.z+10, 20.0)-10;
  //point.x = mod (point.x+10, 20.0)-10;
  vec3 d = point - center;
return max(max(abs(d.x) - size.x, abs(d.y) - size.y), abs(d.z) - size.z);
}


float JazzBuilding01 (in vec3 point){
    vec3 baseSize = vec3(3.0, 6.0, 3.0);
    vec3 centre = vec3(0.0, 0.0, 0.0);
    vec3 sizeSecondaryD = baseSize + vec3(0.0, 0.0, 1.0);
    vec3 posSecondaryD = centre + vec3(0.0, 0.0, -0.5);
    vec3 sizeSecondaryW = baseSize + vec3(1.0, 0.0, 0.0);
    vec3 posSecondaryW = centre + vec3(-0.5, 0.0, 0.0);
    vec3 sizeSecondaryH = baseSize + vec3(-1.0, 1.0, -1.0);
    vec3 posSecondaryH = centre + vec3(+0.5, +0.5, +0.5);
    point.x = mod(point.x, 20);
    point.z = mod(point.z, 20);
    return max(max(max(CubeDistance(point, centre, baseSize), CubeDistance(point, posSecondaryD, sizeSecondaryD)),
                   CubeDistance(point, posSecondaryW, sizeSecondaryW)),
               CubeDistance(point, posSecondaryH, sizeSecondaryH));
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

float lightSphere (in vec3 position, in float radius, in vec3 centrePos){
  vec3 modPosition = centrePos - position;
  return length(modPosition) - radius;
}

void applyFog( in float distance, inout vec3 rgb ){

    //float fogAmount = (1.0 - clamp(distance*0.005,0.0,1.0) );
    float fogAmount = exp( -distance* 0.001 );
    vec3 fogColor = vec3(0.9,0.95,1);
    rgb = mix( skyColor, rgb, fogAmount );
}

float RedDistance(in vec3 position)
{
    return SphereDistance(position, vec3(0.0, 3.0, 5.0), 5.0);
}

float BuildingsDistance(in vec3 position)
{
  vec3 theOtherPosition = position + vec3(350.0,-2.0,0.0);
  //MeshTwist(position);
    return min(
      CubeRepetition(position, vec3(17.0, 0.0, 20.0))
      , CubeRepetition(theOtherPosition, vec3(23.0, 0.0, 23.0))
    );
}

float GroundDistance(in vec3 position)
{
    return PlaneDistance(position, vec3(0.0,1.0,0.0), 0.0);
}

float DistanceField(in vec3 position, out int mtl )
{
    float redDistance = RedDistance(position);
    float bldDistance = BuildingsDistance(position);
    float gndDistance = GroundDistance(position);
    float closest = gndDistance;
    mtl = GROUND_MTL;
    if ( bldDistance < closest )
    {
        closest = bldDistance;
        mtl = BUILDINGS_MTL;
    }
    if ( redDistance < closest )
    {
        closest = redDistance;
        mtl = RED_MTL;
    }
    return closest;
}


float Softshadow( in vec3 landPoint, in vec3 lightVector, float mint, float maxt, float iterations )
{
    float penumbraFactor = 1.0;
    vec3 sphereNormal;
    for( float t = (mint + rand(gl_FragCoord.xy) * 0.01); t < maxt; )
    {
        float nextDist = min(
            BuildingsDistance(landPoint + lightVector * t )
            , RedDistance(landPoint + lightVector * t )
        );

        if( nextDist < 0.001 ){
            return 0.0;
        }
        //float penuAttenuation = mix (1.0, 0.0, t/maxt);
        penumbraFactor = min( penumbraFactor, iterations * nextDist / t );
        t += nextDist;
    }
    return penumbraFactor;
}


vec3 RayMarch(in vec3 position, in vec3 direction, out int mtl)
{
    float nextDistance = 1.0;
    for (int i = 0; i < MAX_STEPS ; ++i)
    {
        nextDistance = DistanceField(position,mtl);

        if ( nextDistance < 0.001)
        {
            return position;
        }
        position += direction * nextDistance;
    }
    // out of steps
    if (direction.y < 0.0 )
    {
        mtl = GROUND_MTL;
    }
    else
    {
        mtl = SKY_MTL;
    }
    return position;
}

vec3 MaterialColor( int mtl )
{
    switch(mtl)
    {
        case SKY_MTL : return skyColor;
        case BUILDINGS_MTL : return buildingsColor;
        case GROUND_MTL : return groundColor;
        case RED_MTL : return redColor;
    }
    return vec3(1.0,0.0,1.0); // means error
}

vec3 ComputeNormal(vec3 pos, int material)
{
    int dummy;
    return normalize(
        vec3(
          DistanceField( vec3(pos.x + epsilon, pos.y, pos.z), dummy ) - DistanceField( vec3(pos.x - epsilon, pos.y, pos.z), dummy )
        , DistanceField( vec3(pos.x, pos.y + epsilon, pos.z), dummy ) - DistanceField( vec3(pos.x, pos.y - epsilon, pos.z), dummy )
        , DistanceField( vec3(pos.x, pos.y, pos.z + epsilon), dummy ) - DistanceField( vec3(pos.x, pos.y, pos.z - epsilon), dummy )
        )
    );
}

void PinHoleCamera( vec2 screenPos, float ratio, float fovy, mat4 transform, out vec3 position, out vec3 direction )
{
    screenPos.x *= ratio;
    direction = normalize(vec3(screenPos.x,screenPos.y, fovy));
    position = vec3(5*sin(fuffaTime*0.01), 25.0, fuffaTime);
}

void FishEyeCamera( vec2 screenPos, float ratio, float fovy, mat4 transform, out vec3 position, out vec3 direction )
{
    screenPos.y -= 0.2;
    screenPos *= vec2(PI*0.5,PI*0.5/ratio)/fovy;

    direction = vec3(
           sin(screenPos.y+PI*0.5)*sin(screenPos.x)
        , -cos(screenPos.y+PI*0.5)
        ,  sin(screenPos.y+PI*0.5)*cos(screenPos.x)
    );
    position = vec3(5*sin(fuffaTime*0.01), 25.0, fuffaTime);
}

float AmbientOcclusion (vec3 point, vec3 normal, float stepDistance, float samples) {
	float occlusion;
	int tempMaterial;
	for (occlusion = 1.0 ; samples > 0.0 ; samples--) {
		occlusion -= (samples * stepDistance - (DistanceField( point + normal * samples * stepDistance, tempMaterial))) / pow(2.0, samples);
	}
	return occlusion;
}

void main(void)
{
    debugColor = vec3(0.0,0.0,0.0);
    float ratio = windowSize.x / windowSize.y;
    // position on the screen
    vec2 screenPos;
    screenPos.x = (gl_FragCoord.x/windowSize.x - 0.5);
    screenPos.y = gl_FragCoord.y/windowSize.y - 0.5;

    vec3 direction;
    vec3 position;
    FishEyeCamera(screenPos, ratio, fovyCoefficient, viewMatrix, position, direction);
    int material;
    vec3 hitPosition = RayMarch(position, direction, material);

    vec3 hitColor;
    if( material != SKY_MTL ) // has hit something
    {
        vec3 lightpos = vec3(50.0 * sin(fuffaTime*0.01), 10 + 40.0 * abs(cos(fuffaTime*0.01)), (fuffaTime) + 100.0 );
        vec3 lightVector = normalize(lightpos - hitPosition);
        // soft shadows
        float shadow = Softshadow(hitPosition, lightVector, 0.1, 50.0, shadowHardness);
        // attenuation due to facing (or not) the light
        vec3 normal = ComputeNormal(hitPosition, material);
        float attenuation = clamp(dot(normal, lightVector),0.0,1.0)*0.6 + 0.4;
        shadow = min(shadow, attenuation);
        //material color
        vec3 mtlColor = MaterialColor(material);

        if(material == BUILDINGS_MTL){
          mtlColor = mix(shadowColor, mtlColor, clamp(hitPosition.y/7.0, 0.0, 1.0));
        }
        hitColor = mix(shadowColor, mtlColor, 0.4+shadow*0.6) - debugColor;
        vec3 hitNormal = ComputeNormal(hitPosition, 0);
        float AO = AmbientOcclusion(hitPosition, hitNormal, 0.35, 5.0);
        hitColor = mix(shadowColor, hitColor, AO);

        //applyFog( length(position-hitPosition), hitColor);
        out_Colour[0] = vec4(hitColor, 1.0);
        out_Colour[1] = vec4(vec3(shadow), 1.0);        // todo apply fog
        out_Colour[2].a = 0.0;


        hitPosition.z -= position.z;

        out_Colour[1].b = clamp(hitPosition.y/7.0, 0.0, 1.0);
        out_Colour[1].a = hitPosition.z;
    }
    else // sky
    {
        vec3 hitColor = skyColor;
        out_Colour[0] = vec4(hitColor, 1.0);
        out_Colour[1] = vec4(1.0);
        out_Colour[1].a = 1000000.0;
        //out_Colour[2] = vec4(hitColor, 1.0);
    }

}
