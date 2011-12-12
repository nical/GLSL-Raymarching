#pragma once
#ifndef COLORNODEVIEW_HPP
#define COLORNODEVIEW_HPP

#include "io/NodeView.hpp"

namespace io{

class ColourPicker;

class ColorNodeView : public NodeView
{
public:
    explicit ColorNodeView(const QPointF& position, kiwi::core::Node * n );
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString nodeName() const;
    void UpdateGraphics();
protected:
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
private:
    ColourPicker *colourPicker;
};




}//namespace

#endif // COLORNODEVIEW_HPP
