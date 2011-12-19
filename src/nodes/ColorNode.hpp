
#pragma once
#ifndef NODES_COLORNODE_HPP
#define NODES_COLORNODE_HPP

#include "glm/glm.hpp"

namespace kiwi{ namespace core{ class Node; }}

namespace nodes{

void RegisterColorNode();
kiwi::core::Node * CreateColorNode( glm::vec3 color );
kiwi::core::Node * CreateColorNode();

}//namespace


#endif
