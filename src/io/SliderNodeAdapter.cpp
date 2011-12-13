#include "SliderNodeAdapter.hpp"

#include "io/SliderNodeView.hpp"

namespace io{

SliderNodeAdapter::SliderNodeAdapter( SliderNodeView * nv )
{
    _sliderNode = nv;
}

void SliderNodeAdapter::setValue(int val)
{
    _sliderNode->updateValue(val);
}


}//namespace
