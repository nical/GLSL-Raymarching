# source files
src = Glob('src/*/*.cpp') + Glob('src/*.cpp') 
# add the 3rd party cpp files
src = src+ Glob('extern/shaderLoader/*.cpp')
# incude directories
includeDirs = ['src/', 'extern/']

# libraries
libraries = ['GLEW','glut']

# build
Program( 'raymarcher', src, CPPPATH = includeDirs, LIBS=libraries )
