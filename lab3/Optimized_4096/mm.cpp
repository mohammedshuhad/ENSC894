#include "mm.h"

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