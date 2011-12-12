
#include "nodes/RayMarchingNode.hpp"

#include "renderer/Shader.hpp"
#include "renderer/DrawQuad.hpp"
#include "renderer/FrameBuffer.hpp"
#include "utils/CheckGLError.hpp"
#include "io/Window.hpp"
#include "kiwi/core/all.hpp"
#include "kiwi/core/DynamicNodeUpdater.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

using namespace renderer;
using namespace kiwi;
using namespace kiwi::core;
using namespace std;


namespace nodes{

static const NodeTypeInfo * _marcherTypeInfo = 0;
static renderer::Shader * _raymarchingShader = 0;

enum{ FBO_INDEX = 0, TEX0_INDEX = 1, TEX1_INDEX=2 };

typedef DynamicNodeUpdater::DataArray DataArray;
bool RayMarcherNodeUpdate(const DataArray& inputs, const DataArray& outputs)
{
    if(_raymarchingShader == 0)
    {
        std::cout << "Error: shader not set\n";
        return false;
    }

    auto viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
    float time = 1.0;

    (*outputs[FBO_INDEX]->value<FrameBuffer*>())->bind();

    _raymarchingShader->bind();
    
    if ( inputs[0] ){
        CHECKERROR
        _raymarchingShader->uniformVec3("skyColor", *inputs[0]->value<glm::vec3>() );
        CHECKERROR
    } else _raymarchingShader->uniform3f("skyColor", 0.9, 1.0, 1.0 );

    if ( inputs[1] ){
        _raymarchingShader->uniformVec3("buildingsColor", *inputs[1]->value<glm::vec3>() );
    } else _raymarchingShader->uniform3f("buildingsColor", 0.9, 1.0, 1.0 );

    if ( inputs[2] ){
        _raymarchingShader->uniformVec3("groundColor", *inputs[2]->value<glm::vec3>() );
    } else _raymarchingShader->uniform3f("groundColor", 1.0, 1.0, 1.0 );

    if ( inputs[3] ){
        _raymarchingShader->uniformVec3("redColor", *inputs[3]->value<glm::vec3>() );
    } else _raymarchingShader->uniform3f("redColor", 1.0, 0.1, 0.1 );
    
    if ( inputs[4] ){
        _raymarchingShader->uniformVec3("shadowColor", *inputs[4]->value<glm::vec3>() );
    } else _raymarchingShader->uniform3f("shadowColor", 0.0, 0.3, 0.7 );
    
    if ( inputs[5] ){
        _raymarchingShader->uniformMatrix4fv("viewMatrix", &(*inputs[5]->value<glm::mat4>())[0][0] );
    } else _raymarchingShader->uniformMatrix4fv("viewMatrix", &viewMatrix[0][0] );
    
    if ( inputs[6] ){
        _raymarchingShader->uniform1f("time", *inputs[6]->value<GLfloat>() );
    } else _raymarchingShader->uniform1f("time", time);
    
    if ( inputs[7] ){
        _raymarchingShader->uniform1f("fovyCoefficient", *inputs[7]->value<GLfloat>() );
    } else _raymarchingShader->uniform1f("fovyCoefficient", 1.0 );
    
    if ( inputs[8] ){
        _raymarchingShader->uniform1f("shadowHardness", *inputs[8]->value<GLfloat>() );
    } else _raymarchingShader->uniform1f("shadowHardness", 7.0f );

   _raymarchingShader->uniform2f("windowSize", io::GetRenderWindowWidth(), io::GetRenderWindowHeight() );

    renderer::DrawQuad();

    FrameBuffer::unbind();
    return true;
}


void RegisterRayMarchingNode( Shader * shader )
{
    _raymarchingShader = shader;
    //RegisterShaderNode("RayMarcher", *raymarchingShader );
    auto mat4TypeInfo = kiwi::core::DataTypeManager::TypeOf("Mat4");
    auto floatTypeInfo = kiwi::core::DataTypeManager::TypeOf("Float");
    auto vec2TypeInfo = kiwi::core::DataTypeManager::TypeOf("Vec2");
    auto vec3TypeInfo = kiwi::core::DataTypeManager::TypeOf("Vec3");
    auto textureTypeInfo = kiwi::core::DataTypeManager::TypeOf("Texture2D");
    auto frameBufferTypeInfo = kiwi::core::DataTypeManager::TypeOf("FrameBuffer");
    
    assert( mat4TypeInfo );
    assert( vec2TypeInfo );
    assert( vec3TypeInfo );
    assert( textureTypeInfo );
    assert( frameBufferTypeInfo );

    NodeLayoutDescriptor raymacherLayout; 
    raymacherLayout.inputs = {
        {"skyColor", vec3TypeInfo, kiwi::READ | OPT },
        {"buildingsColor", vec3TypeInfo, kiwi::READ | OPT },
        {"groundColor", vec3TypeInfo, kiwi::READ | OPT },
        {"sphereColor", vec3TypeInfo, kiwi::READ | OPT },
        {"shadowColor", vec3TypeInfo, kiwi::READ | OPT },
        {"viewMatrix", mat4TypeInfo, kiwi::READ | OPT },
        {"time", floatTypeInfo, kiwi::READ | OPT },
        {"shadowHardness", floatTypeInfo, kiwi::READ | OPT },
        {"fovyCoefficient", floatTypeInfo, kiwi::READ | OPT }
    };
    raymacherLayout.outputs = {
        {"fbo", frameBufferTypeInfo, kiwi::READ },
        {"color", textureTypeInfo, kiwi::READ   },
        {"normals", textureTypeInfo, kiwi::READ }
    };

    _marcherTypeInfo = NodeTypeManager::RegisterNode("RayMarcher", raymacherLayout, new DynamicNodeUpdater( &RayMarcherNodeUpdate ) );

}


Node * CreateRayMarchingNode()
{
    auto node = _marcherTypeInfo->newInstance();

    assert(node->inputs().size() == 9 );
    assert(node->outputs().size() == 3 );

    assert(node->input(0).dataType() == kiwi::core::DataTypeManager::TypeOf("Vec3") );
    assert(node->input(1).dataType() == kiwi::core::DataTypeManager::TypeOf("Vec3") );
    assert(node->input(2).dataType() == kiwi::core::DataTypeManager::TypeOf("Vec3") );
    assert(node->input(3).dataType() == kiwi::core::DataTypeManager::TypeOf("Vec3") );

    auto fbo = new FrameBuffer(2,400,400);
    *node->output(0).dataAs<FrameBuffer*>() = fbo;
    
    assert( *node->output(0).dataAs<FrameBuffer*>() == fbo );
    
    *node->output(1).dataAs<Texture2D*>() = &fbo->texture(0);
    *node->output(2).dataAs<Texture2D*>() = &fbo->texture(1);

    return node;
}

}//namespace
