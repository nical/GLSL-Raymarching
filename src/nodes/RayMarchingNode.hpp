
#pragma once
#ifndef NODES_RAYMARCHINGNODE_HPP
#define NODES_RAYMARCHINGNODE_HPP


namespace kiwi{ namespace core{ class Node; }}

namespace renderer{ class Shader; }

namespace nodes {

void RegisterRayMarchingNode( renderer::Shader* shader );
kiwi::core::Node * CreateRayMarchingNode();

} //namespace


#endif
