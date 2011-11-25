
#include "nodes/RayMarchingNode.hpp"

#include "kiwi/core/all.hpp"
#include "kiwi/core/DynamicNodeUpdater.hpp"

using namespace kiwi;
using namespace kiwi::core;



namespace nodes{

static const NodeTypeInfo * _marcherTypeInfo = 0;
static renderer::Shader * _raymarchingShader = 0;
static renderer::FrameBuffer * _frameBuffer = 0;

typedef DynamicNodeUpdater::DataArray DataArray;
bool RayMarcherNodeUpdate(const DataArray& inputs, const DataArray& outputs)
{
    if(_frameBuffer == 0) return;
    if(_raymarchingShader == 0) return;

    _frameBuffer->bind();

    _raymarchingShader->bind();
    _raymarchingShader->uniformMatrix4fv("viewMatrix", &viewMatrix[0][0] );
    _raymarchingShader->uniform3f("shadowColor", 0.0, 0.3, 0.7 );
    _raymarchingShader->uniform3f("buildingsColor", 1.0, 1.0, 1.0 );
    _raymarchingShader->uniform3f("groundColor", 1.0, 1.0, 1.0 );
    _raymarchingShader->uniform3f("redColor", 1.0, 0.1, 0.1 );
    _raymarchingShader->uniform3f("skyColor", 0.9, 1.0, 1.0 );
    _raymarchingShader->uniform2f("windowSize", window.x, window.y );
    _raymarchingShader->uniform1f("fuffaTime", fuffaTime );
    _raymarchingShader->uniform1f("fovyCoefficient", 1.0 );
    _raymarchingShader->uniform1f("shadowHardness", 7.0f );

    FrameBuffer::unbind();
}


void RegisterRarMarchingNode( Shader * shader )
{
    _raymarchingShader = shader;
    //RegisterShaderNode("RayMarcher", *raymarchingShader );
    auto mat4TypeInfo = kiwi::core::DataTypeManager::TypeOf("Mat4");
    auto uintTypeInfo = kiwi::core::DataTypeManager::TypeOf("GLuint");
    auto vec2TypeInfo = kiwi::core::DataTypeManager::TypeOf("Vec2");
    auto vec3TypeInfo = kiwi::core::DataTypeManager::TypeOf("Vec3");
    auto textureTypeInfo = kiwi::core::DataTypeManager::TypeOf("Texture2D");
    

    NodeLayoutDescriptor raymacherLayout; 
    raymacherLayout.inputs = {
        {"skyColor", vec3TypeInfo, kiwi::READ | OPT },
        {"groundColor", vec3TypeInfo, kiwi::READ | OPT },
        {"buildingsColor", vec3TypeInfo, kiwi::READ | OPT },
        {"shadowColor", vec3TypeInfo, kiwi::READ | OPT },
        {"viewMatrix", mat4TypeInfo, kiwi::READ | OPT },
        {"fuffaTime", uintTypeInfo, kiwi::READ | OPT },
        {"windowSize", vec2TypeInfo, kiwi::READ }
    };
    raymacherLayout.outputs = {
        {"color", textureTypeInfo, kiwi::READ },
        {"normals", textureTypeInfo, kiwi::READ },
        {"godRays", textureTypeInfo, kiwi::READ }
    };

    _marcherTypeInfo = NodeTypeManager::RegisterNode("RayMarcher", raymacherLayout, new DynamicNodeUpdater( &RayMarcherNodeUpdate ) );

}


Node * CreateRayMarchingNode()
{
    return _marcherTypeInfo->newInstance();
}



}//namespace
