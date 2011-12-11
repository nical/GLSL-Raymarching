
#include "nodes/PostFxNode.hpp"

#include "renderer/Shader.hpp"
#include "renderer/DrawQuad.hpp"
#include "renderer/FrameBuffer.hpp"
#include "utils/CheckGLError.hpp"

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

static const DataTypeInfo * textureTypeInfo;
static const DataTypeInfo * vec3TypeInfo;
static const DataTypeInfo * floatTypeInfo;

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

    this->_shader->bind();

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
            _shader->uniform1i("colourTexture", nbTex);
            glActiveTexture( selectTexture(i) );
            (*n.input(i).dataAs<Texture2D*>())->bind();
        }
        else if ( n.input(i).dataType() == vec3TypeInfo )
        {
            _shader->uniformVec3(n.input(i).name(), *n.input(i).dataAs<glm::vec3>() );
        }
        else if ( n.input(i).dataType() == floatTypeInfo )
        {
            _shader->uniform1f(n.input(i).name(), *n.input(i).dataAs<float>() );
        }
    }

    renderer::DrawQuad();

    _shader->unbind();

    return true;
}


void RegisterPostFxNode( renderer::Shader* shader, const std::string& name )
{
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
        {"color", textureTypeInfo, kiwi::READ }
    };
    NodeTypeManager::RegisterNode(name, layout, new ShaderNodeUpdater( shader ) );
}


kiwi::core::Node * CreatePostFxNode(const std::string& name)
{
    return kiwi::core::NodeTypeManager::TypeOf(name)->newInstance();
}

kiwi::core::Node * CreateScreenNode()
{

}



}//namespace
