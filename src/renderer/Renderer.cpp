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

    glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
  }

  void Renderer::drawScene(){
    glBindVertexArray(vaoID[0]);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);

  }

  void Renderer::createPlane(){
    GLfloat vertices[12] = {
      0.0, 0.0, 0.0,
      10.0, 0.0, 0.0,
      10.0, 10.0, 0.0,
      0.0, 10.0, 0.0
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
