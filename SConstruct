# source files
src = Glob('src/*/*.cpp') + Glob('src/*.cpp') 
# add the 3rd party cpp files
src = src+ Glob('extern/shaderLoader/*.cpp')
# incude directories
includeDirs = ['src/', 'extern/', 'extern/kiwi/include/']
# libraries
libraries = ['GLEW','glut','kiwicpp.a']
libPath = ['extern/kiwi/']
# build flags
buildFlags = ['-pg', '-g', '-std=c++0x','-L.']

# build
Program( 'raymarcher', src, CPPFLAGS=buildFlags, CPPPATH = includeDirs, LIBS=libraries, LIBPATH=libPath )
