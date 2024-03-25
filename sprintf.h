#ifndef SPRINTF_H
#define SPRINTF_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 300

int add_int(char *buffer, int value);
void add_float(char **bufferPtr, double value, int width, int precision);
void add_int_buffer(char **bufferPtr, long value, int width);

#endif
