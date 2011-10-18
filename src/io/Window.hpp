#pragma once
#ifndef IO_WINDOW_HPP
#define IO_WINDOW_HPP

namespace renderer{
  class Renderer;
}

namespace io{

  void InitWindow(renderer::Renderer* r, unsigned int w, unsigned int h, const char* name, int argc, char* argv[]);

}//namespace


#endif
