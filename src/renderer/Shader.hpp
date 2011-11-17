
#pragma once

#ifndef RENDERER_SHADER_HPP
#define RENDERER_SHADER_HPP

#include <vector>
#include <GL/glew.h>
#include <string>
#include <map>
#include <assert.h>


namespace renderer{

/**
 * Mother class for shader builders.
 *
 * Has access the Shader's protected mumbers.
 */ 
class ShaderBuilder
{
public:
    
};

class Shader
{
friend class ShaderBuilder;
public:
    struct Location
    {
        Location( int t=0 ) : location(0), type(t) {}
        GLuint location;
        int type;
    };
    typedef int State;
    typedef std::map<std::string,Location> LocationMap;
    typedef LocationMap::const_iterator LocationIterator;
    typedef std::string string;

    enum { NOT_BUILT=0, BINDED=2, BUILD_FAILED=4, VALID=1 };
    enum { UNIFORM=1, ATTRIBUTE=2, OUTPUT=4, INVALID=8
        , FLOAT=16, FLOAT2=32, FLOAT3=64, MAT4F=128, SAMPLER2D = 256 };

    Shader()
    {
        _state = NOT_BUILT;
        _vsId = glCreateShader(GL_VERTEX_SHADER);
        _fsId = glCreateShader(GL_FRAGMENT_SHADER);
        _id   = glCreateProgram();
    }
    
    ~Shader()
    {
        glDetachShader(_id, _fsId);
        glDetachShader(_id, _vsId);
        glDeleteShader(_vsId);
        glDeleteShader(_fsId);
        glDeleteProgram(_id); 
    }

    GLuint id() const
    {
        return _id;
    }

    State state() const
    {
        return _state;
    }

    LocationIterator locations_begin() const
    {
        return _locations.begin();
    }

    LocationIterator locations_end() const
    {
        return _locations.end();
    }

    bool build(const string& vsSrc,const string& fsSrc, const LocationMap& locations);

    bool bind();
    
    void unbind()
    {
        _state &= ~BINDED;
        glUseProgram(0);
    }

    bool hasLocation(const std::string& name) const;
    
    void uniform1i(string name, GLint value )
    {
        assert( hasLocation(name) );
        //if(_state != BINDED) return;
        glUniform1i(_locations[name].location,value);
    }

    void uniform1f(string name, GLfloat value )
    {
        assert( hasLocation(name) );
        //if(_state != BINDED) return;
        glUniform1f(_locations[name].location,value);
    }

    void uniform2f(string name, GLfloat v1, GLfloat v2 )
    {
        assert( hasLocation(name) );
        //if(_state != BINDED) return;
        glUniform2f(_locations[name].location,v1,v2);
    }

    void uniform3f(string name, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        assert( hasLocation(name) );
        //if(_state != BINDED) return;
        glUniform3f(_locations[name].location,v1,v2,v3);
    }

    void uniformMatrix4fv(string name, GLfloat* ptr)
    {
        assert( hasLocation(name) );
        //if(_state != BINDED) return;
        glUniformMatrix4fv(_locations[name].location,1,GL_FALSE, ptr);
    }

protected:


private:
    GLuint _vsId;
    GLuint _fsId;
    GLuint _id;
    State _state;
    LocationMap _locations;
};


}//namespace


#endif
