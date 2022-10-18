#ifndef KERNERL_GEMM_H
#define KERNERL_GEMM_H

#include <stdio.h>
#include <stdlib.h>

#define dimension 4096
#define NI dimension
#define NJ dimension
#define NK dimension

#define N 128

void mm(float C[NI*NJ], float A[NI*NK], float B[NK*NJ], float alpha, float beta);

#endif