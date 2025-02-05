#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "valid.h"

#define ull unsigned long long


extern ull N; 
extern ull M; 
extern unsigned int **matr;
extern const unsigned char bytes_cnt;
extern const ull mx_num;


void allocate_matrix(ull rows, ull cols);
void free_matrix();
void fill_matrix_random();
void fill_matrix_manual();
void print_matrix();

#endif
