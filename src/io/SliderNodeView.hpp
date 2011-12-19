#ifndef SLIDERNODEVIEW_HPP
#define SLIDERNODEVIEW_HPP

#include "io/NodeView.hpp"

class QPointF;

class QSlider;
class QGraphicsScene;

namespace io{

class SliderNodeAdapter;


void AddSliderMenu();

class SliderNodeView : public io::NodeView
{
public:
    explicit SliderNodeView(const QPointF& position, float min, float max);

    void mouseMoveEvent( QGraphicsSceneMouseEvent* event );

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);



    virtual QString nodeName() const;

    void updateValue(int value);

    void addToScene(QGraphicsScene * s);
private:
    float _min;
    float _max;
    QSlider * _slider;
    SliderNodeAdapter * _adapter;
};

}//namespace

#endif // SLIDERNODEVIEW_HPP
