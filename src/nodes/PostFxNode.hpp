
#pragma once
#ifndef NODES_POSTFXNODE_HPP
#define NODES_POSTFXNODE_HPP

#include <string>
#include "kiwi/core/NodeUpdater.hpp"

namespace kiwi{ namespace core{ class Node; }}

namespace renderer{ class Shader; }

namespace nodes {


class ShaderNodeUpdater : public kiwi::core::NodeUpdater
{
public:

    ShaderNodeUpdater( renderer::Shader* shader )
    : _shader(shader) { }

    bool update(const kiwi::core::Node& n);

private:
    renderer::Shader * _shader;
};



void RegisterPostFxNode( renderer::Shader* shader, const std::string& name );
kiwi::core::Node * CreatePostFxNode( const std::string& name );

void RegisterScreenNode();
kiwi::core::Node * CreateScreenNode();


} //namespace


#endif
