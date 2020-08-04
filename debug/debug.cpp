#include "debug.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <dlfcn.h>
#include <cxxabi.h>
#include <signal.h>

#include <execinfo.h>

void print_backtrace()
{
    int depth = 20;
    void *trace_elems[depth];
    int trace_elem_count = backtrace( trace_elems, depth );
    std::cerr << "stack trace:" << std::endl;
    for (int i = 0; i < trace_elem_count; i++) {
        #if 0 // man dladdr:
        typedef struct {
            const char *dli_fname;  /* Pathname of shared object that
                                          contains address */
            void       *dli_fbase;  /* Address at which shared object
                                          is loaded */
            const char *dli_sname;  /* Name of nearest symbol with address
                                          lower than addr */
            void       *dli_saddr;  /* Exact address of symbol named
                                          in dli_sname */
        } Dl_info;
        #endif
        Dl_info info;
        int status;
        dladdr(trace_elems[trace_elem_count - 1 - i], &info);
        if (!info.dli_saddr) continue;
        const char *realname = abi::__cxa_demangle(info.dli_sname, 0, 0, &status);
        if (status != 0) realname = info.dli_sname;
        std::cout << "[" << info.dli_saddr << "]: " << realname << std::endl;
    }
}

void debug_init()
{
    std::set_terminate([](){
        error("terminate");
        print_backtrace();
        abort();
    });
    signal(SIGSEGV, [](int){
        error("Segmentation fault");
        print_backtrace();
        abort();
    });
}
