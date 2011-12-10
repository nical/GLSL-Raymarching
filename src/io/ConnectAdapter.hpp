#ifndef CONNECTADAPTER_HPP
#define CONNECTADAPTER_HPP

#include <QObject>

class QGraphicsScene;

namespace io{

class ConnectAdapter : public QObject
{
    Q_OBJECT
public:
    explicit ConnectAdapter( QGraphicsScene * s )
        : _scene(s) { }

signals:

public slots:
    void connectButtonClecked();
private:
    QGraphicsScene * _scene;
};

}//namespace

#endif // CONNECTADAPTER_HPP
