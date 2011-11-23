
#pragma once
#ifndef RENDERER_TEXTURE_HPP
#define RENDERER_TEXTURE_HPP

#include <GL/glew.h>

namespace renderer{

template< GLuint T >
class Texture {
public:
    static const GLuint TextureType = T;

    Texture()
    {
        glGenTextures(1, &_id);
    }

    GLuint id() const
    {
        return _id;
    }

    void bind() const
    {
        glBindTexture(TextureType, _id);
    }

    static void unbind()
    {
        glBindTexture(TextureType, 0);
    }

    ~Texture()
    {
        glDeleteTextures(1, &_id);
    }
    
private:
    GLuint _id;
};

typedef Texture<GL_TEXTURE_2D> Texture2D;
typedef Texture<GL_TEXTURE_1D> Texture1D;
typedef Texture<GL_TEXTURE_3D> Texture3D;

}//namespace


#endif
