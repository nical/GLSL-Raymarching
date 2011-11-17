
#include "renderer/Shader.hpp"
#include "utils/CheckGLError.hpp"

#include <iostream>
//#include <stdlib.h>
//#include <stdio.h>
#include <string.h>
#include <assert.h>

using namespace std;

namespace renderer{



static void validateProgram(GLuint program) {
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;

    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer); // Ask OpenGL to give us the log associated with the program
    if (length > 0) // If we have any information to display
        cout << "Program " << program << " link error: " << buffer << endl; // Output the information

    glValidateProgram(program); // Get OpenGL to try validating the program
    GLint status;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status); // Find out if the shader program validated correctly
    if (status == GL_FALSE) // If there was a problem validating
		cout << "Error validating shader " << program << endl; // Output which program had the error
}


bool Shader::build(const string& vs_src,const string& fs_src, const LocationMap& locations)
{
    cout << "Shader::build" << endl;
    CHECKERROR
    const char* vs_text = vs_src.c_str();
    const char* fs_text = fs_src.c_str();

    glShaderSource(_vsId, 1, &vs_text, 0); 
    glCompileShader(_vsId);
    CHECKERROR
    glShaderSource(_fsId, 1, &fs_text, 0);
    glCompileShader(_fsId);

    CHECKERROR
    _id = glCreateProgram(); 
    glAttachShader(_id, _vsId);
    glAttachShader(_id, _fsId);

    _locations = locations;

    CHECKERROR
    glBindAttribLocation(_id, 0, "in_Position");
    glBindAttribLocation(_id, 1, "in_Color");

    CHECKERROR
    glLinkProgram(_id);
    
    for(auto it = _locations.begin(); it != _locations.end(); ++it)
    {
        if(it->second.type & UNIFORM)
        {
            it->second.location = glGetUniformLocation( _id, it->first.c_str() );
            cout << "location: " << it->first << endl;
            CHECKERROR
        }
    }

    CHECKERROR
    validateProgram(_id);

    _state |= VALID;
    cout << "shader state: "<<_state << endl;
    return true;
}

bool Shader::bind()
{
    /*
    if( !(_state & VALID) )
    {
        cout << "Shader::bind Error! invalid state "<< _state << endl;
        assert(false);
        return false;
    }*/
    glUseProgram(_id);
    _state &= BINDED;
    return true;
}

bool Shader::hasLocation(const std::string& name) const
{
    for(auto it = locations_begin(); it!= locations_end(); ++it )
    {
        if( name == it->first )
            return true;
    }
    return false;
}

}//namespace
