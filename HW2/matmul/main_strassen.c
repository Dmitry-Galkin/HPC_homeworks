#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "matmul.h"
#include "matmul_strassen.h"


int main()
{
	// берем размер матрицы из окружения
    size_t N = atoi(getenv("N"));
    
    int NRuns = 5;
    size_t i, j, k;

    double *runtimes;
    double *A, *B, *C;
    
    A = (double *) malloc(N * N * sizeof(double));
    B = (double *) malloc(N * N * sizeof(double));
    C = (double *) malloc(N * N * sizeof(double));
    runtimes = (double *) malloc(NRuns * sizeof(double));

    RandomMatrix(&A[0], N);
    RandomMatrix(&B[0], N);
    
    // Strassen
    double average_runtime = 0.0;
    for(int n=0; n<NRuns; n++)
    {
        runtimes[n]=CalcMatMulTime_Strassen(&A[0], &B[0], &C[0], N);
        printf("runtime %lf seconds\n", runtimes[n]);
        average_runtime += runtimes[n]/NRuns;
    }
    printf("average runtime Strassen %lf seconds\n", average_runtime);
    printf("---------------------------------\n");
    
    free(A); 
    free(B);
    free(C);

    return 0;
}

