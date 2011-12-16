
#include "nodes/ColorMix.hpp"

#include "kiwi/core/NodeTypeManager.hpp"
#include "kiwi/core/DataTypeManager.hpp"
#include "kiwi/core/DynamicNodeUpdater.hpp"
#include "glm/glm.hpp"

namespace nodes{

typedef kiwi::core::DynamicNodeUpdater::DataArray DataArray;

bool ApplyColorMix(const DataArray& inputs, const DataArray& outputs)
{
    *outputs[0]->value<glm::vec3>() = glm::mix(
        *inputs[0]->value<glm::vec3>(),
        *inputs[1]->value<glm::vec3>(),
        *inputs[2]->value<float>()
    );
    return true;
}

void RegisterColorMixNode()
{
    auto vec3TypeInfo  = kiwi::core::DataTypeManager::TypeOf("Vec3");
    auto floatTypeInfo = kiwi::core::DataTypeManager::TypeOf("Float");
    kiwi::core::NodeLayoutDescriptor layout;
    layout.inputs = {
        { "color#1", vec3TypeInfo, kiwi::READ },
        { "color#2", vec3TypeInfo, kiwi::READ },
        { "t"      , floatTypeInfo,kiwi::READ }
    };
    layout.outputs = {
        { "out", vec3TypeInfo, kiwi::READ }
    };
    kiwi::core::NodeTypeManager::RegisterNode("ColorMix", layout, new kiwi::core::DynamicNodeUpdater( &ApplyColorMix ) );
}

kiwi::core::Node * CreateColorMixNode()
{
    return kiwi::core::NodeTypeManager::Create("ColorMix");
}

}//namespace
