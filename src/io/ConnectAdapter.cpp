#include "ConnectAdapter.hpp"
#include "io/NodeView.hpp"

#include <QGraphicsScene>
#include <iostream>
#include "io/PortView.hpp"
#include "io/Compositor.hpp"


namespace io{

void ConnectAdapter::connectButtonClecked()
{
    auto sp = _scene->selectedItems();
    PortView * input = 0;
    PortView * output = 0;
    for( auto it = sp.begin(); it != sp.end(); ++it )
    {
        PortView* pv = dynamic_cast<PortView*>( *it );
        if (!pv) continue;

        if ( pv->isInput() )
            input = pv;
        else if ( pv->isOutput() )
            output = pv;

        if( input && output ) break;
    }

    if( input && output )
        Compositor::Instance().tryConnect( output, input );
    else
        std::cerr << "wrong connection request\n";
}

}//namespace
