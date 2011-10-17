
#pragma once

#ifndef RENDERER_SHADERPROGRAM_HPP
#define RENDERER_SHADERPROGRAM_HPP


namespace renderer{


class ShaderProgram
{
public:
    ShaderProgram(const char* vs_source, const char* fs_shader)
    _Id(0), _vsId(0), _fsId(0), _vs(vs_shader), _fs(fs_shader) 
    {
        // TODO !
    }

    bool compile();

    void bind();

private:
    GLuint  _Id
    GLuint  _vsId
    GLuint  _fsId

    const char* _vs
    const char* _fs
};


}//namespace
