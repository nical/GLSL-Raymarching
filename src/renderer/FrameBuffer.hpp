
#pragma once
#ifndef RENDERER_FRAMEBUFFER_HPP
#define RENDERER_FRAMEBUFFER_HPP

#include <GL/glew.h>
#include <vector>
#include "renderer/Texture.hpp"

namespace renderer{


void ResizeFrameBuffers(int w, int h);


class FrameBuffer{
public:
    typedef std::vector<Texture2D*> TextureArray; 
    FrameBuffer( int nbTextures, int fbwidth, int fbheight );
    ~FrameBuffer();

    GLuint id() const
    {
        return _id;
    }

    void bind() const
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _id);
    }

    static void unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Texture2D& texture(int i) const
    {
        return *_textures[i];
    }

    void resize(int w, int h);
private:
    void init(int nbTextures, int fbwidth, int fbheight);
    void destroy();

    GLuint _nbTex;
    GLuint _id;
    TextureArray _textures;
};

}//namespace

#endif
