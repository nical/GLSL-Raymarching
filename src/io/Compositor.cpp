#include "Compositor.hpp"

#include <QGraphicsView>
#include <assert.h>

#include "io/NodeView.hpp"
#include "io/PortView.hpp"
#include "kiwi/core/NodeTypeManager.hpp"

namespace io{

static Compositor * s_instance = 0;

void Compositor::Create( QGraphicsView * v )
{
    assert(s_instance == 0);
    s_instance = new Compositor(v);
}

Compositor& Compositor::Instance()
{
    assert( s_instance != 0 );
    return *s_instance;
}

Compositor::Compositor(QGraphicsView * v)
{
    assert(v);
    _view = v;
    _view->setScene( &_scene );
}

Compositor::~Compositor()
{
    _view->setScene(0);
}

void Compositor::add(NodeView * nv)
{
    nv->addToScene( &_scene );
}


} //namespace
