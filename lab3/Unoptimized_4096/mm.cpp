#include "mm.h"

// void load_buf_a(float A[NI*NK], float buf_a[NK], int index) // row wise
// {
//   LOAD_LOOP_A: for(int i = 0 ; i < NK ; i++)
//   {
//     buf_a[i] = A[index*NK + i];
//   }
// }

// void load_buf_b(float B[NK*NJ], float buf_b[NK], int index) // column wise reading
// {
//   LOAD_LOOP_B: for(int i = 0 ; i < NK ; i++)
//   {
//     buf_b[i] = B[i*NK + index];
//   }
// }

// void load_buf_c(float C[NI*NJ], float buf_c[NJ], int index, float beta) // multiplier
// {
//   LOAD_LOOP_C: for(int i = 0 ; i < NJ ; i ++)
//   {
//     buf_c[i] = C[index*NJ + i] * beta;
//   }
// }

// void store_buf(float C[NI*NJ], float buf_c[NJ], int index)
// {
//   STORE_LOOP: for(int i = 0 ; i < NJ ; i++)
//   {
//     C[index*NI + i] += buf_c[i];
//   }
// }

// void compute(float buf_a[NK], float buf_b[NK], float buf_c[NJ], float alpha, int j)
// {
//   int k = 0;
//   COMPUTE_LOOP: for(k = 0 ; k < NK ; k++)
//   {
//     buf_c [j] += alpha * buf_a[k] * buf_b[k];
//   }
// }

// void mm(float C[NI*NJ], float A[NI*NK], float B[NK*NJ], float alpha, float beta)
// {
//   float buf_a[NK], buf_b[NK], buf_c[NJ];
//   for(int i = 0 ; i < NI ; i++)
//   {
//     load_buf_c(C, buf_c, i, beta);
//     load_buf_a(A, buf_a, i);
//     for(int j = 0 ; j < NK ; j++)
//     {
//       load_buf_b(B, buf_b, j);
//       compute(buf_a, buf_b, buf_c, alpha, j);
//       store_buf(C, buf_c, i);
//     }
//   }
// }

void load_A(float A[NI*NK], float A_buff[NK],int i)
{      
       	int j;
        COPY_LOOP: for(j=0;j<NK;j++){
		       A_buff[j] = A[i*NK + j]; }
}


void load_B(float A[NK*NJ], float A_buff[NJ],int i)
{      
       	int j;
	//column is extracted
        COPY_LOOP: for(j=0;j<NK;j++){
		       A_buff[j] = A[j*NJ + i]; } 
}

void load_C(float A[NI*NJ], float A_buff[NJ],int i,float beta)
{      
       	int j;
        COPY_LOOP: for(j=0;j<NJ;j++){
		       A_buff[j] = A[i*NJ + j] * beta; }
}


		       
void store(float C_buff[NJ], float C[NI*NJ],int i)
{
        int j;
	COPY_LOOP: for(j=0;j<NJ;j++){
		       C[i*NI + j]=C_buff[j];
		   }

}

void compute(float A_buff[NK], float B_buff[NJ],float C_buff[NJ],int i,int j, float alpha)
{
        int k;
	MULT_LOOP: for(k=0;k<NK;k++){
		       C_buff[j]+= alpha * A_buff[k] * B_buff[k];
		   }

}

void mm(float C[NI*NJ], float A[NI*NK], float B[NK*NJ], float alpha, float beta)
{
  
  float A_buff[NK], B_buff[NJ], C_buff[NJ];
  // => Form C := alpha*A*B + beta*C,
//A is NIxNK
//B is NKxNJ
//C is NIxNJ
//
	
  int i, j, k;

//To fetch one row of A and one column of B at a time - total number of iterations needed NI * NJ
 OUTER_LOOP: for(i=0;i<NI;i++){
           load_A(A,A_buff,i);
	   load_C(C,C_buff,i,beta);
           INNER_LOOP:for(j=0;j<NJ;j++){
		      load_B(B,B_buff,j);		      
                      compute(A_buff, B_buff, C_buff, i,j,alpha);
	              store(C_buff,C,i);
                      }
	     }
}