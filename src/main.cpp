// g++ src/main.cpp -lglut -lGLEW

#include <stdlib.h>
#include <string.h>
#include "io/Window.hpp"
#include "renderer/Renderer.hpp"
#include <assert.h>
#include "kiwi/core/all.hpp"
#include "renderer/Shader.hpp"
#include <GL/glew.h>
#include <GL/freeglut.h>
#define WINDOW_TITLE_PREFIX "Raymarcher Shader"
#define WIDTH     640
#define HEIGHT    480
#include <iostream>

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);

KIWI_DECLARE_CONTAINER(int,"Int");

void InitKiwi();

int main(int argc, char* argv[])
{

    InitKiwi();

    glewExperimental = GL_TRUE;
    renderer::Renderer* _renderer = new renderer::Renderer(WIDTH, HEIGHT);
	io::InitWindow(_renderer, WIDTH, HEIGHT, WINDOW_TITLE_PREFIX, argc, argv);
	glutMainLoop();

	return (EXIT_SUCCESS);
}

