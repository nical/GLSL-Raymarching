
#pragma once

#ifndef RENDERER_SHADERPROGRAM_HPP
#define RENDERER_SHADERPROGRAM_HPP

#include <vector>
#include <GL/glew.h>
#include "utils/LoadFile.hpp"

namespace renderer{

class Shader
{
public:

    Shader( GLuint shaderType, const char* fileName )
    {
        _state = false;
        _type = shaderType;
        strcpy( _file, fileName );
        _src = 0;
        shader_vp = glCreateShader(ShaderType);
    }
    
    ~Shader()
    {
        glDeleteShader(_fsId);
        delete _file;
        delete _src;
    }

    GLuint id() const
    {
        return _id;
    }
    
    const char * file() const
    {
        return _file;
    }

    const char * src() const
    {
        return _src;
    }
    
    bool loadSource();
    
    bool build()
    {
        glShaderSource(_id, 1, &_src, 0);
        glCompileShader(_id);
    }
    
    void printLog() const
    {
        validateShader(_id,_file);
    }

private:
    const char * _file;
    const char * _src;
    GLuint _id;
    GLuint _type;
    bool _state;
}


class ShaderProgram
{
public:
    typedef enum { MATRIX4F, FLOAT, DOUBLE, TEXTURE1, TEXTURE2, TEXTURE3 } UniformeType;

    ShaderProgram( Shader* vertexShader, Shader* fragmentShader* );
    

    void printLog() const;

    void build();
    
    void bind() const
    {
        glUseProgram(_id);
    }

    void unbind()
    {
        glUseProgram(0);
    }


    GLuint id() const
    {
        return _id();
    }

private:
    GLuint  _id
    vertexShader* _vs;
    vertexShader* _fs;

    std::vector<UniformeType> _uniforms;
};


}//namespace
