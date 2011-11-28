#include "nodes/ColorNode.hpp"

#include "kiwi/core/all.hpp"
#include <assert.h>

namespace nodes{


kiwi::core::Node * CreateColorNode( glm::vec3 color )
{
    auto n = kiwi::core::NodeTypeManager::Create("Vec3");
    assert(n);
    *n->output().dataAs<glm::vec3>() = color;
    return n;
}

kiwi::core::Node * CreateColorNode()
{
    auto n = kiwi::core::NodeTypeManager::Create("Vec3");
    assert(n);
    *n->output().dataAs<glm::vec3>() = glm::vec3(0.0,0.0,0.0);
    return n;
}




}//namespace
