
#include "utils/LoadFile.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace utils{

bool LoadTextFile( const string& path, string& buffer )
{
	string line;

	ifstream file(path);
	if (file.is_open())
    {
		while (!file.eof())
        {
			getline(file, line);
		  	buffer.append(line);
			buffer.append("\n");
		}
		file.close();
	}
    else
    {
        cout << "Failed loading " << path << endl;
	    return false;
    }

return true;
}


unsigned int LoadFile2( const char* path, char *& buffer )
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
