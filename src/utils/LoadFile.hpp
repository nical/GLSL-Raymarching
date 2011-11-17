#pragma once

#ifndef UTILS_LOADFILE_HPP
#define UTILS_LOADFILE_HPP

#include <string>

namespace utils{

  bool LoadTextFile( const std::string& path, std::string& buffer );

}//namespace

#endif
