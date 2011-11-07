#include "renderer/Renderer.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

namespace renderer{

  Renderer::Renderer(){

  }

  Renderer::~Renderer(){

  }

  void Renderer::init(){
    createPlane();

    planeShader = new Shader ("shaders/Raymarching.vert", "shaders/Raymarching.frag");

    projectionMatrix = glm::ortho (0.0, 1.0, 0.0, 1.0);
    viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
    modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
  }

  void Renderer::drawScene(){
    planeShader->bind();
    GLint projectionMatrixLocation = glGetUniformLocation(planeShader->id(), "projectionMatrix");
    GLint viewMatrixLocation = glGetUniformLocation(planeShader->id(), "viewMatrix");
    GLint modelMatrixLocation = glGetUniformLocation(planeShader->id(), "modelMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    glBindVertexArray(vaoID[0]);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    planeShader->unbind();
  }

  void Renderer::createPlane(){
    GLfloat vertices[12] = {
      0.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      1.0, 0.0, 0.0,
      1.0, 1.0, 0.0
    };

    //  Generate and bind Vertex Array
    glGenVertexArrays(1, &vaoID[0]);
    glBindVertexArray(vaoID[0]);

    //  Generate and bind Vertex Buffer Objects
    glGenBuffers(1, &vboID[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);

    //  Load the buffer with the vertices and set its attributes
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
  }

}
