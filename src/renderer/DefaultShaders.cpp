
#include "renderer/DefaultShader.hpp"

namespace renderer{ 

static const GLchar* VertexShader =
{
    "#version 300\n"\
 
    "layout(location=0) in vec4 in_Position;\n"\
    "layout(location=1) in vec4 in_Color;\n"\
    "out vec4 ex_Color;\n"\
 
    "void main(void)\n"\
    "{\n"\
    "   gl_Position = in_Position;\n"\
    "   ex_Color = in_Color;\n"\
    "}\n"
};

static const GLchar* FragmentShader =
{
    "#version 400\n"\
 
    "in vec4 ex_Color;\n"\
    "out vec4 out_Color;\n"\
 
    "void main(void)\n"\
    "{\n"\
    "   out_Color = ex_Color;\n"\
    "}\n"
};



const GLchar* DefaultVertexShaderSrc()
{
    return VertexShader;
}

const GLchar* DefaultFragmentShaderSrc()
{
    return FragmentShader;
}


}//namespace
