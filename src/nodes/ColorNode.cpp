#include "nodes/ColorNode.hpp"

#include "kiwi/core/all.hpp"
#include <assert.h>

#include "io/ColorNodeView.hpp"
#include "io/Compositor.hpp"

namespace nodes{

void AddColorNodeToCompositor( const QPointF& p )
{
    io::Compositor::Instance().add( new io::ColorNodeView(p, CreateColorNode() ) );
}

void RegisterColorNode()
{
    io::Compositor::Instance().addNodeToMenu( "Color", &AddColorNodeToCompositor );
}

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
