#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "matmul.h"
#include "matmul_strassen.h"


void Sum(double * A, double * B, double * C, size_t N)
{
	// C = A + B
	
	size_t i, j;
	
	for (j = 0; j < N; j++)
		for (i = 0; i < N; i++)
			C[i * N + j] = A[i * N + j] + B[i * N + j];
}

void Diff(double * A, double * B, double * C, size_t N)
{
	// C = A - B

	size_t i, j;

	for (j = 0; j < N; j++)
		for (i = 0; i < N; i++)
			C[i * N + j] = A[i * N + j] - B[i * N + j];
}

void Split(double * A, double * A11, double * A12, double * A21, double * A22, size_t N, size_t N_split)
{
	// A -> A11, A12, A21, A22
	
	size_t i, j;
	
	for (j = 0; j < N_split; j++)
		for (i = 0; i < N_split; i++)
			A11[i * N_split + j] = A[i * N + j];
			
	for (j = N_split; j < N; j++)
		for (i = 0; i < N_split; i++)
			A12[i * N_split + j - N_split] = A[i * N + j];
	
	for (j = 0; j < N_split; j++)			
		for (i = N_split; i < N; i++)
			A21[(i - N_split) * N_split + j] = A[i * N + j];
	
	for (j = N_split; j < N; j++)	
		for (i = N_split; i < N; i++)	
			A22[(i - N_split) * N_split + j - N_split] = A[i * N + j];
}

void Join(double * A, double * A11, double * A12, double * A21, double * A22, size_t N, size_t N_split)
{
	// A11, A12, A21, A22 -> A

	size_t i, j;
	
	for (j = 0; j < N_split; j++)
		for (i = 0; i < N_split; i++)
			A[i * N + j] = A11[i * N_split + j];
	
	for (j = N_split; j < N; j++)	
		for (i = 0; i < N_split; i++)	
			 A[i * N + j] = A12[i * N_split + j - N_split];
	
	for (j = 0; j < N_split; j++)		
		for (i = N_split; i < N; i++)	
			A[i * N + j] = A21[(i - N_split) * N_split + j];
	
	for (j = N_split; j < N; j++)		
		for (i = N_split; i < N; i++)	
			A[i * N + j] = A22[(i - N_split) * N_split + j - N_split];
}

void Strassen_2x2(double * A, double * B, double * C)
{
	// Алгоритм Штрассена для перемножения матриц 2х2
		
	double P1 = A[0] * (B[1] - B[3]);
	double P2 = B[3] * (A[0] + A[1]);
	double P3 = B[0] * (A[2] + A[3]);
	double P4 = A[3] * (B[2] - B[0]);
	double P5 = (A[0] + A[3]) * (B[0] + B[3]);
	double P6 = (A[1] - A[3]) * (B[2] + B[3]);
	double P7 = (A[0] - A[2]) * (B[0] + B[1]);
	
	C[0] = P5 + P4 - P2 + P6;
	C[1] = P1 + P2;
	C[2] = P3 + P4;
	C[3] = P5 + P1 - P3 - P7;
}

