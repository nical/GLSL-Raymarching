
#pragma once

#ifndef RENDERER_SHADER_HPP
#define RENDERER_SHADER_HPP

#include <vector>
#include <GL/glew.h>
#include <string>
#include <map>


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
        Location(GLuint l=0, int t=0) : location(l), type(t) {}
        GLuint location;
        int type;
    };
    typedef int State;
    typedef std::map<std::string,Location> LocationMap;
    typedef LocationMap::const_iterator LocationIterator;
    typedef std::string string;

    enum { NOT_BUILT=0, BINDED=2, BUILD_FAILED=4, VALID=1 };
    enum { INVALID, FLOAT, FLOAT2, FLOAT3, MAT4F };

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

    bool bind()
    {
        if(_state != VALID)
            return false;
        glUseProgram(_id);
        _state &= BINDED;
        return true;
    }

    void unBind()
    {
        _state &= ~BINDED;
        glUseProgram(0);
    }

    void uniform1f(string name, GLfloat value )
    {
        if(_state != BINDED) return;
        glUniform1f(_locations[name].location,value);
    }

    void uniform2f(string name, GLfloat v1, GLfloat v2 )
    {
        if(_state != BINDED) return;
        glUniform2f(_locations[name].location,v1,v2);
    }

    void uniform3f(string name, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        if(_state != BINDED) return;
        glUniform3f(_locations[name].location,v1,v2,v3);
    }

    void uniformMatrix4fv(string name, GLfloat* ptr)
    {
        if(_state != BINDED) return;
        glUniformMatrix4fv(_locations[name].location,1,GL_FALSE, ptr);
    }

protected:
    void addLocation( std::string name, int type )
    {
        _locations[name] = Location(glGetUniformLocation(id(), name.c_str()),type);
    }

private:
    GLuint _vsId;
    GLuint _fsId;
    GLuint _id;
    State _state;
    LocationMap _locations;
};


}//namespace


#endif
