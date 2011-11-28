
#include "nodes/TimeNode.hpp"
#include "kiwi/core/all.hpp"
#include "kiwi/core/DynamicNodeUpdater.hpp"
#include <assert.h>
#include <GL/glew.h>

using namespace kiwi;
using namespace kiwi::core;
//using namespace kiwi::processing;

namespace nodes{

static const NodeTypeInfo * _nodeTypeInfo = 0;

typedef DynamicNodeUpdater::DataArray DataArray;
bool TimerNodeUpdate(const DataArray& inputs, const DataArray& outputs)
{
    *outputs[0]->value<GLfloat>() += 1;
}

void RegisterTimeNode()
{
    auto floatTypeInfo = DataTypeManager::TypeOf("Float");
    assert( floatTypeInfo != 0 );
    kiwi::core::NodeLayoutDescriptor layout; 
    layout.outputs = {
        {"time", floatTypeInfo, kiwi::READ }
    };

    _nodeTypeInfo = NodeTypeManager::RegisterNode(
            "Timer", layout, new DynamicNodeUpdater(&TimerNodeUpdate));
    assert( _nodeTypeInfo != 0 );
}

Node * CreateTimeNode()
{
    assert( _nodeTypeInfo != 0 );

    return _nodeTypeInfo->newInstance();
}



}//namespace
