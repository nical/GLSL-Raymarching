#ifndef CREATENODEACTION_HPP
#define CREATENODEACTION_HPP

#include <QAction>

class QString;

namespace io{

class CreateNodeAction : public QAction
{
    Q_OBJECT
public:
    typedef void (*funcptr)( const QPointF& );
    CreateNodeAction( const QString& str, funcptr f );
public slots:
    void createNode();
private:
    funcptr _function;
};

}//namespace

#endif // CREATENODEACTION_HPP
