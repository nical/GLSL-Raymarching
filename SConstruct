# source files
src = Glob('src/*/*.cpp') + Glob('src/*.cpp') 
# add the 3rd party cpp files
src = src+ Glob('extern/shaderLoader/*.cpp')
# incude directories
includeDirs = ['src/', 'extern/']

# libraries
libraries = ['GLEW','glut','libkiwicpp.a']

# build flags
buildFlags = ['-pg', '-g', '-std=c++0x','-L.']
# build
Program( 'raymarcher', src, CPPFLAGS=buildFlags, CPPPATH = includeDirs, LIBS=libraries )
