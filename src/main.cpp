// g++ src/main.cpp -lglut -lGLEW

#include <stdlib.h>
#include <string.h>
#include "io/Window.hpp"
#include "renderer/DefaultShaders.hpp"
#include "renderer/Renderer.hpp"

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

int main(int argc, char* argv[])
{
    std::cout<<"plop\n";
    renderer::Renderer* _renderer = new renderer::Renderer(WIDTH, HEIGHT);
    std::cout<<"plop\n";
	io::InitWindow(_renderer, WIDTH, HEIGHT, WINDOW_TITLE_PREFIX, argc, argv);
    std::cout<<"plop\n";
	glutMainLoop();

	return (EXIT_SUCCESS);
}

