
#include "renderer/FrameBuffer.hpp"
#include <assert.h>
#include <iostream>
#include "utils/CheckGLError.hpp"
#include <vector>

using namespace std;

namespace renderer{

static std::vector<FrameBuffer*> s_frameBuffers;

void ResizeFrameBuffers(int w, int h)
{
    for(unsigned int i = 0; i < s_frameBuffers.size(); ++i)
        s_frameBuffers[i]->resize(w,h);
}

void AddFrameBuffer( FrameBuffer* fbo )
{
    s_frameBuffers.push_back( fbo );
}

void DeleteFrameBuffer( FrameBuffer* fbo )
{
    for(int i = 0; i < s_frameBuffers.size(); ++i)
    {
        if(s_frameBuffers[i] == fbo)
        {
            s_frameBuffers[i] = s_frameBuffers[s_frameBuffers.size()-1];
            s_frameBuffers.resize(s_frameBuffers.size()-1);
        }
    }
}

GLenum getGLColorAttachement( int i )
{
    switch(i)
    {
        case 0 : return GL_COLOR_ATTACHMENT0;
        case 1 : return GL_COLOR_ATTACHMENT1;
        case 2 : return GL_COLOR_ATTACHMENT2;
        case 3 : return GL_COLOR_ATTACHMENT3;
        case 4 : return GL_COLOR_ATTACHMENT4;
        case 5 : return GL_COLOR_ATTACHMENT5;
    }
    assert("bad"=="parameter");
    return 0;
}

FrameBuffer::FrameBuffer( int nbTextures, int fbwidth, int fbheight)
{
    AddFrameBuffer(this);
    init(nbTextures,fbwidth,fbheight);
}

void FrameBuffer::init( int nbTextures, int fbwidth, int fbheight)
{
    cout << "FrameBuffer::init("<< nbTextures <<", "<< fbwidth << ", "<<fbheight <<")"<< endl;

    _nbTex = nbTextures;
    _textures.clear();
    CHECKERROR
    for( int i = 0; i < nbTextures+1; ++i)
    {
        _textures.push_back( new Texture2D );
        glBindTexture( GL_TEXTURE_2D, _textures[i]->id() );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        if ( i == nbTextures ) // depth texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, fbwidth, fbheight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, fbwidth, fbheight, 0, GL_RGBA, GL_FLOAT, 0);
    }
    CHECKERROR
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_UNSUPPORTED)
    {
        glGenFramebuffers(1, &_id);
        std::cout << "Framebuffer Generated!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        std::cout << "Framebuffer Binded! ID: " << _id << std::endl;
        CHECKERROR
        //  Binging Textures to the Framebuffer
        GLenum* attachements = new GLenum[_textures.size()-1];
        for( int i = 0; i < _textures.size()-1; ++i )
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, getGLColorAttachement(i), GL_TEXTURE_2D, _textures[i]->id(), 0);
            attachements[i] = getGLColorAttachement(i);
        }
        CHECKERROR
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _textures[_textures.size()-1]->id(), 0);
        CHECKERROR
        glDrawBuffers(_textures.size()-1, &attachements[0]);
        CHECKERROR
        delete[] attachements;

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
          std::cout << "Fuck!" << std::endl;
        }
    }
    else
    {
      std::cout << "Ok, this is a problem..." << std::endl;
    }
    CHECKERROR


}

void FrameBuffer::destroy()
{
     for( int i = 0; i < _textures.size(); ++i )
        delete _textures[i];

    glDeleteFramebuffers(1, &_id);
}


void FrameBuffer::resize(int w, int h)
{
    //destroy();
    //init(_nbTex,w,h);
    //
    cout << "resize " << w <<" "<<h<<endl;
    glDeleteFramebuffers(1, &_id);
    glGenFramebuffers(1, &_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    for( int i = 0; i < _textures.size(); ++i)
    {
        //GLuint oldTexId = _textures[i]->id();
        //glDeleteTextures(1, &oldTexId );
        Texture2D::unbind();
        _textures[i]->regenerate();
        glBindTexture( GL_TEXTURE_2D, _textures[i]->id() );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        if ( i == _textures.size()-1 ) // depth texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, 0);

        GLenum* attachements = new GLenum[_textures.size()-1];
        for( int i = 0; i < _textures.size()-1; ++i )
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, getGLColorAttachement(i), GL_TEXTURE_2D, _textures[i]->id(), 0);
            attachements[i] = getGLColorAttachement(i);
        }

        CHECKERROR
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _textures[_textures.size()-1]->id(), 0);
        CHECKERROR
        glDrawBuffers(_textures.size()-1, &attachements[0]);
        CHECKERROR
    }
    CHECKERROR
}

FrameBuffer::~FrameBuffer()
{
    DeleteFrameBuffer(this);
    destroy();
}


}//namespace
