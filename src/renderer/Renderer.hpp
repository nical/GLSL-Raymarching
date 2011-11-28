#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "kiwi/core/all.hpp"

namespace renderer{
  class Shader;
  class FrameBuffer;
  
class Renderer 
{
public:

  Renderer(unsigned int x, unsigned int y){
    window.x = x;
    window.y = y;
    bufID[0] = 0;
    _frameBuffer = 0;
  }
  ~Renderer();

  void init();
  void drawScene();

  void setWindowDimensions (unsigned int x, unsigned int y);

  void drawQuad();

  void createBuffers();

private:
  FrameBuffer* _frameBuffer;
  
  struct {
    unsigned int x;
    unsigned int y;
  } window;

  glm::mat4 viewMatrix;

  kiwi::core::Node * viewMatNode; 
  kiwi::core::Node * winSizeNode;
  kiwi::core::Node * timeNode;
  kiwi::core::Node * skyColorNode;
  kiwi::core::Node * groundColorNode;
  kiwi::core::Node * buildingsColorNode;
  kiwi::core::Node * redColorNode;
  
  kiwi::core::Node * rayMarchingNode;
  kiwi::core::Node * radialBlurNode;
  kiwi::core::Node * edgeDetectionNode;

  kiwi::core::Pipeline * renderPipeline;

  Shader *raymarchingShader;
  Shader *postEffectShader;
  unsigned int vaoID[1];
  unsigned int tcoID[1];
  unsigned int vboID[1];
  unsigned int bufID[1];

  GLuint fuffaTime;

  void freeBuffers();
  void createPlane();
};


} //  namespace

#endif // RENDERER_HPP_INCLUDED
