
#include "nodes/RayMarchingNode.hpp"

#include "renderer/Shader.hpp"
#include "renderer/DrawQuad.hpp"
#include "renderer/FrameBuffer.hpp"

#include "kiwi/core/all.hpp"
#include "kiwi/core/DynamicNodeUpdater.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


using namespace renderer;
using namespace kiwi;
using namespace kiwi::core;



namespace nodes{

static const NodeTypeInfo * _marcherTypeInfo = 0;
static renderer::Shader * _raymarchingShader = 0;
static renderer::FrameBuffer * _frameBuffer = 0;

typedef DynamicNodeUpdater::DataArray DataArray;
bool RayMarcherNodeUpdate(const DataArray& inputs, const DataArray& outputs)
{
    if(_frameBuffer == 0) return false;
    if(_raymarchingShader == 0) return false;

    auto viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
    float time = 1.0;

    _frameBuffer->bind();

    _raymarchingShader->bind();
    _raymarchingShader->uniformMatrix4fv("viewMatrix", &viewMatrix[0][0] );
    _raymarchingShader->uniform3f("shadowColor", 0.0, 0.3, 0.7 );
    _raymarchingShader->uniform3f("buildingsColor", 1.0, 1.0, 1.0 );
    _raymarchingShader->uniform3f("groundColor", 1.0, 1.0, 1.0 );
    _raymarchingShader->uniform3f("redColor", 1.0, 0.1, 0.1 );
    _raymarchingShader->uniform3f("skyColor", 0.9, 1.0, 1.0 );
    _raymarchingShader->uniform2f("windowSize", 400, 400 );
    _raymarchingShader->uniform1f("fuffaTime", time);
    _raymarchingShader->uniform1f("fovyCoefficient", 1.0 );
    _raymarchingShader->uniform1f("shadowHardness", 7.0f );

    FrameBuffer::unbind();
    return true;
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
    auto frameBufferTypeInfo = kiwi::core::DataTypeManager::TypeOf("FrameBuffer");
    

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
        {"fbo", frameBufferTypeInfo, kiwi::READ },
        {"color", textureTypeInfo, kiwi::READ   },
        {"normals", textureTypeInfo, kiwi::READ }
    };

    _marcherTypeInfo = NodeTypeManager::RegisterNode("RayMarcher", raymacherLayout, new DynamicNodeUpdater( &RayMarcherNodeUpdate ) );

}


Node * CreateRayMarchingNode()
{
    auto node = _marcherTypeInfo->newInstance();
    auto fbo = new FrameBuffer(2,400,400);
    *node->output(0).dataAs<FrameBuffer*>() = fbo;
    *node->output(1).dataAs<Texture2D*>() = &fbo->texture(0);
    *node->output(2).dataAs<Texture2D*>() = &fbo->texture(1);

    return node;
}



}//namespace
