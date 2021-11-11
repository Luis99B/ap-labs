#include <stdio.h>
#include "logger.h"
#include <stdarg.h>

// ANSI color codes
#define NORMAL "\e[0m" // default
#define LOG "\e[0;34m" // blue
#define INFO "\e[0;32m" // green
#define WARNING "\e[0;33m" // yellow
#define ERROR "\e[0;31m" // red
#define PANIC "\e[0;35m" // purple

int initLogger(char *logType) {
    printf("%sInitializing Logger on:%s %s\n", LOG, NORMAL, logType);
    return 0;
}

int infof(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    printf("%sINFO:%s ", INFO, NORMAL);
    vprintf(format, ap);
    printf("\n");
    va_end(ap);
    return 0;
}

int warnf(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    printf("%sWARNING:%s ", WARNING, NORMAL);
    vprintf(format, ap);
    printf("\n");
    va_end(ap);
    return 0;
}

int errorf(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    printf("%sERROR:%s ", ERROR, NORMAL);
    vprintf(format, ap);
    printf("\n");
    va_end(ap);
    return 0;
}

int panicf(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    printf("%sPANIC:%s ", PANIC, NORMAL);
    vprintf(format, ap);
    printf("\n");
    va_end(ap);
    return 0;
}
