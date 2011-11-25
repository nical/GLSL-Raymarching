
#include "renderer/DrawQuad.hpp"

#include <GL/glew.h>

namespace renderer{

static GLuint _VAO;
static GLuint _VBO;

void DrawQuad()
{
    CHECKERROR
    glBindVertexArray( _VAO );
    CHECKERROR
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CHECKERROR
}

void InitQuad()
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

    glGenVertexArrays(1, &_VAO);

    glBindVertexArray(_VAO);


    // Generate and bind Vertex Buffer Objects
    glGenBuffers(1, &_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);


    // Load the buffer with the vertices and set its attributes
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);


    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

}

void DeleteQuad()
{
    // TODO
}

}//namespace
