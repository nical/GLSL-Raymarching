#include "renderer/Renderer.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <time.h>

using namespace std;

#define CHECKERROR __glcheckerror(__LINE__,__PRETTY_FUNCTION__);

void __glcheckerror(int line, const char* comment = "")
  {
    GLuint err = glGetError();
    if( err != GL_NO_ERROR )
    {
        cout << "gl error at " <<line<<" "<< comment << ": " <<  err <<endl;
    }
  }
namespace renderer{


  Renderer::~Renderer(){
  }

  void Renderer::init(){

    createPlane();

    planeShader = new Shader ("shaders/Raymarching.vert", "shaders/Raymarching.frag");

    planeShader->addLocation("projectionMatrix");
    planeShader->addLocation("viewMatrix");
    planeShader->addLocation("modelMatrix");
    planeShader->addLocation("fuffaTime");
    planeShader->addLocation("windowSize");

    postEffectShader = new Shader ("shaders/SecondPass.vert", "shaders/SecondPass.frag");

    postEffectShader->addLocation("projectionMatrix");
    postEffectShader->addLocation("viewMatrix");
    postEffectShader->addLocation("modelMatrix");
    postEffectShader->addLocation("fuffaTime");
    postEffectShader->addLocation("windowSize");
    postEffectShader->addLocation("colourTexture");
    postEffectShader->addLocation("normalsTexture");

    projectionMatrix = glm::ortho (0.0, 1.0, 0.0, 1.0);
    viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
    modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));


    //glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


    fuffaTime = 0;

  }

  void Renderer::drawScene(){

    //glEnable(GL_TEXTURE_2D);
    glBindFramebuffer(GL_FRAMEBUFFER, bufID[0]);


    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    planeShader->bind();

    glUniformMatrix4fv(planeShader->getLocation("projectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(planeShader->getLocation("viewMatrix"), 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(planeShader->getLocation("modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform2f(planeShader->getLocation("windowSize"), window.x, window.y);
    glUniform1f(planeShader->getLocation("fuffaTime"), fuffaTime);



    fuffaTime++;


    glBindVertexArray(vaoID[0]);


    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


    glFinish();




    glBindVertexArray(0);


    planeShader->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    postEffectShader->bind();

    //  Putting data in the uniforms
    glUniformMatrix4fv(postEffectShader->getLocation("projectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(postEffectShader->getLocation("viewMatrix"), 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(postEffectShader->getLocation("modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform2f(postEffectShader->getLocation("windowSize"), window.x, window.y);
    glUniform1f(postEffectShader->getLocation("fuffaTime"), fuffaTime);
    glUniform1i(postEffectShader->getLocation("colourTexture"), 0);
    glUniform1i(postEffectShader->getLocation("normalsTexture"), 1);

    //  Binding Colour Texture
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texColour[0]);


    //  Binding Normals' Texture
    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, texNorms[0]);


    glBindVertexArray(vaoID[0]);

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glFinish();


    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    postEffectShader->unbind();


    GLuint err = glGetError();
    if( err != GL_NO_ERROR )
    {
        cout << "gl error! " <<  err <<endl;

    }
  }
  void Renderer::createPlane(){

    if (!GLEW_ARB_vertex_array_object)
      std::cerr << "ARB_vertex_array_object not available." << std::endl;

    GLfloat vertices[12] = {
      0.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      1.0, 0.0, 0.0,
      1.0, 1.0, 0.0
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

  void Renderer::createBuffers(){


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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window.x, window.y, 0, GL_RGBA, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);


    //  Normals Texture

    glGenTextures(1, &texNorms[0]);
    glBindTexture(GL_TEXTURE_2D, texNorms[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window.x, window.y, 0, GL_RGBA, GL_FLOAT, 0);
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

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColour[0], 0);
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
