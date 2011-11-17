
#include "utils/CheckGLError.hpp"
#include <iostream>
#include <GL/glew.h>

using namespace std;

namespace utils{
bool gl_check_error(int line, const char* comment)
{
    GLuint err = glGetError();
    if( err != GL_NO_ERROR )
    {
        cout << "GL error at line " <<line<<" of "<< comment << ": " <<  err <<endl;
        return false;
    }
    return true;
}

}//namespace
