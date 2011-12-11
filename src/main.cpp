// g++ src/main.cpp -lglut -lGLEW
#include <GL/glew.h>
//#include <GL/gl.h>
#include <stdlib.h>
#include <string.h>
#include "io/Compositor.hpp"
#include "io/Window.hpp"
#include "io/ZoomAdapter.hpp"
#include "io/ConnectAdapter.hpp"
#include "renderer/Renderer.hpp"
#include <assert.h>
#include "kiwi/core/all.hpp"
#include "renderer/Shader.hpp"
#include <QApplication>
#include <QGLFormat>
#include <QtUiTools>
#include <QGridLayout>
#include <QWidget>
#include <QFrame>
#include <assert.h>
#include <QGraphicsView>
#include <QTransform>
#include <QSlider>
#include <QPushButton>

#define WINDOW_TITLE_PREFIX "Raymarcher Shader"
#define WIDTH     600
#define HEIGHT    282
#include <iostream>

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);

void InitKiwi();



int main(int argc, char* argv[])
{

    InitKiwi();
    QApplication raymarcher( argc, argv );

    QGLFormat glFormat;
    glFormat.setVersion( 3, 3 );
    //glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    //glFormat.setAlpha( true );
    //glFormat.setSampleBuffers( true );

    glewExperimental = GL_TRUE;

    io::GLWidget glsection (glFormat);
    //glsection.show();

    QUiLoader loader;

    QFile uiFile("../mainwindow.ui");
    uiFile.open(QFile::ReadOnly);

    QWidget *mainUi = loader.load(&uiFile);
    uiFile.close();

    QFrame* renderFrame = mainUi->findChild<QFrame*>("renderFrame");
    assert(renderFrame);
    QGridLayout renderFrameLayout;
    renderFrameLayout.setMargin(0);
    renderFrameLayout.addWidget( &glsection  );
    renderFrame->setLayout( &renderFrameLayout );

    auto kiwiGraphicsView = mainUi->findChild<QGraphicsView*>("kiwiGraphicsView");
    assert( kiwiGraphicsView );
    kiwiGraphicsView->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    kiwiGraphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    io::Compositor::Create( kiwiGraphicsView );


    mainUi->show();
    //mainUi->resize(600,400);
    renderer::Renderer* _renderer = new renderer::Renderer(WIDTH, HEIGHT);
    glsection.setRenderer(_renderer);

    mainUi->resize(600,400);
    QSlider* zoomSlider = mainUi->findChild<QSlider*>("kiwiZoomSlider");
    assert(zoomSlider);
    io::ZoomAdapter za( kiwiGraphicsView );
    QObject::connect(zoomSlider, SIGNAL(valueChanged(int)),&za, SLOT(zoomChanged(int)) );


    QPushButton* connectButton = mainUi->findChild<QPushButton*>("connectButton");
    assert(connectButton);
    io::ConnectAdapter ca( io::Compositor::Instance().scene() );
    QObject::connect(connectButton,SIGNAL(clicked(void)), &ca, SLOT(buttonClicked(void)) );

    return raymarcher.exec();
}

