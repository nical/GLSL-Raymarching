// g++ src/main.cpp -lglut -lGLEW

#include <stdlib.h>
#include <string.h>
#include "io/Window.hpp"
#include "renderer/DefaultShaders.hpp"
#include "renderer/Renderer.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#define WINDOW_TITLE_PREFIX "Chapter 1"
#define WIDTH     800
#define HEIGHT    600

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);

int main(int argc, char* argv[])
{
  renderer::Renderer* _renderer = new renderer::Renderer;

	io::InitWindow(_renderer, WIDTH, HEIGHT, WINDOW_TITLE_PREFIX, argc, argv);

	glutMainLoop();

	return (EXIT_SUCCESS);
}

