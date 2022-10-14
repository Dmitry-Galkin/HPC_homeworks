#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void Show(int * A, size_t N)
{
    // вывод матрицы на печать

    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
            printf("%d\t", A[i * N + j]);
        printf("\n");
    }
    printf("\n");
}

void ZeroMatrix(int * A, size_t N)
{
    for(size_t i=0; i<N; i++)
        for(size_t j=0; j<N; j++)
            A[i * N + j] = 0;
}

double MatMul(int * A, int * B, int * C, size_t N)
{
    size_t i, j, k;

    ZeroMatrix(&C[0], N);

    for (k = 0; k < N; k++)
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < N; j++)
                C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
        }
}

void RandomAdjacencyMatrix(int * A, size_t N)
{
    // случайная матрица смежности без петель
    
    double r;
    srand(time(NULL));
    
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
        {
            r = rand() / (float)RAND_MAX;
            if (i != j && r >= 0.5)
                A[i * N + j] = 1;
            else
                A[i * N + j] = 0;
        }
}

void MatrixCopy(int * AFrom, int * ATo, size_t N)
{
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
            ATo[i * N + j] = AFrom[i * N + j];
}

void MatrixPower(int * A, int * APower, size_t N, size_t power)
{
    int * dummy;
    dummy = (int *) malloc(N * N * sizeof(int));
    
    if (power == 1)
    {
        MatrixCopy(&A[0], &APower[0], N);
    }
    else if (power % 2 != 0)
    {
        MatrixPower(&A[0], &APower[0], N, power - 1);
        MatMul(&APower[0], &A[0], &dummy[0], N);
        MatrixCopy(&dummy[0], &APower[0], N);
    }
    else
    {
        MatrixPower(&A[0], &APower[0], N, power / 2);
        MatMul(&APower[0], &APower[0], &dummy[0], N);
        MatrixCopy(&dummy[0], &APower[0], N);
    }
}

void main()
{
    // размер графа
    size_t N = atoi(getenv("N"));
    // показатель степени
    size_t power = atoi(getenv("power"));
    
    // матрица смежности
    int *A;
    A = (int *) malloc(N * N * sizeof(int));
    RandomAdjacencyMatrix(&A[0], N);
    
    // вывод на печать матрицы смежности
    printf("\n********** Adjacency Matrix with size = %li **********\n\n", N);
    Show(A, N);
    
    // возведение в степень
    int *APower;
    APower = (int *) malloc(N * N * sizeof(int));
    ZeroMatrix(&APower[0], N);
    MatrixPower(&A[0], &APower[0], N, power);
    
    // вывод на печать матрицы результата возведения в степень
    printf("\n********** Paths number (A^%li) **********\n\n", power);
    Show(APower, N);
    
    free(A);
    free(APower);
}