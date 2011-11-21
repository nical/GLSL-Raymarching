#include "renderer/Renderer.hpp"
#include "utils/LoadFile.hpp"
#include "renderer/Shader.hpp"
#include "utils/CheckGLError.hpp"

#include "kiwi/core/all.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
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
    projectionMatrix = glm::ortho (0.0, 1.0, 0.0, 1.0);
    viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
    modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    projMatNode = kiwi::core::NodeTypeManager::Create("Mat4");
    viewMatNode = kiwi::core::NodeTypeManager::Create("Mat4");
    modelMatNode = kiwi::core::NodeTypeManager::Create("Mat4");
    winSizeNode = kiwi::core::NodeTypeManager::Create("Vec2");
    
    *projMatNode->output().dataAs<glm::mat4>() = glm::ortho(0.0, 1.0, 0.0, 1.0);
    *viewMatNode->output().dataAs<glm::mat4>() = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
    *modelMatNode->output().dataAs<glm::mat4>() = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    assert( *projMatNode->output().dataAs<glm::mat4>() == projectionMatrix );

    //glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    fuffaTime = 0;

    // shaders

    CHECKERROR
    string vs, fs;
    utils::LoadTextFile("shaders/Raymarching.vert", vs);
    utils::LoadTextFile("shaders/Raymarching.frag", fs);
    Shader::LocationMap marcherLoc = {
        {"projectionMatrix",{ Shader::UNIFORM | Shader::MAT4F} },
        {"viewMatrix",      { Shader::UNIFORM | Shader::MAT4F} },
        {"modelMatrix",     { Shader::UNIFORM | Shader::MAT4F} },
        {"fuffaTime",       { Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",      { Shader::UNIFORM | Shader::FLOAT2} },
        {"colourTexture",   { Shader::OUTPUT  | Shader::SAMPLER2D} },
        {"normalsTexture",  { Shader::OUTPUT  | Shader::SAMPLER2D} }
    };
    raymarchingShader= new Shader();
    CHECKERROR
    raymarchingShader->build( vs, fs, marcherLoc );

    auto mat4TypeInfo = kiwi::core::DataTypeManager::TypeOf("Mat4");
    auto uintTypeInfo = kiwi::core::DataTypeManager::TypeOf("GLuint");
    auto vec2TypeInfo = kiwi::core::DataTypeManager::TypeOf("Vec2");
    //RegisterShaderNode("RayMarcher", *raymarchingShader );
    kiwi::core::NodeLayoutDescriptor raymacherLayout; 
    raymacherLayout.inputs = {
        {"projectionMatrix", mat4TypeInfo, kiwi::READ },
        {"viewMatrix", mat4TypeInfo, kiwi::READ },
        {"modelMatrix", mat4TypeInfo, kiwi::READ },
        {"fuffaTime", uintTypeInfo, kiwi::READ },
        {"windowSize", vec2TypeInfo, kiwi::READ }
    };

    CHECKERROR
    vs.clear();
    fs.clear();

    utils::LoadTextFile("shaders/SecondPass.vert", vs);
    utils::LoadTextFile("shaders/SecondPass.frag", fs);
    Shader::LocationMap postFxLoc = {
        {"projectionMatrix",{ Shader::UNIFORM | Shader::MAT4F} },
        {"viewMatrix",      { Shader::UNIFORM | Shader::MAT4F} },
        {"modelMatrix",     { Shader::UNIFORM | Shader::MAT4F} },
        {"fuffaTime",       { Shader::UNIFORM | Shader::FLOAT} },
        {"windowSize",      { Shader::UNIFORM | Shader::FLOAT2} },
        {"colourTexture",   { Shader::UNIFORM | Shader::SAMPLER2D} },
        {"normalsTexture",  { Shader::UNIFORM | Shader::SAMPLER2D} }
    };
    postEffectShader = new Shader;
    CHECKERROR
    postEffectShader->build( vs, fs, postFxLoc );

    CHECKERROR
  }

  void Renderer::drawScene(){
    CHECKERROR
    //glEnable(GL_TEXTURE_2D);
    glBindFramebuffer(GL_FRAMEBUFFER, bufID[0]);

    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    raymarchingShader->bind();
    raymarchingShader->uniformMatrix4fv("projectionMatrix", &projectionMatrix[0][0] );
    raymarchingShader->uniformMatrix4fv("viewMatrix", &viewMatrix[0][0] );
    raymarchingShader->uniformMatrix4fv("modelMatrix", &modelMatrix[0][0] );
    raymarchingShader->uniform2f("windowSize", window.x, window.y );
    raymarchingShader->uniform1f("fuffaTime", fuffaTime );

    fuffaTime++;

    glBindVertexArray(iboID[0]);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#ifdef DEBUG
    glFinish();
#endif

    glBindVertexArray(0);

    raymarchingShader->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CHECKERROR


    postEffectShader->bind();
    postEffectShader->uniformMatrix4fv("projectionMatrix", &projectionMatrix[0][0] );
    postEffectShader->uniformMatrix4fv("viewMatrix", &viewMatrix[0][0] );
    postEffectShader->uniformMatrix4fv("modelMatrix", &modelMatrix[0][0] );
    postEffectShader->uniform2f("windowSize", window.x, window.y );
    postEffectShader->uniform1f("fuffaTime", fuffaTime );
    postEffectShader->uniform1i("colourTexture", 0 );
    postEffectShader->uniform1i("normalsTexture", 1 );
    CHECKERROR
    
    //  Putting data in the uniforms
    //glUniform1i(postEffectShader->getLocation("colourTexture"), 0);
    //glUniform1i(postEffectShader->getLocation("normalsTexture"), 1);

    //  Binding Colour Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texColour[0]);
    CHECKERROR

    //  Binding Normals' Texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texNorms[0]);
    CHECKERROR

    glBindVertexArray(iboID[0]);
    CHECKERROR

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    CHECKERROR
    glFinish();

    glBindVertexArray(0);

    //glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);


    postEffectShader->unbind();
    CHECKERROR
  }
  void Renderer::createPlane()
  {
    if (!GLEW_ARB_vertex_array_object)
      std::cerr << "ARB_vertex_array_object not available." << std::endl;

    GLfloat vertices[12] = {
      0.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      1.0, 0.0, 0.0,
      1.0, 1.0, 0.0
    };


    GLuint indices[6]={0,1,2,1,2,3};

    glGenVertexArrays(1, &iboID[0]);
    glBindVertexArray(iboID[0]);

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

    //  Colour Renderbuffer

    glGenTextures(1, &texColour[0]);
    glBindTexture(GL_TEXTURE_2D, texColour[0]);
    std::cout << "TexColour ID: " << texColour[0] << std::endl;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window.x, window.y, 0, GL_RGBA, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //  Normals Texture

    glGenTextures(1, &texNorms[0]);
    glBindTexture(GL_TEXTURE_2D, texNorms[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window.x, window.y, 0, GL_RGBA, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //  Depth Texture, created for compatibility

    glGenTextures(1, &texDepth[0]);
    glBindTexture(GL_TEXTURE_2D, texDepth[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, window.x, window.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    //  Generate and bind the framebuffers

    std::cout << "Textures Generated!" << std::endl;

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_UNSUPPORTED){
      glGenFramebuffers(1, &bufID[0]);
      std::cout << "Framebuffer Generated!" << std::endl;
      glBindFramebuffer(GL_FRAMEBUFFER, bufID[0]);
      std::cout << "Framebuffer Binded! ID: " << bufID[0] << std::endl;

        //  Binging Textures to the Framebuffer

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColour[0],0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texNorms[0], 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepth[0], 0);

        GLenum shaderBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, shaderBuffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
          std::cout << "Fuck!" << std::endl;
        }
    } else {
      std::cout << "Ok, this is a problem..." << std::endl;
    }

    //  Unbind the Framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

  }

  void Renderer::freeBuffers(){
    glDeleteTextures(1, &texColour[0]);
    glDeleteTextures(1, &texNorms[0]);
    glDeleteTextures(1, &texDepth[0]);
    glDeleteFramebuffers(1, &bufID[0]);
  }
}
