
#pragma once
#ifndef NODES_POSTFXNODE_HPP
#define NODES_POSTFXNODE_HPP

namespace kiwi{ namespace core{ class Node; }}

namespace renderer{ class Shader; }

namespace nodes {

void RegisterPostFxNode( renderer::Shader* shader );
kiwi::core::Node * CreatePostFxNode();

} //namespace


#endif
