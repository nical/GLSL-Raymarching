#ifndef ZOOMADAPTER_HPP
#define ZOOMADAPTER_HPP

#include <QObject>

class QGraphicsView;


namespace io{

class ZoomAdapter : public QObject
{
    Q_OBJECT;
public:
    ZoomAdapter( QGraphicsView * gv ) : _gfxView(gv){}
    ~ZoomAdapter();
    QGraphicsView * gfxView() const
    {
        return _gfxView;
    }

public slots:
    void zoomChanged( int value );
private:
    QGraphicsView * _gfxView;
};

}

#endif // ZOOMADAPTER_HPP
