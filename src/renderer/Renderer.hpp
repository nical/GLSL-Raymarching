#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

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

      void createPlane();
  };
} //  namespace

#endif // RENDERER_HPP_INCLUDED
