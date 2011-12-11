#include "ZoomAdapter.hpp"

#include <QGraphicsView>


namespace io{

ZoomAdapter::~ZoomAdapter()
{

}

void ZoomAdapter::zoomChanged( int val )
{
    _gfxView->setTransform( QTransform::fromScale( ((float)val) * 0.01, ((float)val) * 0.01 ) );
}


}//namespace
