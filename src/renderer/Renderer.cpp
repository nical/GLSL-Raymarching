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

    //glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    fuffaTime = 0;
  }

  void Renderer::drawScene(){
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_INDEX_ARRAY);

    planeShader->bind();

    GLint projectionMatrixLocation = glGetUniformLocation(planeShader->id(), "projectionMatrix");
    GLint viewMatrixLocation = glGetUniformLocation(planeShader->id(), "viewMatrix");
    GLint modelMatrixLocation = glGetUniformLocation(planeShader->id(), "modelMatrix");
    GLint timeLocation = glGetUniformLocation(planeShader->id(), "fuffaTime");

    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform1f(timeLocation, fuffaTime);
    fuffaTime++;

    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glVertexPointer(3,GL_FLOAT,0,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID[0]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

    //glBindBuffer(GL_ARRAY_BUFFER,0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    planeShader->unbind();

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

}
