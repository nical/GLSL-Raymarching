
#include "nodes/PostFxNode.hpp"

#include "renderer/Shader.hpp"
#include "renderer/DrawQuad.hpp"
#include "renderer/FrameBuffer.hpp"
#include "utils/CheckGLError.hpp"
#include "utils/LoadFile.hpp"
#include "io/Window.hpp"

#include "kiwi/core/NodeTypeManager.hpp"
#include "kiwi/core/DataTypeManager.hpp"
#include "kiwi/core/Data.hpp"
#include "kiwi/core/Node.hpp"
#include "kiwi/core/InputPort.hpp"
#include "kiwi/core/OutputPort.hpp"
#include "kiwi/core/DynamicNodeUpdater.hpp"

#include <iostream>

using namespace renderer;
using namespace kiwi::core;

static const DataTypeInfo * textureTypeInfo = 0;
static const DataTypeInfo * vec3TypeInfo = 0;
static const DataTypeInfo * floatTypeInfo = 0;

namespace nodes{


static GLuint selectTexture(int i)
{
    switch(i)
    {
        case 0 : return GL_TEXTURE0;
        case 1 : return GL_TEXTURE1;
        case 2 : return GL_TEXTURE2;
        case 3 : return GL_TEXTURE3;
        case 4 : return GL_TEXTURE4;
        case 5 : return GL_TEXTURE5;
        case 6 : return GL_TEXTURE6;
        case 7 : return GL_TEXTURE7;
    }
    assert(false);
    return GL_TEXTURE0;
}


bool ShaderNodeUpdater::update(const Node& n)
{
    CHECKERROR
    _shader->bind();
    CHECKERROR
    if(_shader->hasLocation("windowSize"))
        _shader->uniform2f("windowSize", io::GetRenderWindowWidth(), io::GetRenderWindowHeight() );
    CHECKERROR
    int nbTex = 0;

    for(int i = 0; i < n.inputs().size(); ++i)
    {
        if( ! n.input(i).isConnected() )
        {
            std::cerr << "ShadeNodeUpdate::update error! disconnected input\n";
            return false;
        }
        if( n.input(i).dataType() == textureTypeInfo )
        {
            CHECKERROR
            _shader->uniform1i(n.input(i).name(), nbTex);
            glActiveTexture( selectTexture(nbTex) );
            (*n.input(i).dataAs<Texture2D*>())->bind();
            //std::cerr << "uniform texture " << n.input(i).name() << " -> "<< nbTex << std::endl;
            ++nbTex;
            CHECKERROR
        }
        else if ( n.input(i).dataType() == vec3TypeInfo )
        {
            CHECKERROR
            _shader->uniformVec3(n.input(i).name(), *n.input(i).dataAs<glm::vec3>() );
            //std::cerr << "uniform vec3" << n.input(i).name() << std::endl;
            CHECKERROR
        }
        else if ( n.input(i).dataType() == floatTypeInfo )
        {
            CHECKERROR
            //std::cerr << "uniform float " << n.input(i).name() << std::endl;
            _shader->uniform1f(n.input(i).name(), *n.input(i).dataAs<float>() );
            CHECKERROR
        }
    }
    CHECKERROR
    (*n.output(0).dataAs<FrameBuffer*>())->bind();
    CHECKERROR
    renderer::DrawQuad();
    CHECKERROR
    _shader->unbind();

    return true;
}


void RegisterPostFxNode( renderer::Shader* shader, const std::string& name )
{
    auto fboTypeInfo = DataTypeManager::TypeOf("FrameBuffer");
    textureTypeInfo = DataTypeManager::TypeOf("Texture2D");
    vec3TypeInfo = DataTypeManager::TypeOf("Vec3");
    floatTypeInfo = DataTypeManager::TypeOf("Float");
    assert(textureTypeInfo);
    assert(vec3TypeInfo);
    assert(floatTypeInfo);

    NodeLayoutDescriptor layout;
    for(auto it = shader->locations_begin(); it != shader->locations_end(); ++it)
    {
        const DataTypeInfo * info = 0;
        switch(it->second.type)
        {
            case (Shader::UNIFORM | Shader::TEXTURE2D) :
            {
                info = textureTypeInfo;
                break;
            }
            case (Shader::UNIFORM | Shader::FLOAT3) :
            {
                info = vec3TypeInfo;
                break;
            }
            case (Shader::UNIFORM | Shader::FLOAT) :
            {
                info = floatTypeInfo;
                break;
            }
            default:
            {
                std::cerr << "ignored location " << it->first << std::endl;
            }
        }

        if ( info )
            layout.inputs.push_back(InputPortDescriptor(it->first, info, kiwi::READ ));
    }
    layout.outputs = {
        {"frame buffer", fboTypeInfo, kiwi::READ },
        {"color", textureTypeInfo, kiwi::READ }
    };
    NodeTypeManager::RegisterNode(name, layout, new ShaderNodeUpdater( shader ) );
}


kiwi::core::Node * CreatePostFxNode(const std::string& name)
{
    auto node = kiwi::core::NodeTypeManager::TypeOf(name)->newInstance();

    // create output resources (for render to texture)
    // Port 0 : frame buffer
    // Port 1 : texture (attached to the fbo)

    auto fbo = new FrameBuffer(2,400,400);
    *node->output(0).dataAs<FrameBuffer*>() = fbo;

    assert( *node->output(0).dataAs<FrameBuffer*>() == fbo );

    *node->output(1).dataAs<Texture2D*>() = &fbo->texture(0);

    return node;
}

static renderer::Shader * s_renderToScreenShader = 0;

typedef DynamicNodeUpdater::DataArray DataArray;
bool RenderToScreen(const DataArray& inputs, const DataArray&)
{
    assert(s_renderToScreenShader);
    s_renderToScreenShader->bind();

    auto inputTex = *inputs[0]->value<Texture2D*>();
    assert(inputTex);

    s_renderToScreenShader->uniform1i("colourTexture",0);
    s_renderToScreenShader->uniform2f("windowSize", io::GetRenderWindowWidth(), io::GetRenderWindowHeight());

    glActiveTexture(GL_TEXTURE0);
    inputTex->bind();

    renderer::DrawQuad();
}

void RegisterScreenNode()
{
    assert(textureTypeInfo);

    s_renderToScreenShader = new renderer::Shader;
    std::string vs;
    std::string fs;
    assert( utils::LoadTextFile("shaders/ToScreen.vert", vs) );
    assert( utils::LoadTextFile("shaders/ToScreen.frag", fs) );

    renderer::Shader::LocationMap locations = {
        {"colourTexture",   { Shader::UNIFORM | Shader::TEXTURE2D} },
        {"windowSize",   { Shader::UNIFORM | Shader::FLOAT2} }
    };
    s_renderToScreenShader->build(vs,fs,locations);

    NodeLayoutDescriptor layout;
    layout.inputs = {
        { "color", textureTypeInfo, kiwi::READ }
    };

    NodeTypeManager::RegisterNode("Screen", layout, new DynamicNodeUpdater( &RenderToScreen ) );
}

kiwi::core::Node * CreateScreenNode()
{
    return NodeTypeManager::Create("Screen");
}

}//namespace
