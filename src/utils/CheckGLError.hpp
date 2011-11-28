
#ifndef UTILS_CHECKGLERROR_HPP
#define UTILS_CHECKGLERROR_HPP

#define CHECKERROR gl_check_error(__LINE__,__PRETTY_FUNCTION__);


bool gl_check_error(int line, const char* comment = "");


#endif
