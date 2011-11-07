#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shaderLoader/shader.hpp"

namespace renderer{

  class Renderer {

    public:

      Renderer();
      ~Renderer();

      void init();
      void drawScene();

    private:

      unsigned int vaoID[1];
      unsigned int vboID[1];

      glm::mat4 projectionMatrix;
      glm::mat4 viewMatrix;
      glm::mat4 modelMatrix;

      Shader *planeShader;

      void createPlane();
  };
} //  namespace

#endif // RENDERER_HPP_INCLUDED
