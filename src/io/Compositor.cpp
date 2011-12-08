#include "Compositor.hpp"

#include <QGraphicsView>
#include <assert.h>

#include "io/NodeView.hpp"
#include "kiwi/core/NodeTypeManager.hpp"

namespace io{


Compositor::Compositor(QGraphicsView * v)
{
    assert(v);
    _view = v;
    _view->setScene( &_scene );


    auto rm_info = kiwi::core::NodeTypeManager::TypeOf("RayMarcher");
    assert(rm_info);
    auto nv1 = new NodeView(QPointF(0,0), rm_info);
    assert(nv1);

    auto vec_info = kiwi::core::NodeTypeManager::TypeOf("Vec3");
    assert(vec_info);
    auto nv2 = new NodeView(QPointF(0,0), vec_info);

    nv1->addToScene( &_scene );
    nv2->addToScene( &_scene );

}

Compositor::~Compositor()
{
    _view->setScene(0);
}



} //namespace
