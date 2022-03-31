//
// Created by badcw on 2022/3/25.
//

#include <dbg.h>

int aok = 0;

/* printf version of libspl_assert */
void libspl_assertf(const char* file, const char* func, int line, const char* format, ...) {
    va_list args;

    va_start(args, format);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    fprintf(stderr, "ASSERT at %s:%d:%s()", file, line, func);
    va_end(args);
    if (aok) { return; }
    abort();
}
