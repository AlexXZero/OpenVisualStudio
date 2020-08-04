#ifndef EXCEPTION_HPP_INCLUDED
#define EXCEPTION_HPP_INCLUDED

#include <stdexcept>    // for std::exception
#include <stdio.h>      // for vsnprintf()
#include <stdarg.h>     // for va_start(), va_end()

#define EXCEPTION_BUF_MAX 1024

class Exception: public std::exception {
private:
    char buf[EXCEPTION_BUF_MAX];
public:
    Exception(const char *format, ...): std::exception() {
        va_list va_list;
        va_start(va_list, format);
        vsnprintf(buf, sizeof(buf), format, va_list);
        va_end(va_list);
    }
    virtual ~Exception() = default;
    virtual const char* what() const noexcept { return buf; }
};

/* from http://stackoverflow.com/questions/2670816/how-can-i-use-the-compile-time-constant-line-in-a-string */
#define STRINGIZE2(x) #x
#define STRINGIZE(x) STRINGIZE2(x)

#define Exception(...) Exception(__FILE__ ":" STRINGIZE(__LINE__) ": Exception: " __VA_ARGS__)

#endif /* EXCEPTION_HPP_INCLUDED */
