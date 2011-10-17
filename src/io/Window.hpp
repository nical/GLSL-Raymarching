#pragma once
#ifndef IO_WINDOW_HPP
#define IO_WINDOW_HPP

namespace io{

class Window
{
public:
    Window( render::Renderer* r, unsigned int w, unsigned int h, const char* name)
    : _renderer(r), _width(w), _height(h), _caption(name)
    {
        
    }
    
    // glut stuff here

private:
    render::Renderer* _renderer;
    unsigned int _width;
    unsigned int _height;
    const char* _caption
}


}//namespace


#endif
