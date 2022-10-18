#include "mm.h"


void load_A (float A[NI * NK], float A_buff[NK], int i)
{
    int j;
    COPY_LOOP_A:for (j = 0; j < NK; j++)
    {
        #pragma HLS pipeline II=1
        A_buff[j] = A[i * NK + j];
    }
}


void load_B (float B[NK * NJ], float B_buff[NJ * N], int j)
{
    int k, jj;
    //Loop to extract N columns
    OUTER_LOOP_B:for (jj = 0; jj < N; jj++, j++)
    {
        #pragma HLS loop flatten
        COPY_LOOP_B:for (k = 0; k < NK; k++)
        {
            #pragma HLS pipeline II=1
            B_buff[jj * NJ + k] = B[k * NJ + j];
        }
    }
}

void load_C (float C[NI * NJ], float C_buff[NJ], int i, float beta)
{
    int j;
    COPY_LOOP_C:for (j = 0; j < NJ; j++)
    {
        #pragma HLS pipeline II=1
        C_buff[j] = C[i * NJ + j] * beta;
    }
}


void store (float C_buff[NJ], float C[NI * NJ], int i)
{
    int j;
    COPY_LOOP_STORE:for (j = 0; j < NJ; j++)
    {
        #pragma HLS pipeline II=1
        C[i * NJ + j] = C_buff[j];
    }
}

void compute (float A_buff[NK], float B_buff[N * NJ], float C_buff[NJ], int j, float alpha)
{
    #pragma HLS array_partition variable=A_buff block factor=16  
    #pragma HLS array_partition variable=B_buff cyclic factor=16
    int k;
    float c_value = C_buff[j];
    MULT_LOOP:for (k = 0; k < NK; k++)
    {                      
        c_value += alpha * A_buff[k] * B_buff[(j % N) * NJ + k];
    }
    C_buff[j] = c_value;
}

void mm (float C[NI * NJ], float A[NI * NK], float B[NK * NJ], float alpha,
    float beta)
{
    int i, j, k, jj,c;
    float A_buff[NK], B_buff[N * NJ], C_buff[NJ];
    OUTER_LOOP:for (i = 0; i < NI; i++)
    {
        // load A and C           
        load_A (A, A_buff, i);
        load_C (C, C_buff, i, beta);
        COUNT_LOOP:for (jj = 0; jj < NJ; jj += N)
        {
            //load N columns of B from jj to (jj+N-1) into B_buff as N rows
            load_B (B, B_buff, jj);
            j=jj;
            INNER_LOOP:for (c=0; c< N ; c++,j++)
            {            
                //multiply 1 row of A and B and store result in column j of C_buff     
                compute (A_buff, B_buff, C_buff, j, alpha);
            }
        }
        store (C_buff, C, i);
    }
}