#include "io/ColourPicker.hpp"

#include "kiwi/core/Node.hpp"
#include "kiwi/core/OutputPort.hpp"
#include <QColor>
#include <QGraphicsSceneMouseEvent>
#include <QColorDialog>
#include "glm/glm.hpp"
#include "io/ColorNodeView.hpp"

namespace io{

    ColourPicker::ColourPicker( kiwi::core::Node * n, ColorNodeView * parentNode )
    {
        colourNode = n;
        daddy = parentNode;
        QColor colour(colourNode->output().dataAs<glm::vec3>()->r*255.0, colourNode->output().dataAs<glm::vec3>()->g*255.0, colourNode->output().dataAs<glm::vec3>()->b*255.0);
        this->setCurrentColor(colour);
        assert(QObject::connect(this, SIGNAL(currentColorChanged(QColor)), SLOT(SetColour(QColor))));
    }

    ColourPicker::~ColourPicker(){

    }


    void ColourPicker::SetColour(const QColor &colour){
        daddy->UpdateGraphics();
        *colourNode->output().dataAs<glm::vec3>() = glm::vec3(colour.red()/255.0, colour.green()/255.0, colour.blue()/255.0);
    }

}
