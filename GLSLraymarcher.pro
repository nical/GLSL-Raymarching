CONFIG += qt
QT += opengl
HEADERS +=  src/io/Window.hpp \
            src/utils/LoadFile.hpp \
            src/renderer/Renderer.hpp \
            src/renderer/Texture.hpp \
            src/renderer/FrameBuffer.hpp \
            src/renderer/Shader.hpp \
            src/utils/CheckGLError.hpp
INCLUDEPATH += ./extern ./src ./extern/kiwi/include
SOURCES +=  src/main.cpp \
            src/io/Window.cpp \
            src/utils/LoadFile.cpp \ 
            src/renderer/Renderer.cpp \
            src/renderer/Shader.cpp \
            src/renderer/FrameBuffer.cpp \
            src/utils/CheckGLError.cpp \
            src/KiwiInit.cpp

QMAKE_CXXFLAGS += -std=c++0x -pg -g
            
LIBS += -lGLEW ./extern/kiwi/libkiwicpp.a
DESTDIR = ./bin/
TARGET = GLSLraymarcher
