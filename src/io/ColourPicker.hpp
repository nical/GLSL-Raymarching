#ifndef COLOURPICKER_HPP
#define COLOURPICKER_HPP

#include <QColorDialog>
#include "kiwi/core/Node.hpp"
#include "kiwi/core/OutputPort.hpp"
#include "kiwi/core/Data.hpp"

namespace io{

class ColorNodeView;

class ColourPicker : public QColorDialog
{
    Q_OBJECT
    public:
        ColourPicker(kiwi::core::Node * n, ColorNodeView * parentNode );
        ~ColourPicker();

    public slots:
        void SetColour(const QColor &color);
    private:
        ColorNodeView *daddy;
        kiwi::core::Node *colourNode;
    };
}
#endif // COLOURPICKER_HPP
