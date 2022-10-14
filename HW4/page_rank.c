#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const int N = 194;  // количество станций
char * StationName[194];
int * A;  // матрица смежности


struct str
{
    float value;
    int index;
};

int compare(const void * a, const void * b)
{
    struct str *a1 = (struct str *)a;
    struct str *a2 = (struct str *)b;
    if ((*a1).value > (*a2).value)
        return -1;
    else if ((*a1).value < (*a2).value)
        return 1;
    else
        return 0;
}

void ReadStationName()
{
	// чтение файла с названиями станций
	
	char *filename = "Moscow_Subway/StationName.txt";
	size_t i;
	
	FILE *fp = fopen(filename, "r");
	const unsigned MAX_LENGTH = 4096 * 2;
    char buffer[MAX_LENGTH];
	
	while (fgets(buffer, MAX_LENGTH, fp))
	{
		char delim[] = ",";
		char *ptr = strtok(buffer, delim);
		i = 0;
		while(ptr != NULL)
		{
			StationName[i] = ptr;
			i++;
			ptr = strtok(NULL, delim);
		}
	}
	
	fclose(fp);
}

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
	
	// начальное приближение
	for (i = 0; i < N; i++) rank[i] = 1.0 / N;
	
	err = 10;
	while (err > eps)
	{
		yNorm = 0;
		
		for (i = 0; i < N; i++)
		{
			y[i] = 0;
			for (j = 0; j < N; j++)
				y[i] += At[i * N + j] * rank[j];

			yNorm += y[i] * y[i];
		}

		yNorm = sqrt(yNorm);
		
		err = 0;
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
	// матрица смежности
	A = (int *) malloc(N * N * sizeof(int));
	ReadAdjacencyMatrix();

	float alpha = atof(getenv("alpha"));
	float * At;
    At = (float *) malloc(N * N * sizeof(float));
	PrepareAdjacencyMatrix(&At[0], alpha);
	
	// алгоритм PageRank
	float * rank;
	rank = (float *) malloc(N * sizeof(float));
	PageRank(&At[0], &rank[0], 1e-6);

	// сортировка результата
	struct str objects[N];
    for (size_t i = 0; i < N; i++)
    {
        objects[i].value = rank[i];
        objects[i].index = i;
    }
	qsort(objects, N, sizeof(objects[0]), compare);
	
	// названия станций
	ReadStationName();
	
	// вывод результатов
	for (size_t i = 0; i < N; i++)
		printf("%25s\t%f\n", StationName[objects[i].index], objects[i].value);
	printf("\n");
	
	free(At);
	free(rank);
}


