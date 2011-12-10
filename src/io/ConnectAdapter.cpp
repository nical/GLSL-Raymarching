#include "ConnectAdapter.hpp"
#include "io/NodeView.hpp"

#include <QGraphicsScene>
#include <iostream>
#include "io/PortView.hpp"
#include "io/Compositor.hpp"
#include "kiwi/core/Node.hpp"
#include "kiwi/core/InputPort.hpp"

namespace io{

void ConnectAdapter::buttonClicked()
{
    std::cerr << "adapter: Disconnect\n";
    auto sp = _scene->selectedItems();

    for( auto it = sp.begin(); it != sp.end(); ++it )
    {
        PortView* pv = dynamic_cast<PortView*>( *it );
        if (!pv) continue;

        if ( pv->isInput() )
        {
            kiwi::core::Node * n = pv->nodeView()->node();
            int i = pv->nodeView()->indexOf(pv, PortView::INPUT);
            n->input(i).disconnectAll();
            //pv->disconnect();
        }
    }
}

}//namespace
