#include "mm.h"

void load_A(float A[NI*NK], float A_buff[NK*N],int i)
{      
    ROW_LOOP_A: for (int k = 0 ; k < N ; k++)
    {
		#pragma HLS pipeline II = 1
        COPY_LOOP_A: for(int j=0;j<NK;j++)
        {   
        A_buff[k*NK + j] = A[i*NK + k*NK + j]; 
        }
    }
}

void load_B(float B[NK*NJ], float B_buff[NJ*N],int j)
{      
    COLUMN_LOOP_B:for(int jj=0;jj<N;jj++,j++)
    {
		#pragma HLS pipeline II = 1
        //all elements of #jj column is extracted
        COPY_LOOP_B: for(int k=0;k<NK;k++)
        {
        B_buff[jj*NJ + k] = B[k*NJ + j]; 
        }
    }
}

void load_C(float C[NI*NJ], float C_buff[NJ*N],int i,float beta)
{      
    int j;
    ROW_LOOP_C: for (int k = 0 ; k < N ; k++)
    {
		#pragma HLS pipeline II = 1
        COPY_LOOP_C: for(j=0;j<NJ;j++)
        {
            C_buff[k*NJ + j] = C[i*NJ + k*NJ + j] * beta; 
        }
    }
}
		       
void store(float C_buff[NJ*N], float C[NI*NJ],int i)
{
    int j;
	STORE_LOOP: for(j=0;j<NJ*N;j++)
    {
		#pragma HLS pipeline II = 1
        C[i*NJ + j]=C_buff[j]; 
    }

}

void compute(float A_buff[NK*N], float B_buff[NJ*N],float C_buff[NJ*N], int j, float alpha)
{
    int b_column = 0;
	int p = 0;
    //TODO : figure out the currect order of loop, i think it wouldnt matter in rtl impl, maybe wit blocking
	#pragma HLS array_partition variable = A_buff block factor = 32
	#pragma HLS array_partition variable = B_buff block factor = 32
	#pragma HLS array_partition variable = C_buff block factor = 32
    OUTPUT_ITERATOR: for(int p = 0 ; p < N ; p++)
    {
        ROW_ITERATOR: for(int k = 0 ; k < NK ; k ++)
        {
			#pragma HLS unroll factor = 32
            TILE_ITERATOR : for(int q = 0 ; q < N ; q++)
            {
            C_buff[q*NJ + p + j] += alpha * A_buff[q*NJ + k] * B_buff[b_column*NK + k];
            }
        }
        b_column++;
    }
}

void mm(float C[NI*NJ], float A[NI*NK], float B[NK*NJ], float alpha, float beta)
{
    int i, j, k, jj;
    float A_buff[NK*N], B_buff[NJ*N], C_buff[NJ*N];
    OUTER_LOOP: for(i=0;i<NI;i+= N)
    {	     
        load_A(A,A_buff,i);
        load_C(C,C_buff,i,beta);
        INNER_LOOP:for(j=0; j<NJ ; j+=N)
        {	
            load_B(B,B_buff,j); 
            compute(A_buff, B_buff, C_buff,j,alpha);
        }
        store(C_buff,C,i);                                				 
    }		       
}