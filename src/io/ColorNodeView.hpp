#pragma once
#ifndef COLORNODEVIEW_HPP
#define COLORNODEVIEW_HPP

#include "io/NodeView.hpp"

namespace io{

class ColorNodeView : public NodeView
{
public:
    explicit ColorNodeView(const QPointF& position, kiwi::core::Node * n );
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString nodeName() const;
};




}//namespace

#endif // COLORNODEVIEW_HPP
