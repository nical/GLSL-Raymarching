#include "renderer/Renderer.hpp"
#include "utils/LoadFile.hpp"
#include "renderer/Shader.hpp"
#include "utils/CheckGLError.hpp"
#include "renderer/FrameBuffer.hpp"
#include "renderer/DrawQuad.hpp"

#include "kiwi/core/all.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "nodes/TimeNode.hpp"
#include "nodes/ColorNode.hpp"
#include "nodes/PostFxNode.hpp"
#include "nodes/RayMarchingNode.hpp"
#include "nodes/FloatMathNodes.hpp"
#include "io/Compositor.hpp"
#include "io/NodeView.hpp"
#include "io/ColorNodeView.hpp"
#include "io/PortView.hpp"
#include "io/SliderNodeView.hpp"

#include <GL/glew.h>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <initializer_list>


using namespace std;

namespace renderer{


  Renderer::~Renderer(){
  }

  void Renderer::setWindowDimensions (unsigned int x, unsigned int y) 
  {
    window.x = x;
    window.y = y;
    *winSizeNode->output().dataAs<glm::vec2>() = glm::vec2((float)x, (float)y);
  }


  void Renderer::init(){
      
    CHECKERROR
    
    //createPlane();
    InitQuad();
    


    viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));

    auto mat4TypeInfo = kiwi::core::DataTypeManager::TypeOf("Mat4");
    auto uintTypeInfo = kiwi::core::DataTypeManager::TypeOf("GLuint");
    auto vec2TypeInfo = kiwi::core::DataTypeManager::TypeOf("Vec2");
    auto vec3TypeInfo = kiwi::core::DataTypeManager::TypeOf("Vec3");
    auto textureTypeInfo = kiwi::core::DataTypeManager::TypeOf("Texture2D");
    

    viewMatNode = kiwi::core::NodeTypeManager::Create("Mat4");
    winSizeNode = kiwi::core::NodeTypeManager::Create("Vec2");
    *viewMatNode->output().data()->value<glm::mat4>() = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));

    nodes::RegisterTimeNode();
    timeNode = nodes::CreateTimeNode();
    winSizeNode = kiwi::core::NodeTypeManager::Create("Vec2");
    *winSizeNode->output().dataAs<glm::vec2>() = glm::vec2((float)window.x, (float)window.y);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // shaders

    CHECKERROR
    string vs, fs;
    utils::LoadTextFile("shaders/Raymarching.vert", vs);
    utils::LoadTextFile("shaders/Raymarching.frag", fs);
    Shader::LocationMap marcherLoc = {
        {"viewMatrix",      { Shader::UNIFORM | Shader::MAT4F} },
        {"shadowColor",     { Shader::UNIFORM | Shader::FLOAT3} },
        {"skyColor",        { Shader::UNIFORM | Shader::FLOAT3} },
        {"groundColor",     { Shader::UNIFORM | Shader::FLOAT3} },
        {"buildingsColor",  { Shader::UNIFORM | Shader::FLOAT3} },
        {"redColor",        { Shader::UNIFORM | Shader::FLOAT3} },
        {"time",            { Shader::UNIFORM | Shader::FLOAT} },
        {"shadowHardness",  { Shader::UNIFORM | Shader::FLOAT} },
        {"fovyCoefficient", { Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",      { Shader::UNIFORM | Shader::FLOAT2} },
        {"colourTexture",   { Shader::OUTPUT  | Shader::TEXTURE2D} },
        {"normalsTexture",  { Shader::OUTPUT  | Shader::TEXTURE2D} },
        {"godRaysTexture",  { Shader::OUTPUT  | Shader::TEXTURE2D} }
    };
    raymarchingShader = new Shader;
    CHECKERROR
    raymarchingShader->build( vs, fs, marcherLoc );

    nodes::RegisterRayMarchingNode(raymarchingShader);
    rayMarchingNode = nodes::CreateRayMarchingNode();


    CHECKERROR
    vs.clear();
    fs.clear();

    utils::LoadTextFile("shaders/SecondPass.vert", vs);

    //  Depth Of Field Shader
    utils::LoadTextFile("shaders/DOF.frag", fs);
    Shader::LocationMap postFxLoc = {
        {"windowSize",      { Shader::UNIFORM | Shader::FLOAT2} },
        {"colourTexture",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"normalsTexture",  { Shader::UNIFORM | Shader::TEXTURE2D} }
    };
    postEffectShader = new Shader;
    CHECKERROR
    postEffectShader->build( vs, fs, postFxLoc );

    CHECKERROR

    nodes::RegisterPostFxNode( postEffectShader ,"Depth of field");
    nodes::RegisterScreenNode();

    //  Edge Detection Shader

    fs.clear();
    utils::LoadTextFile("shaders/EdgeDetection.frag", fs);
    Shader::LocationMap edgeLoc = {
        {"colourTexture",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"normalsTexture",  { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"edgeColour",      { Shader::UNIFORM | Shader::FLOAT3} },
        {"windowSize",     { Shader::UNIFORM | Shader::FLOAT2} }

    };
    auto edgeShader = new Shader;
    CHECKERROR
    edgeShader->build( vs, fs, edgeLoc  );
    nodes::RegisterPostFxNode( edgeShader  ,"Edge detection");
    auto edgeNode = nodes::CreatePostFxNode("Edge detection");

    //  Bloom Shader

    fs.clear();
    utils::LoadTextFile("shaders/Bloom.frag", fs);
    Shader::LocationMap bloomLoc = {
        {"colourTexture",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"normalsTexture",  { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"windowSize",     { Shader::UNIFORM | Shader::FLOAT2} }
    };
    auto bloomShader = new Shader;
    CHECKERROR
    bloomShader->build( vs, fs, bloomLoc  );
    nodes::RegisterPostFxNode( bloomShader  ,"Bloom");
    auto bloomNode = nodes::CreatePostFxNode("Bloom");

    //  Radial Blur Shader

    fs.clear();
    utils::LoadTextFile("shaders/RadialBlur.frag", fs);
    Shader::LocationMap radialLoc = {
        {"colourTexture",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"normalsTexture",  { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"windowSize",     { Shader::UNIFORM | Shader::FLOAT2} }
    };
    auto radialShader = new Shader;
    CHECKERROR
    radialShader->build( vs, fs, radialLoc  );
    nodes::RegisterPostFxNode( radialShader  ,"Radial Blur");
    auto radialNode = nodes::CreatePostFxNode("Radial Blur");


    //-----------------------------------------------------
    fs.clear();
    utils::LoadTextFile("shaders/Sepia.frag", fs );
    Shader::LocationMap sepiaMap = {
        {"colorTexture",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"factor",         { Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",     { Shader::UNIFORM | Shader::FLOAT2} }

    };
    auto sepiaShader = new Shader;
    sepiaShader->build(vs,fs,sepiaMap);
    nodes::RegisterPostFxNode( sepiaShader  ,"Sepia");
    auto sepiaNode = nodes::CreatePostFxNode("Sepia");

    //-----------------------------------------------------
    fs.clear();
    utils::LoadTextFile("shaders/BlackAndWhite.frag", fs );
    Shader::LocationMap bnwMap = {
        {"colorTexture",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"factor",         { Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",     { Shader::UNIFORM | Shader::FLOAT2} }

    };
    auto bnwShader = new Shader;
    bnwShader->build(vs,fs,bnwMap);
    nodes::RegisterPostFxNode( bnwShader  ,"Black and white");
    auto bnwNode = nodes::CreatePostFxNode("Black and white");


    CHECKERROR

    nodes::RegisterFloatMathNodes();

    skyColorNode = nodes::CreateColorNode( glm::vec3(0.0,0.0,1.0) );
    groundColorNode = nodes::CreateColorNode( glm::vec3(0.8,0.8,0.8) );
    buildingsColorNode = nodes::CreateColorNode( glm::vec3(1.0,1.0,1.0) );
    sphereColorNode = nodes::CreateColorNode( glm::vec3(1.0,0.0,0.0) );
    dofNode = nodes::CreatePostFxNode("Depth of field");
    screenNode = nodes::CreateScreenNode();



    io::Compositor::Instance().add( new io::NodeView(QPointF(-300,50), timeNode) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(400,0),screenNode) );
    io::Compositor::Instance().add( new io::ColorNodeView(QPointF(-300, 100), skyColorNode ) );
    io::Compositor::Instance().add( new io::ColorNodeView(QPointF(-300, 150), groundColorNode ) );
    io::Compositor::Instance().add( new io::ColorNodeView(QPointF(-300, 200), buildingsColorNode ) );
    io::Compositor::Instance().add( new io::ColorNodeView(QPointF(-300, 250), sphereColorNode ) );

    io::Compositor::Instance().add( new io::NodeView(QPointF(0,0), rayMarchingNode) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(200,0),dofNode) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(200,200),edgeNode) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(200,400),radialNode) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(200,500),bloomNode) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(200,300),sepiaNode) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(400,300),bnwNode) );

    io::Compositor::Instance().add( new io::NodeView(QPointF(-100, 300), nodes::CreateAddNode()) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(-100, 400), nodes::CreateSinNode()) );

    io::Compositor::Instance().add( new io::SliderNodeView(QPointF(-100, 450), 0.0, 100.0 ) );

    assert( timeNode );
    assert( timeNode->output() >> rayMarchingNode->input(6) );
    assert( rayMarchingNode->output(1) >> screenNode->input() );
    rayMarchingNode->output(1) >> dofNode->input(0);
    rayMarchingNode->output(2) >> dofNode->input(1);
    dofNode->output(1) >> screenNode->input();
  }


  static std::list<kiwi::core::Node*> s_processList;
  void OrderNodes( kiwi::core::Node * last )
  {
    for( auto it = last->previousNodes().begin(); it != last->previousNodes().end(); ++it)
    {
        OrderNodes( *it );
        if( find(s_processList.begin(), s_processList.end(), *it ) == s_processList.end() )
        {
            s_processList.push_back( *it );
        }
    }
  }

  void ProcessNodes( kiwi::core::Node * last )
  {
      s_processList.clear();
      OrderNodes(last);
      s_processList.push_back(last);
      for(auto it = s_processList.begin(); it != s_processList.end(); ++it )
      {
         // std::cerr << (*it)->type()->name() << endl;
          (*it)->update();
      }
      //std::cerr << "----------------\n";

  }

  void Renderer::drawScene()
  {

    CHECKERROR
    if( _frameBuffer == 0 ) return;

    ProcessNodes(screenNode);

  }



  void Renderer::createBuffers()
  {

    std::cout << "Window dimensions: " << window.x << "x" << window.y << std::endl;

    GLint maxBuffers;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxBuffers);
    std::cout << "Max Colour Attachments: " << maxBuffers << std::endl;

    _frameBuffer = (FrameBuffer*)1; // TODO: change that before the gos of programming see it.

  }

  void Renderer::freeBuffers()
  {

  }
}
