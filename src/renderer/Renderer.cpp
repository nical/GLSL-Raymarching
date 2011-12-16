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
#include "nodes/ColorMix.hpp"
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
  }


  void Renderer::init(){
      
    CHECKERROR
    
    InitQuad();
    
    nodes::RegisterTimeNode();

    timeNode = nodes::CreateTimeNode();
    
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
        {"outputImage",     { Shader::OUTPUT  | Shader::TEXTURE2D} },
        {"fragmentInfo",    { Shader::OUTPUT  | Shader::TEXTURE2D} }
    };
    raymarchingShader = new Shader;
    CHECKERROR
    raymarchingShader->build( vs, fs, marcherLoc );

    nodes::RegisterRayMarchingNode(raymarchingShader);


    CHECKERROR
    vs.clear();
    fs.clear();

    utils::LoadTextFile("shaders/SecondPass.vert", vs);

    //  Depth Of Field Shader
    utils::LoadTextFile("shaders/DOF.frag", fs);
    Shader::LocationMap postFxLoc = {
        {"windowSize",      { Shader::UNIFORM | Shader::FLOAT2} },
        {"inputImage",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"fragmentInfo",  { Shader::UNIFORM | Shader::TEXTURE2D} }
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
        {"inputImage",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"fragmentInfo",  { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"edgeColor",      { Shader::UNIFORM | Shader::FLOAT3} },
        {"windowSize",     { Shader::UNIFORM | Shader::FLOAT2} }

    };
    auto edgeShader = new Shader;
    CHECKERROR
    edgeShader->build( vs, fs, edgeLoc  );
    nodes::RegisterPostFxNode( edgeShader  ,"Edge detection");

    //  Bloom Shader

    fs.clear();
    utils::LoadTextFile("shaders/Bloom.frag", fs);
    Shader::LocationMap bloomLoc = {
        {"inputImage",      { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"bloomCoefficient",{ Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",      { Shader::UNIFORM | Shader::FLOAT2} }
    };
    auto bloomShader = new Shader;
    CHECKERROR
    bloomShader->build( vs, fs, bloomLoc  );
    nodes::RegisterPostFxNode( bloomShader  ,"Bloom");

    //  Radial Blur Shader

    fs.clear();
    utils::LoadTextFile("shaders/RadialBlur.frag", fs);
    Shader::LocationMap radialLoc = {
        {"inputImage",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"windowSize",     { Shader::UNIFORM | Shader::FLOAT2} }
    };
    auto radialShader = new Shader;
    CHECKERROR
    radialShader->build( vs, fs, radialLoc  );
    nodes::RegisterPostFxNode( radialShader  ,"Radial Blur");
    

    //-----------------------------------------------------
    fs.clear();
    utils::LoadTextFile("shaders/Sepia.frag", fs );
    Shader::LocationMap sepiaMap = {
        {"inputImage",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"factor",         { Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",     { Shader::UNIFORM | Shader::FLOAT2} }

    };
    auto sepiaShader = new Shader;
    sepiaShader->build(vs,fs,sepiaMap);
    nodes::RegisterPostFxNode( sepiaShader  ,"Sepia");
    

    //-----------------------------------------------------
    fs.clear();
    utils::LoadTextFile("shaders/BlackAndWhite.frag", fs );
    Shader::LocationMap bnwMap = {
        {"inputImage",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"factor",         { Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",     { Shader::UNIFORM | Shader::FLOAT2} }

    };
    auto bnwShader = new Shader;
    bnwShader->build(vs,fs,bnwMap);
    nodes::RegisterPostFxNode( bnwShader  ,"Black and white");
    
    //-----------------------------------------------------
    fs.clear();
    utils::LoadTextFile("shaders/Corners.frag", fs );
    Shader::LocationMap cornerMap = {
        {"inputImage",     { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"cornerColor",          { Shader::UNIFORM | Shader::FLOAT3} },
        {"offset",         { Shader::UNIFORM | Shader::FLOAT} },
        {"factor",         { Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",     { Shader::UNIFORM | Shader::FLOAT2} }
    };
    auto cornerShader = new Shader;
    cornerShader->build(vs,fs,cornerMap);
    nodes::RegisterPostFxNode( cornerShader  ,"Corners");
    
    //-----------------------------------------------------
    fs.clear();
    utils::LoadTextFile("shaders/SetAlpha.frag", fs );
    Shader::LocationMap alphaMap = {
        {"inputImage",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"alpha",         { Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",     { Shader::UNIFORM | Shader::FLOAT2} }

    };
    auto alphaShader = new Shader;
    alphaShader->build(vs,fs,alphaMap);
    nodes::RegisterPostFxNode( alphaShader  ,"SetAlpha");
    auto alphaNode = nodes::CreatePostFxNode("SetAlpha");

    CHECKERROR

    nodes::RegisterFloatMathNodes();
    nodes::RegisterColorMixNode();


    io::Compositor::Instance().add( new io::NodeView(QPointF(-300,50), timeNode) );
    io::Compositor::Instance().add( new io::ColorNodeView(QPointF(-200, 100), nodes::CreateColorNode( glm::vec3(0.0,0.0,1.0) ) ) );
    io::Compositor::Instance().add( new io::ColorNodeView(QPointF(-200, 150), nodes::CreateColorNode( glm::vec3(0.8,0.8,0.8) ) ) );
    io::Compositor::Instance().add( new io::ColorNodeView(QPointF(-200, 200), nodes::CreateColorNode( glm::vec3(1.0,1.0,1.0) ) ) );
    io::Compositor::Instance().add( new io::ColorNodeView(QPointF(-200, 250), nodes::CreateColorNode( glm::vec3(1.0,0.0,0.0) ) ) );

    auto rayMarcher = nodes::CreateRayMarchingNode();
    screenNode = nodes::CreateScreenNode();
    io::Compositor::Instance().add( new io::NodeView(QPointF(0,0),     rayMarcher ) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(600,0),   screenNode ) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(200,0),   nodes::CreatePostFxNode("Depth of field") ) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(200,200), nodes::CreatePostFxNode("Edge detection") ) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(200,400), nodes::CreatePostFxNode("Radial Blur") ) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(200,500), nodes::CreatePostFxNode("Bloom")) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(200,300), nodes::CreatePostFxNode("Sepia") ) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(400,300), nodes::CreatePostFxNode("Black and white") ) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(400,400), nodes::CreatePostFxNode("Corners") ) );

    io::Compositor::Instance().add( new io::NodeView(QPointF(-100, 200), nodes::CreateColorMixNode()) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(-100, 300), nodes::CreateAddNode()) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(-100, 400), nodes::CreateSinNode()) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(-300, 400), nodes::CreateCosNode()) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(-100, 500), nodes::CreateMultiplyNode()) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(-100, 600), nodes::CreateDivideNode()) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(-100, 600), nodes::CreateClampNode()) );
    io::Compositor::Instance().add( new io::NodeView(QPointF(-300, 600), nodes::CreateSubstractNode() ) );

    io::Compositor::Instance().add( new io::SliderNodeView(QPointF(-500, 0), 1.0, 100.0 ) );
    io::Compositor::Instance().add( new io::SliderNodeView(QPointF(-500, 100), 0.0, 10.0 ) );
    io::Compositor::Instance().add( new io::SliderNodeView(QPointF(-500, 200), 0.0, 1.0 ) );
    io::Compositor::Instance().add( new io::SliderNodeView(QPointF(-500, 300), 0.0, 1.0 ) );

    assert( screenNode );
    assert( timeNode );
    assert( rayMarcher );
    assert( timeNode->output() >> rayMarcher->input(6) );
    assert( rayMarcher->output(1) >> screenNode->input() );
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
    std::cout << "Max color Attachments: " << maxBuffers << std::endl;

    _frameBuffer = (FrameBuffer*)1; // TODO: change that before the gos of programming see it.

  }

  void Renderer::freeBuffers()
  {

  }
}
