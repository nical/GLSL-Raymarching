#include "CreateNodeAction.hpp"
#include "io/Window.hpp"
#include <QPointF>
#include <iostream>
#include <QObject>

namespace io{

CreateNodeAction::CreateNodeAction( const QString& str, funcptr f )
:QAction(str, 0)
{
    _function = f;
    QObject::connect(this, SIGNAL(triggered()),this, SLOT(createNode()));
}

void CreateNodeAction::createNode()
{
    std::cerr << "CreateNode\n";
    if(_function) _function( QPointF((float)io::GetCursorX(), (float)io::GetCursorY()) );
}


}//namespace
