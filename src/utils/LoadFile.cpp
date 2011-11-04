
#include "utils/LoadFile.hpp"
#include <iostream>
#include <fstream>

namespace utils{

unsigned int LoadFile( const char* path, char *& buffer )
{

    int length;

    std::ifstream inputStream;
    inputStream.open (path, std::ios::binary );

    // get length of file:
    inputStream.seekg (0, std::ios::end);
    length = inputStream.tellg();
    inputStream.seekg (0, std::ios::beg);

    // allocate memory:
    buffer = new char [length];

    // read data as a block:
    inputStream.read (buffer,length);
    inputStream.close();

    return length;
}


}//namespace
