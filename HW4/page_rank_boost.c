#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

const int N = 194;  // количество станций
char * StationName[194];
int * A;  // матрица смежности


void ReadAdjacencyMatrix()
{
	// чтение файла с матрицей смежности графа Московского метро
	
	char *filename = "Moscow_Subway/SubwayAdj.txt";
	size_t i, j;
	
	FILE *fp = fopen(filename, "r");
	const unsigned MAX_LENGTH = 512;
    char buffer[MAX_LENGTH];
	
	i = 0;
	while (fgets(buffer, MAX_LENGTH, fp))
	{
		char delim[] = ",";
		char *ptr = strtok(buffer, delim);
		
		j = 0;
		while(ptr != NULL)
		{
			A[i * N + j] = atoi(ptr);
			j++;
			ptr = strtok(NULL, delim);
		}
		i++;
	}
	
	fclose(fp);
}

void PrepareAdjacencyMatrix(float * At, float alpha)
{
	// здесь мы транспонируем матрицу смежности, отнормируем и добавим демфирующий элемент
	
	size_t i, j;
	size_t rowSum;
	
	for (i = 0; i < N; i++)
	{
		rowSum = 0;
		for (j = 0; j < N; j++)
			rowSum += A[i * N + j];
		
		for (j = 0; j < N; j++)
			 At[j * N + i] = A[i * N + j] / (float)rowSum * alpha + (1 - alpha) / N;
	}
			
}

void PageRank(float * At, float * rank, float eps)
{
	size_t i, j;
	float yNorm;
	float err;
	float * y;
	y = (float *) malloc(N * sizeof(float));
	
	omp_set_num_threads(8);
	
	// начальное приближение
	#pragma omp parallel for private(i) shared(N, rank)
	for (i = 0; i < N; i++) 
		rank[i] = 1.0 / N;
	
	err = 10;
	while (err > eps)
	{
		#pragma omp parallel for private(i) shared(N, y)
		for (i = 0; i < N; i++) 
			y[i] = 0.0;
		

		#pragma omp parallel for collapse(2) \
							private(i, j) \
							shared(N, y, At, rank)
		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
				y[i] += At[i * N + j] * rank[j];
		
		yNorm = 0;
		
		#pragma omp parallel for reduction(+:yNorm) \
							private(i) \
							shared(N, y)
		for (i = 0; i < N; i++) 
			yNorm += y[i] * y[i];

		yNorm = sqrt(yNorm);
		
		err = 0;
		#pragma omp parallel for reduction(+:err) \
							private(i) \
							shared(N, yNorm, rank, y)
		for (i = 0; i < N; i++)
		{
			err += (rank[i] - y[i] / yNorm) * (rank[i] - y[i] / yNorm);
			rank[i] = y[i] / yNorm;
		}
		
		err = sqrt(err);
	}
}


int main()
{
    struct timeval start, end;
    double r_time = 0.0;
    int NRuns = 100;
    double *runtimes;
    runtimes = (double *) malloc(NRuns * sizeof(double));

    float alpha = atof(getenv("alpha"));
	// матрица смежности
	A = (int *) malloc(N * N * sizeof(int));
	ReadAdjacencyMatrix();

	float * At;
    At = (float *) malloc(N * N * sizeof(float));
    PrepareAdjacencyMatrix(&At[0], alpha);
	
	// алгоритм PageRank
	float * rank;
	rank = (float *) malloc(N * sizeof(float));
   
    double average_runtime = 0.0;
    for (int n = 0; n <= NRuns; n++)
    {
        gettimeofday(&start, NULL);
	    PageRank(&At[0], &rank[0], 1e-6);
        gettimeofday(&end, NULL);
        runtimes[n] = end.tv_sec - start.tv_sec + ((double) (end.tv_usec - start.tv_usec)) / 1000000;
        // printf("runtime %lf seconds\n", runtimes[n]);
        average_runtime += runtimes[n] / NRuns;
    }
    
	printf("pagerank average runtime %lf seconds\n", average_runtime);
    
	free(At);
	free(rank);
}


