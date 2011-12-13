#ifndef SLIDERNODEADAPTER_HPP
#define SLIDERNODEADAPTER_HPP

#include <QObject>

namespace io{

class SliderNodeView;

class SliderNodeAdapter : public QObject
{
    Q_OBJECT
public:
    explicit SliderNodeAdapter(SliderNodeView * nv );

signals:

public slots:
    void setValue(int val);

private:
    SliderNodeView * _sliderNode;
};

}//namespace

#endif // SLIDERNODEADAPTER_HPP
