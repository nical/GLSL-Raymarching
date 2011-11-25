#include "renderer/Renderer.hpp"
#include "utils/LoadFile.hpp"
#include "renderer/Shader.hpp"
#include "utils/CheckGLError.hpp"
#include "renderer/FrameBuffer.hpp"

#include "kiwi/core/all.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GL/glew.h>
#include <iostream>
#include <time.h>
#include <initializer_list>


using namespace std;

namespace renderer{


  Renderer::~Renderer(){
  }

  void Renderer::init(){
      
    CHECKERROR
    
    createPlane();

    viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));

    auto mat4TypeInfo = kiwi::core::DataTypeManager::TypeOf("Mat4");
    auto uintTypeInfo = kiwi::core::DataTypeManager::TypeOf("GLuint");
    auto vec2TypeInfo = kiwi::core::DataTypeManager::TypeOf("Vec2");
    auto vec3TypeInfo = kiwi::core::DataTypeManager::TypeOf("Vec3");
    auto textureTypeInfo = kiwi::core::DataTypeManager::TypeOf("Texture2D");
    

    viewMatNode = kiwi::core::NodeTypeManager::Create("Mat4");
    winSizeNode = kiwi::core::NodeTypeManager::Create("Vec2");
    *viewMatNode->output().data()->value<glm::mat4>() = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    fuffaTime = 0;

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
        {"fuffaTime",       { Shader::UNIFORM | Shader::FLOAT} },
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

    
    //RegisterShaderNode("RayMarcher", *raymarchingShader );
    kiwi::core::NodeLayoutDescriptor raymacherLayout; 
    raymacherLayout.inputs = {
        {"skyColor", vec3TypeInfo, kiwi::READ },
        {"groundColor", vec3TypeInfo, kiwi::READ },
        {"buildingsColor", vec3TypeInfo, kiwi::READ },
        {"shadowColor", vec3TypeInfo, kiwi::READ },
        {"viewMatrix", mat4TypeInfo, kiwi::READ },
        {"fuffaTime", uintTypeInfo, kiwi::READ },
        {"windowSize", vec2TypeInfo, kiwi::READ }
    };
    raymacherLayout.outputs = {
        {"color", textureTypeInfo, kiwi::READ },
        {"normals", textureTypeInfo, kiwi::READ },
        {"godRays", textureTypeInfo, kiwi::READ }
    };

    CHECKERROR
    vs.clear();
    fs.clear();

    utils::LoadTextFile("shaders/SecondPass.vert", vs);
    utils::LoadTextFile("shaders/SecondPass.frag", fs);
    Shader::LocationMap postFxLoc = {
        {"fuffaTime",       { Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",      { Shader::UNIFORM | Shader::FLOAT2} },
        {"colourTexture",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"normalsTexture",  { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"godRaysTexture",  { Shader::UNIFORM | Shader::TEXTURE2D} }
    };
    postEffectShader = new Shader;
    CHECKERROR
    postEffectShader->build( vs, fs, postFxLoc );

    CHECKERROR
  }

  void Renderer::drawScene(){

    CHECKERROR
    if( _frameBuffer == 0 ) return;
    
    //glEnable(GL_TEXTURE_2D);
    _frameBuffer->bind();
    
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    raymarchingShader->bind();
    raymarchingShader->uniformMatrix4fv("viewMatrix", &viewMatrix[0][0] );
    raymarchingShader->uniform3f("shadowColor", 0.0, 0.3, 0.7 );
    raymarchingShader->uniform3f("buildingsColor", 1.0, 1.0, 1.0 );
    raymarchingShader->uniform3f("groundColor", 1.0, 1.0, 1.0 );
    raymarchingShader->uniform3f("redColor", 1.0, 0.1, 0.1 );
    raymarchingShader->uniform3f("skyColor", 0.9, 1.0, 1.0 );
    raymarchingShader->uniform2f("windowSize", window.x, window.y );
    CHECKERROR
    raymarchingShader->uniform1f("fuffaTime", fuffaTime );
    CHECKERROR
    raymarchingShader->uniform1f("fovyCoefficient", 1.0 );
    CHECKERROR
    raymarchingShader->uniform1f("shadowHardness", 7.0f );
    CHECKERROR
    fuffaTime++;

    glBindVertexArray(vaoID[0]);
    CHECKERROR
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CHECKERROR
#ifdef DEBUG
    glFinish();
#endif

    glBindVertexArray(0);

    raymarchingShader->unbind();

    FrameBuffer::unbind();
    CHECKERROR

    postEffectShader->bind();
    postEffectShader->uniform2f("windowSize", window.x, window.y );
    postEffectShader->uniform1f("fuffaTime", fuffaTime );
    postEffectShader->uniform1i("colourTexture", 0 );
    postEffectShader->uniform1i("normalsTexture", 1 );
    postEffectShader->uniform1i("godRaysTexture", 2 );

    CHECKERROR
    
    //  Putting data in the uniforms
    //glUniform1i(postEffectShader->getLocation("colourTexture"), 0);
    //glUniform1i(postEffectShader->getLocation("normalsTexture"), 1);

    //  Binding Colour Texture
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, texColour[0]);
    _frameBuffer->texture(0).bind();
    CHECKERROR

    //  Binding Normals' Texture
    glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, texNorms[0]);
    _frameBuffer->texture(1).bind();
    CHECKERROR
    
    //  Binding Normals' Texture
    glActiveTexture(GL_TEXTURE2);
    //glBindTexture(GL_TEXTURE_2D, texNorms[0]);
    _frameBuffer->texture(2).bind();
    CHECKERROR

    glBindVertexArray(vaoID[0]);
CHECKERROR
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    CHECKERROR
    glFinish();

    glBindVertexArray(0);
CHECKERROR
    
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
  void Renderer::createPlane()
  {
    if (!GLEW_ARB_vertex_array_object)
      std::cerr << "ARB_vertex_array_object not available." << std::endl;

    GLfloat vertices[12] = {
      -1.0, -1.0, -1.0,
      -1.0, 1.0, -1.0,
      1.0, -1.0, -1.0,
      1.0, 1.0, -1.0
    };


    GLuint indices[6]={0,1,2,1,2,3};

    glGenVertexArrays(1, &vaoID[0]);

    glBindVertexArray(vaoID[0]);


    // Generate and bind Vertex Buffer Objects
    glGenBuffers(1, &vboID[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);


    // Load the buffer with the vertices and set its attributes
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);


    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

  }

  void Renderer::createBuffers()
  {

    std::cout << "Window dimensions: " << window.x << "x" << window.y << std::endl;

    GLint maxBuffers;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxBuffers);
    std::cout << "Max Colour Attachments: " << maxBuffers << std::endl;

    _frameBuffer = new FrameBuffer(3, window.x, window.y);

  }

  void Renderer::freeBuffers(){
    if( _frameBuffer != 0 )
        delete _frameBuffer;
  }
}
