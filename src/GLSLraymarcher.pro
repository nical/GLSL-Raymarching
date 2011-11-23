CONFIG += qt
QT += opengl
HEADERS +=  io/Window.hpp \
            utils/LoadFile.hpp \
            renderer/DefaultShaders.hpp \
            renderer/ShaderProgram.hpp \
            renderer/Renderer.hpp
INCLUDEPATH += ../extern
SOURCES +=  main.cpp \
            io/Window.cpp \
            utils/LoadFile.cpp \ 
            renderer/DefaultShaders.cpp \
            renderer/Renderer.cpp \
            ../extern/shaderLoader/shader.cpp
LIBS += -lGLEW
DESTDIR = ../bin/
TARGET = GLSLraymarcher
