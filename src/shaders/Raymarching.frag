#version 330
#define MAX_STEPS 100

//in vec3 pass_Color;

out vec4 out_Colour[2];

uniform float fuffaTime;
uniform vec2 windowSize;

#define shadowColor vec3(0.0,0.3,0.7) // todo pass it as uniform
#define buildingsColor vec3(1.0,1.0,1.0) // todo pass it as uniform
#define groundColor vec3(1.0,1.0,1.0) // todo pass it as uniform
#define redColor vec3(1.0,0.1,0.1) // todo pass it as uniform
#define skyColor vec3(0.9,1.0,1.0) // todo pass it as uniform
#define viewMatrix mat4(0.0) // todo pass it as uniform
#define fovyCoefficient 1.0 // todo pass it as uniform
#define shadowHardness 7.0 // todo pass it as uniform

#define epsilon 0.01
#define PI 3.14159265

#define NO_HIT 0
#define HAS_HIT 1
// materials
#define SKY_MTL 0
#define GROUND_MTL 1
#define BUILDINGS_MTL 2
#define RED_MTL 3


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


float CubeDistance2 (in vec3 point, in vec3 size) {

  return length(max(abs(point)-size, 0.0));
}

vec3 DistanceRepetition(in vec3 point, in vec3 repetition ) {
  vec3 q = mod(point, repetition)-0.5*repetition;
  return q;
}



float Building1Distance(in vec3 point)
{
    vec3 size = vec3(2.0, 3.0, 2.0);
    float tempz = point.z;
    float tempx = point.x;
    point.z = mod (point.z+10, 17.0)-10;
    point.x = mod (point.x+10, 27.0)-10;
    //point = DistanceRepetition(point, vec3(17.0, 0.0, 27.0));
    size.y = 2.0 + abs(cos(floor(tempx/27) + sin( 1.3*ceil(tempz/17))) + sin(0.5*floor(tempz/17)) ) * 3;
    //size.y = 2.0 + abs(cos());
    point -= vec3(5.0, 3.0, 3.0); // center
    return max(max(abs(point.x) - size.x, abs(point.y) - size.y), abs(point.z) - size.z);
    //return CubeDistance2(point, size);
}

float CubeRepetition(in vec3 point, in vec3 repetition ) {
    vec3 q = mod(point, repetition)-0.5*repetition;
    q.y = point.y;
    return CubeDistance2 ( q, vec3 (2.0, 4.0, 2.0));
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






void applyFog( in float distance, inout vec3 rgb ){

    float fogAmount = (1.0 - clamp(distance*0.0005,0.0,1.0) );
    //float fogAmount = exp( -distance* 0.006 );
    vec3 fogColor = vec3(0.9,0.95,1);
    rgb = mix( skyColor, rgb, fogAmount );
}

vec3 AmbiantOcclusion2( in vec3 position, in vec3 direction )
{
    vec3 newPos = position + direction * 0.5;
    float sphereDist = 2*clamp( SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0), 0, 0.5 );
    float planeDist = 2*clamp( PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0) , 0, 0.5 );
    float building1Dist = 2*clamp( Building1Distance(newPos) , 0, 0.5 );
    newPos += direction*2;
    float sphereDist2 = 0.5*clamp( SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0), 0, 2.5 );
    float planeDist2 = 0.5*clamp( PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0) , 0, 2.5 );
    float building1Dist2 = 0.5*clamp( Building1Distance(newPos) , 0, 2.5 );
    newPos += direction*2;
    float sphereDist3 = 0.2*clamp( SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0), 0, 4.5 );
    float planeDist3 = 0.2*clamp( PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0) , 0, 4.5 );
    float building1Dist3 = 0.2*clamp( Building1Distance(newPos) , 0, 4.5 );
    newPos += direction*3;
    float sphereDist4 = 0.15*clamp( SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0), 0, 7.5 );
    float planeDist4 = 0.15*clamp( PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0) , 0, 7.5 );
    float building1Dist4 = 0.15*clamp( Building1Distance(newPos) , 0, 7.5 );

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
    float planeDist = PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0);
    float building1Dist1 = Building1Distance(newPos);
    newPos += direction*2;
    float sphereDist2 = SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0);
    float planeDist2 = PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0);
    float building1Dist2 = Building1Distance(newPos);
    newPos += direction*2;
    float sphereDist3 = SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0);
    float planeDist3 = PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0);
    float building1Dist3 = Building1Distance(newPos);
    newPos += direction*3;
    float sphereDist4 = SphereDistance(newPos, vec3(0.0, 3.0, 5.0), 5.0);
    float planeDist4 = PlaneDistance(newPos, vec3(0.0, 1.0, 0.0), 0.0);
    float building1Dist4 = Building1Distance(newPos);

    float occlusion =(
        1-min(min( sphereDist, planeDist ), building1Dist1) * 0.5
        + (3- min(building1Dist2,min( sphereDist2, planeDist2 )))*0.25
        + (5- min(building1Dist3,min( sphereDist3, planeDist3 )))*0.15
        + (8- min(building1Dist3,min( sphereDist3, planeDist3 )))*0.10
    )/3.0;

    return clamp(1.0-occlusion,0.0,1.0)*0.6+0.4 ;
}

float RedDistance(in vec3 position)
{
    return SphereDistance(position, vec3(0.0, 3.0, 5.0), 5.0);
}

float BuildingsDistance(in vec3 position)
{
    return min(
        CubeRepetition(position, vec3(20.0, 0.0, 20.0))
        , Building1Distance(position)
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

void main(void)
{
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
        hitColor = mix(shadowColor, mtlColor, 0.4+shadow*0.6);
        
        applyFog( length(position-hitPosition), hitColor);
        out_Colour[0] = vec4(hitColor, 1.0);
        out_Colour[1] = vec4(vec3(shadow), 1.0);        // todo apply fog
        hitPosition.z -= position.z;

        out_Colour[1].a = hitPosition.z/300.0;
    }
    else // sky
    {
        vec3 hitColor = skyColor;
        out_Colour[0] = vec4(hitColor, 1.0);
        out_Colour[1] = vec4(1.0);
    }

}
