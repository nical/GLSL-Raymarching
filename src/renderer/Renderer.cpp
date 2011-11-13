#include "renderer/Renderer.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <time.h>

using namespace std;

namespace renderer{

  Renderer::~Renderer(){

  }

  void Renderer::init(){
    createPlane();
    planeShader = new Shader ("shaders/Raymarching.vert", "shaders/Raymarching.frag");
    postEffectShader = new Shader ("shaders/SecondPass.vert", "shaders/SecondPass.frag");
    projectionMatrix = glm::ortho (0.0, 1.0, 0.0, 1.0);
    viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
    modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    //glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    fuffaTime = 0;
  }

  void Renderer::drawScene(){
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_INDEX_ARRAY);

    glBindFramebuffer(GL_FRAMEBUFFER_EXT, bufID[0]);
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    GLenum shaderBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, shaderBuffers);

    planeShader->bind();

    GLint projectionMatrixLocation = glGetUniformLocation(planeShader->id(), "projectionMatrix");
    GLint viewMatrixLocation = glGetUniformLocation(planeShader->id(), "viewMatrix");
    GLint modelMatrixLocation = glGetUniformLocation(planeShader->id(), "modelMatrix");
    GLint timeLocation = glGetUniformLocation(planeShader->id(), "fuffaTime");
    GLint windowSizeLocation = glGetUniformLocation(planeShader->id(), "windowSize");

    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform2f(windowSizeLocation, window.x, window.y);
    glUniform1f(timeLocation, fuffaTime);
    fuffaTime++;

    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glVertexPointer(3,GL_FLOAT,0,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID[0]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
    glFinish();
    //glBindBuffer(GL_ARRAY_BUFFER,0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    planeShader->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    postEffectShader->bind();

    projectionMatrixLocation = glGetUniformLocation(postEffectShader->id(), "projectionMatrix");
    viewMatrixLocation = glGetUniformLocation(postEffectShader->id(), "viewMatrix");
    modelMatrixLocation = glGetUniformLocation(postEffectShader->id(), "modelMatrix");
    timeLocation = glGetUniformLocation(postEffectShader->id(), "fuffaTime");
    windowSizeLocation = glGetUniformLocation(postEffectShader->id(), "windowSize");

    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform2f(windowSizeLocation, window.x, window.y);

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    GLint textureLocation = glGetUniformLocation(postEffectShader->id(), "colourTexture");
    glUniform1i(textureLocation, 0);
    glBindTexture(GL_TEXTURE_2D, texColour[0]);

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE1);
    GLint normalsLocation = glGetUniformLocation(postEffectShader->id(), "normalsTexture");
    glUniform1i(normalsLocation, 1);
    glBindTexture(GL_TEXTURE_2D, texNorms[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glVertexPointer(3,GL_FLOAT,0,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID[0]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
    glFinish();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    postEffectShader->unbind();

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


    //vbo
    //  Generate and bind Vertex Buffer Objects
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);

    glGenBuffers(1, &vboID[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    //ibo
    glGenBuffers(1, &iboID[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    //Specify that our coordinate data is going into attribute index 0(shaderAtribute), and contains three floats per vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Enable attribute index 0(shaderAtribute) as being used
    glEnableVertexAttribArray(0);
  }

  void Renderer::createBuffers(){

    //  Generate and bind the framebuffers

    glGenFramebuffers(1, &bufID[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, bufID[0]);

    //  Generate and bind Textures to the Framebuffer

    //  Colour Texture
    glGenTextures(1, &texColour[0]);
    glBindTexture(GL_TEXTURE_2D, texColour[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window.x, window.y, 0, GL_RGBA, GL_FLOAT, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColour[0], 0);

    //  Normals Texture
    glGenTextures(1, &texNorms[0]);
    glBindTexture(GL_TEXTURE_2D, texNorms[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window.x, window.y, 0, GL_RGBA, GL_FLOAT, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texNorms[0], 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      std::cout << "Fuck!" << std::endl;
    }

    //  Unbind the Framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

  }
}
