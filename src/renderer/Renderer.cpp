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
#include "nodes/RayMarchingNode.hpp"
#include "io/Compositor.hpp"
#include "io/NodeView.hpp"
#include "io/PortView.hpp"

#include <GL/glew.h>
#include <iostream>
#include <time.h>
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
    utils::LoadTextFile("shaders/SecondPass.frag", fs);
    Shader::LocationMap postFxLoc = {
        {"time",            { Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",      { Shader::UNIFORM | Shader::FLOAT2} },
        {"colourTexture",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"normalsTexture",  { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"godRaysTexture",  { Shader::UNIFORM | Shader::TEXTURE2D} }
    };
    postEffectShader = new Shader;
    CHECKERROR
    postEffectShader->build( vs, fs, postFxLoc );

    CHECKERROR


    auto nv1 = new io::NodeView(QPointF(0,0), rayMarchingNode);
    auto nv2 = new io::NodeView(QPointF(0,0), timeNode);

    io::Compositor::Instance().add( nv1 );
    io::Compositor::Instance().add( nv2 );

    skyColorNode = nodes::CreateColorNode( glm::vec3(1.0,0.0,0.0) );
    assert( timeNode );
    //assert( skyColorNode->output() >> rayMarchingNode->input(0) );
    nv2->outputs()[0]->connect( nv1->inputs()[0] );
    assert( timeNode->output() >> rayMarchingNode->input(6) );
    assert( winSizeNode->output() >> rayMarchingNode->input(9) );
  }




  void Renderer::drawScene(){

    CHECKERROR
    if( _frameBuffer == 0 ) return;

    timeNode->update();

    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    rayMarchingNode->update();


    glBindVertexArray(0);

    raymarchingShader->unbind();

    FrameBuffer::unbind();
    CHECKERROR

    postEffectShader->bind();
    postEffectShader->uniform2f("windowSize", window.x, window.y );
    postEffectShader->uniform1f("time", 0 );
    postEffectShader->uniform1i("colourTexture", 0 );
    postEffectShader->uniform1i("normalsTexture", 1 );

    CHECKERROR
    
    //  Binding Colour Texture
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, texColour[0]);
    //_frameBuffer->texture(0).bind();
    (*rayMarchingNode->output(1).dataAs<Texture2D*>())->bind();
    CHECKERROR

    //  Binding Normals' Texture
    glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, texNorms[0]);
    //_frameBuffer->texture(1).bind();
    (*rayMarchingNode->output(2).dataAs<Texture2D*>())->bind();
    CHECKERROR
    
    DrawQuad();
    /*
    glBindVertexArray(vaoID[0]);
CHECKERROR
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    CHECKERROR
    glFinish();

    glBindVertexArray(0);
CHECKERROR
    */
    //glActiveTexture(GL_TEXTURE1);
    CHECKERROR
    glBindTexture(GL_TEXTURE_2D, 0);
CHECKERROR

    glActiveTexture(GL_TEXTURE1);
    CHECKERROR
    glBindTexture(GL_TEXTURE_2D, 0);
CHECKERROR

    glActiveTexture(GL_TEXTURE0);
    CHECKERROR
    glBindTexture(GL_TEXTURE_2D, 0);

    postEffectShader->unbind();
    CHECKERROR
  }



  void Renderer::createBuffers()
  {

    std::cout << "Window dimensions: " << window.x << "x" << window.y << std::endl;

    GLint maxBuffers;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxBuffers);
    std::cout << "Max Colour Attachments: " << maxBuffers << std::endl;

    _frameBuffer = (FrameBuffer*)1;

  }

  void Renderer::freeBuffers()
  {

  }
}
