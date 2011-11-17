
#ifndef UTILS_CHECKGLERROR_HPP
#define UTILS_CHECKGLERROR_HPP

#define CHECKERROR utils::gl_check_error(__LINE__,__PRETTY_FUNCTION__);

namespace utils{
bool gl_check_error(int line, const char* comment = "");
}

#endif
