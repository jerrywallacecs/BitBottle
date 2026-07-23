#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ErrorHandling.h"

void FatalError(char* fmt, ...)
{
    va_list argptr;

    va_start(argptr, fmt);
    printf("Fatal Error: ");
    vprintf(fmt, argptr);
    va_end(argptr);

    exit(-1);
}