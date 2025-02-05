#ifndef VALID_H
#define VALID_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ull unsigned long long

extern int argc;
extern char *argv[];
extern const ull mx_num;

bool is_num(const char *var);
bool validate_arguments(int argc, char *argv[]);

#endif