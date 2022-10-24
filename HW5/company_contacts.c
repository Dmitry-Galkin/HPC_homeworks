#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


int genRandInt(int lower, int upper)
{
    // Генерирование случайного целого числа в заданном диапазоне [lower, upper).

    int randInt = (rand() % (upper - lower)) + lower;
    
    return randInt;
}

int genRecvIndex(int lower, int upper, int* msg, int startIndex)
{
    // Генерирование еще неиспользованного индекса узла.

    int randIndex = genRandInt(lower, upper) + startIndex;

    while (msg[randIndex] == -1)
    {
        randIndex = genRandInt(lower, upper) + startIndex;
    }
    
    return msg[randIndex];
}

void initMsg(int* msg, int psize)
{
    // Инициализация начального сообщения.

    msg[0] = 0;

    for (size_t i = 1; i < 2 * psize + 1; i++)
    {
        if (i > psize)
        {
            msg[i] = i - psize - 1;
        }
        else
        {
            msg[i] = -1;
        }
    }
}

void formMsg(int* msg, int prank, int psize)
{
    // Корректировка сообщения. Чтобы понимать, от кого пришло и где уже было.

    msg[0]++;
    msg[msg[0]] = prank;
    msg[prank + psize + 1] = -1;
}

void showMsg(int* msg, int psize)
{
    // Вывод всего сообщения со всей служебной информацией.
    for (int i = 0; i < 2 * psize + 1; i++)
    {
        printf("%d ", msg[i]);
    }
    printf("\n");
}

void showSeq(int* msg, int psize)
{
    // Вывод последовательности, где мы уже были. 
    for (int i = 1; i < psize + 1; i++)
    {
        printf("%d", msg[i]);
        if (i != psize)
        {
            printf(" -> ");
        }
    }
    printf("\n");
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int psize;
    int prank;
    int ierr;
    int receiver;
    int sender;
    MPI_Status status;

    srand(time(NULL));

    ierr = MPI_Comm_size(MPI_COMM_WORLD, &psize);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    
    //  Сообщение пересылаемое.
    int* msg;
    msg = (int*)malloc((2 * psize + 1) * sizeof(int));

    if (prank == 0)
    {
         // начальное сообщение
        initMsg(&msg[0], psize);

        // формируем сообщение
        formMsg(&msg[0], prank, psize);
    
        // кому посылаем сообщение
        receiver = genRecvIndex(0, psize, msg, psize + 1);
        // пересылка
        MPI_Ssend(&msg[0], 2 * psize + 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);

        printf("I'm %d. Send message to %d.\n", prank, receiver);
    }
    else
    {
        MPI_Recv(&msg[0], 2 * psize + 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        sender = status.MPI_SOURCE;
        
        // формируем сообщение
        formMsg(&msg[0], prank, psize);

        if (msg[0] != psize)
        {
            // кому посылаем сообщение
            receiver = genRecvIndex(0, psize, msg, psize + 1);
            printf("I'm %d. Received message from %d. Send to %d.\n", prank, sender, receiver);
            MPI_Ssend(&msg[0], 2 * psize + 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);
        }
        else
        {
            printf("I'm %d. Received message from %d. I'm the last one. Let me show you the sequence:\n", prank, sender);
            showSeq(&msg[0], psize);
        }
    }
    
    free(msg);
    MPI_Finalize();
}
