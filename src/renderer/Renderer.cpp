#include "renderer/Renderer.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

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

  }

  void Renderer::drawScene(){

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);

    planeShader->bind();
    GLint projectionMatrixLocation = glGetUniformLocation(planeShader->id(), "projectionMatrix");
    GLint viewMatrixLocation = glGetUniformLocation(planeShader->id(), "viewMatrix");
    GLint modelMatrixLocation = glGetUniformLocation(planeShader->id(), "modelMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID[0]);

    glVertexPointer(3,GL_FLOAT,0,0);

    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
    glDrawArrays(GL_TRIANGLES,0,3);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glDisable(GL_CULL_FACE);
    /*
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0,0.0);glColor3f(1.0,10,1.0);
        glVertex2f(1.0,0.0);glColor3f(1.0,10,1.0);
        glVertex2f(1.0,1.0);glColor3f(1.0,10,1.0);
    glEnd();
    */
    glBindVertexArray(0);
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

    GLuint indices[6]={1,3,2,2,3,4};

    //vbo

    //  Generate and bind Vertex Buffer Objects
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);

    glGenBuffers(1, &vboID[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    //  allocatebufferongpu
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    // copy vertices from cpu to gpu
    //glBufferSubData(GL_ARRAY_BUFFER,0,12,vertices);

    //ibo

    glGenBuffers(1, &iboID[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID[0]);
    //  allocatebufferongpu
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    // copy vertices from cpu to gpu
    //glBufferSubData(GL_INDEX_BUFFER,0,6,indices);


    //glEnableVertexAttribArray(0);
    //glBindVertexArray(0);
  }

}
