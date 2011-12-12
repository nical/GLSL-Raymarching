
#pragma once
#ifndef NODES_FLOATMATHNODES_HPP
#define NODES_FLOATMATHNODES_HPP

namespace kiwi{ namespace core{ class Node; }}

namespace nodes{


void RegisterFloatMathNodes();

kiwi::core::Node * CreateSinNode();
kiwi::core::Node * CreateCosNode();
kiwi::core::Node * CreateClampNode();
kiwi::core::Node * CreateMultiplyNode();
kiwi::core::Node * CreateDivideNode();
kiwi::core::Node * CreateAddNode();
kiwi::core::Node * CreateSubstractNode();



}//namespace


#endif
