// g++ src/main.cpp -lglut -lGLEW
#include <GL/glew.h>
//#include <GL/gl.h>
#include <stdlib.h>
#include <string.h>
#include "io/Window.hpp"
#include "renderer/Renderer.hpp"
#include <assert.h>
#include "kiwi/core/all.hpp"
#include "renderer/Shader.hpp"
#include <QApplication>
#include <QGLFormat>

#define WINDOW_TITLE_PREFIX "Raymarcher Shader"
#define WIDTH     400
#define HEIGHT    400
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
    glFormat.setAlpha( true );
    glFormat.setSampleBuffers( true );


    io::GraphicsView view;
    view.setViewport(new QGLWidget( QGLFormat(QGL::SampleBuffers)));
    view.setViewportUpdateMode( QGraphicsView::FullViewportUpdate);
    view.setScene(new io::GraphicsScene);
    view.show();
    /*
    // Specify an OpenGL 3.3 format using the Core profile.
    // That is, no old-school fixed pipeline functionality
    QGLFormat glFormat;
    glFormat.setVersion( 3, 3 );
    //glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setAlpha( true );
    glFormat.setSampleBuffers( true );

    glewExperimental = GL_TRUE;
    renderer::Renderer* _renderer = new renderer::Renderer(WIDTH, HEIGHT);
    io::GLWidget glsection (glFormat);
    glsection.show();
    glsection.setRenderer(_renderer);
    */
	return (raymarcher.exec());
}

