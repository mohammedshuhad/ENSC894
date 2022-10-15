#include "mm.h"

void init_array(float C_hw[NI*NJ], float C_sw[NI*NJ], float A[NI*NK], float B[NK*NJ])
{
  int i, j;

  for (i = 0; i < NI; i++)
    for (j = 0; j < NJ; j++)
    {
      C_hw[i*NJ+j] = (float)((i*j+1) % NI) / NI;
      C_sw[i*NJ+j] = (float)((i*j+1) % NI) / NI;
    }
  for (i = 0; i < NI; i++)
    for (j = 0; j < NK; j++)
      A[i*NK+j] = (float)(i*(j+1) % NK) / NK;
  for (i = 0; i < NK; i++)
    for (j = 0; j < NJ; j++)
      B[i*NJ+j] = (float)(i*(j+2) % NJ) / NJ;
}

float array_sum(float C[NI*NJ])
{
  int i, j;

  float sum = 0.0;
  
  for (i = 0; i < NI; i++)
    for (j = 0; j < NJ; j++)
      sum += C[i*NJ+j];

  return sum;
}

void kernel_gemm_sw(float C[NI*NJ], float A[NI*NK], float B[NK*NJ], float alpha, float beta)
{
  int i, j, k;
  for (i = 0; i < NI; i++) 
  {
    for (j = 0; j < NJ; j++) 
    {
      C[i*NJ+j] *= beta;
    }
    for (j = 0; j < NJ; j++) 
    {
      for (k = 0; k < NK; ++k) 
      {
        C[i*NJ+j] += alpha * A[i*NK+k] * B[k*NJ+j];
      }
    }
  }
}

int main(int argc, char** argv)
{
  /* Variable declaration/allocation. */
  float *A = (float *)malloc(NI*NK*sizeof(float));
  float *B = (float *)malloc(NK*NJ*sizeof(float));
  float *C_hw = (float *)malloc(NI*NJ*sizeof(float));
  float *C_sw = (float *)malloc(NI*NJ*sizeof(float));

  /* Initialize array(s). */
  init_array (C_hw, C_sw, A, B);

  /* Run kernel. */
  kernel_gemm_sw (C_sw, A, B, 1.5, 2.5);
  mm (C_hw, A, B, 1.5, 2.5);
  
  /* Print results. */
  float array_sum_sw = array_sum(C_sw);
  float array_sum_hw = array_sum(C_hw);

  if(array_sum_sw != array_sum_hw)
    printf("TEST FAILED, results not matching, array_sum_sw = %f, array_sum_hw = %f.\n",array_sum_sw, array_sum_hw);
  else
    printf("TEST PASSED!\n");

  /* free memory for A, B, C */
  free(A);
  free(B);
  free(C_hw);
  free(C_sw);
  
  return 0;
}