
#include "nodes/FloatMathNodes.hpp"

#include "kiwi/core/Node.hpp"
#include "kiwi/core/InputPort.hpp"
#include "kiwi/core/DynamicNodeUpdater.hpp"
#include "kiwi/core/NodeTypeManager.hpp"
#include "kiwi/core/DataTypeManager.hpp"

#include "io/NodeView.hpp"
#include "io/Compositor.hpp"

#include <math.h>

using namespace kiwi::core;

namespace nodes{

typedef DynamicNodeUpdater::DataArray DataArray;

bool ApplySin(const DataArray& inputs, const DataArray& outputs)
{
    *outputs[0]->value<float>() = sin(*inputs[0]->value<float>());
}

bool ApplyCos(const DataArray& inputs, const DataArray& outputs)
{
    *outputs[0]->value<float>() = cos(*inputs[0]->value<float>());
}

bool ApplyClamp(const DataArray& inputs, const DataArray& outputs)
{
    float val = *inputs[0]->value<float>();
    if(val > 1.0 ) *outputs[0]->value<float>() = 1.0;
    else if(val < 0.0 ) *outputs[0]->value<float>() = 0.0;
    else *outputs[0]->value<float>();
}

bool ApplyMult(const DataArray& inputs, const DataArray& outputs)
{
    *outputs[0]->value<float>() = (*inputs[0]->value<float>()) * (*inputs[1]->value<float>());
}

bool ApplyDiv(const DataArray& inputs, const DataArray& outputs)
{
    *outputs[0]->value<float>() = (*inputs[0]->value<float>()) / (*inputs[1]->value<float>());
}

bool ApplyAdd(const DataArray& inputs, const DataArray& outputs)
{
    *outputs[0]->value<float>() = (*inputs[0]->value<float>()) + (*inputs[1]->value<float>());
}

bool ApplySub(const DataArray& inputs, const DataArray& outputs)
{
    *outputs[0]->value<float>() = (*inputs[0]->value<float>()) - (*inputs[1]->value<float>());
}


#define FuncForMenu( create, func ) void func( const QPointF& pos ){ \
    std::cerr << "add node\n"; \
io::Compositor::Instance().add( new io::NodeView(pos, create() ) ); \
}

#define CompositorAdd( func, name ) io::Compositor::Instance().addNodeToMenu( name, func );

FuncForMenu( CreateSinNode, AddSinToMenu )
FuncForMenu( CreateCosNode, AddCosToMenu )
FuncForMenu( CreateClampNode, AddClampToMenu )
FuncForMenu( CreateAddNode, AddAddToMenu )
FuncForMenu( CreateSubstractNode, AddSubstractToMenu )
FuncForMenu( CreateMultiplyNode, AddMultiplyToMenu )
FuncForMenu( CreateDivideNode, AddDivideToMenu )

void RegisterFloatMathNodes()
{
    const DataTypeInfo * floatTypeInfo = DataTypeManager::TypeOf("Float");

    assert(floatTypeInfo);

    NodeLayoutDescriptor layout_1_1;
    layout_1_1.inputs = {
        { "in", floatTypeInfo, kiwi::READ }
    };
    layout_1_1.outputs = {
        { "out", floatTypeInfo, kiwi::READ }
    };

    NodeLayoutDescriptor layout_2_1;
    layout_2_1.inputs = {
        { "A", floatTypeInfo, kiwi::READ },
        { "B", floatTypeInfo, kiwi::READ }
    };
    layout_2_1.outputs = {
        { "out", floatTypeInfo, kiwi::READ }
    };
    
    NodeLayoutDescriptor layout_1_2;
    
    NodeTypeManager::RegisterNode("Sin", layout_1_1, new DynamicNodeUpdater( &ApplySin ) );
    NodeTypeManager::RegisterNode("Cos", layout_1_1, new DynamicNodeUpdater( &ApplyCos ) );
    NodeTypeManager::RegisterNode("Clamp", layout_1_1, new DynamicNodeUpdater( &ApplyClamp ) );
    NodeTypeManager::RegisterNode("Multiply", layout_2_1, new DynamicNodeUpdater( &ApplyMult ) );
    NodeTypeManager::RegisterNode("Divide", layout_2_1, new DynamicNodeUpdater( &ApplyDiv ) );
    NodeTypeManager::RegisterNode("Add", layout_2_1, new DynamicNodeUpdater( &ApplyAdd ) );
    NodeTypeManager::RegisterNode("Substract", layout_2_1, new DynamicNodeUpdater( &ApplySub ) );

    CompositorAdd( &AddSinToMenu, "Sin" );
    CompositorAdd( &AddCosToMenu, "Cos" );
    CompositorAdd( &AddAddToMenu, "Add" );
    CompositorAdd( &AddSubstractToMenu, "Substract" );
    CompositorAdd( &AddMultiplyToMenu, "Multiply" );
    CompositorAdd( &AddDivideToMenu, "Divide" );
    CompositorAdd( &AddClampToMenu, "Clamp" );
}

kiwi::core::Node * CreateSinNode()
{
    return kiwi::core::NodeTypeManager::Create("Sin");
}

kiwi::core::Node * CreateCosNode()
{
    return kiwi::core::NodeTypeManager::Create("Cos");
}

kiwi::core::Node * CreateClampNode()
{
    return kiwi::core::NodeTypeManager::Create("Clamp");
}

kiwi::core::Node * CreateMultiplyNode()
{
    return kiwi::core::NodeTypeManager::Create("Multiply");
}

kiwi::core::Node * CreateDivideNode()
{
    return kiwi::core::NodeTypeManager::Create("Divide");
}

kiwi::core::Node * CreateAddNode()
{
    return kiwi::core::NodeTypeManager::Create("Add");
}

kiwi::core::Node * CreateSubstractNode()
{
    return kiwi::core::NodeTypeManager::Create("Substract");
}


}//namespace