void Strassen(double * A, double * B, double * C, size_t N)
{
	if (N == 2)
	{
		Strassen_2x2(&A[0], &B[0], &C[0]);
	}
	else
	{
		size_t N_split = N / 2;
	
		// Разбиение на блоки матрицы А
		double *A11, *A12, *A21, *A22;
		A11 = (double *) malloc(N_split * N_split * sizeof(double));
		A12 = (double *) malloc(N_split * N_split * sizeof(double));
		A21 = (double *) malloc(N_split * N_split * sizeof(double));
		A22 = (double *) malloc(N_split * N_split * sizeof(double));
		ZeroMatrix(&A11[0], N_split);
		ZeroMatrix(&A12[0], N_split);
		ZeroMatrix(&A21[0], N_split);
		ZeroMatrix(&A22[0], N_split); 
		Split(&A[0], &A11[0], &A12[0], &A21[0], &A22[0], N, N_split);
		
		// Разбиение на блоки матрицы B
		double *B11, *B12, *B21, *B22;
		B11 = (double *) malloc(N_split * N_split * sizeof(double));
		B12 = (double *) malloc(N_split * N_split * sizeof(double));
		B21 = (double *) malloc(N_split * N_split * sizeof(double));
		B22 = (double *) malloc(N_split * N_split * sizeof(double));
		ZeroMatrix(&B11[0], N_split);
		ZeroMatrix(&B12[0], N_split);
		ZeroMatrix(&B21[0], N_split);
		ZeroMatrix(&B22[0], N_split);  
		Split(&B[0], &B11[0], &B12[0], &B21[0], &B22[0], N, N_split);
		
		// Промежуточные матрицы S
		double *S1, *S2, *S3, *S4, *S5, *S6, *S7, *S8, *S9, *S10;
		S1 = (double *) malloc(N_split * N_split * sizeof(double));
		S2 = (double *) malloc(N_split * N_split * sizeof(double));
		S3 = (double *) malloc(N_split * N_split * sizeof(double));
		S4 = (double *) malloc(N_split * N_split * sizeof(double));
		S5 = (double *) malloc(N_split * N_split * sizeof(double));
		S6 = (double *) malloc(N_split * N_split * sizeof(double));
		S7 = (double *) malloc(N_split * N_split * sizeof(double));
		S8 = (double *) malloc(N_split * N_split * sizeof(double));
		S9 = (double *) malloc(N_split * N_split * sizeof(double));
		S10 = (double *) malloc(N_split * N_split * sizeof(double));
		ZeroMatrix(&S1[0], N_split);
		ZeroMatrix(&S2[0], N_split);
		ZeroMatrix(&S3[0], N_split);
		ZeroMatrix(&S4[0], N_split);
		ZeroMatrix(&S5[0], N_split);
		ZeroMatrix(&S6[0], N_split);
		ZeroMatrix(&S7[0], N_split);
		ZeroMatrix(&S8[0], N_split);
		ZeroMatrix(&S9[0], N_split);
		ZeroMatrix(&S10[0], N_split);
		
		// Промежуточные матрицы P
		double *P1, *P2, *P3, *P4, *P5, *P6, *P7;
		P1 = (double *) malloc(N_split * N_split * sizeof(double));
		P2 = (double *) malloc(N_split * N_split * sizeof(double));
		P3 = (double *) malloc(N_split * N_split * sizeof(double));
		P4 = (double *) malloc(N_split * N_split * sizeof(double));
		P5 = (double *) malloc(N_split * N_split * sizeof(double));
		P6 = (double *) malloc(N_split * N_split * sizeof(double));
		P7 = (double *) malloc(N_split * N_split * sizeof(double));
		ZeroMatrix(&P1[0], N_split);
		ZeroMatrix(&P2[0], N_split);
		ZeroMatrix(&P3[0], N_split);
		ZeroMatrix(&P4[0], N_split);
		ZeroMatrix(&P5[0], N_split);
		ZeroMatrix(&P6[0], N_split);
		ZeroMatrix(&P7[0], N_split);
		
		// Вычисление промежуточных матриц S
		Diff(&B12[0], &B22[0], &S1[0], N_split);
		Sum(&A11[0], &A12[0], &S2[0], N_split);
		Sum(&A21[0], &A22[0], &S3[0], N_split);
		Diff(&B21[0], &B11[0], &S4[0], N_split);
		Sum(&A11[0], &A22[0], &S5[0], N_split);
		Sum(&B11[0], &B22[0], &S6[0], N_split);
		Diff(&A12[0], &A22[0], &S7[0], N_split);
		Sum(&B21[0], &B22[0], &S8[0], N_split);
		Diff(&A11[0], &A21[0], &S9[0], N_split);
		Sum(&B11[0], &B12[0], &S10[0], N_split);
		
		// Вычисление промежуточных матриц P (рекурсивно)
		Strassen(&A11[0], &S1[0], &P1[0], N_split);
		Strassen(&S2[0], &B22[0], &P2[0], N_split);
		Strassen(&S3[0], &B11[0], &P3[0], N_split);
		Strassen(&A22[0], &S4[0], &P4[0], N_split);
		Strassen(&S5[0], &S6[0], &P5[0], N_split);
		Strassen(&S7[0], &S8[0], &P6[0], N_split);
		Strassen(&S9[0], &S10[0], &P7[0], N_split);
		
		// Финальные блоки матрицы
		double *C11, *C12, *C21, *C22;
		C11 = (double *) malloc(N_split * N_split * sizeof(double));
		C12 = (double *) malloc(N_split * N_split * sizeof(double));
		C21 = (double *) malloc(N_split * N_split * sizeof(double));
		C22 = (double *) malloc(N_split * N_split * sizeof(double));
		ZeroMatrix(&C11[0], N_split);
		ZeroMatrix(&C12[0], N_split);
		ZeroMatrix(&C21[0], N_split);
		ZeroMatrix(&C22[0], N_split);
		 
		// C11
		Sum(&P5[0], &P4[0], &C11[0], N_split);
		Diff(&C11[0], &P2[0], &C11[0], N_split);
		Sum(&C11[0], &P6[0], &C11[0], N_split);
		// C12
		Sum(&P1[0], &P2[0], &C12[0], N_split);	
		// C21
		Sum(&P3[0], &P4[0], &C21[0], N_split);
		// C22
		Sum(&P5[0], &P1[0], &C22[0], N_split);
		Diff(&C22[0], &P3[0], &C22[0], N_split);
		Diff(&C22[0], &P7[0], &C22[0], N_split);
		
		// Объединение всех блоков в итоговую матрицу
		Join(&C[0], &C11[0], &C12[0], &C21[0], &C22[0], N, N_split);
		
		// Освобождение памяти
		free(A11);
		free(A12);
		free(A21);
		free(A22);
		
		free(B11);
		free(B12);
		free(B21);
		free(B22);
		
		free(C11);
		free(C12);
		free(C21);
		free(C22);
		
		free(P1);
		free(P2);
		free(P3);
		free(P4);
		free(P5);
		free(P6);
		free(P7);
		
		free(S1);
		free(S2);
		free(S3);
		free(S4);
		free(S5);
		free(S6);
		free(S7);
		free(S8);
		free(S9);
		free(S10);
	}
}

double CalcMatMulTime_Strassen(double * A, double * B, double * C, size_t N)
{
    struct timeval start, end;
    double r_time = 0.0;
    size_t i, j, k;

    ZeroMatrix(&C[0], N);

    gettimeofday(&start, NULL);
    Strassen(&A[0], &B[0], &C[0], N);
    gettimeofday(&end, NULL);
    
    r_time = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
    
    return r_time;
}

