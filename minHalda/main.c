// uloha-2.cpp -- Uloha 2
// Ondrej Harnusek, 16.4.2017 00:12:56

#include <stdio.h>
#include <stdlib.h>

#define PARENT(i) ((i-1)/2)
#define LEFT(i) (2*i + 1)
#define RIGHT(i) (2*i + 2)
int QUEUE_SIZE = 30;
typedef struct node                     // zaznam o vrchole v grafe
{
    unsigned int x:13;
    unsigned int y:13;
    unsigned int d:1;
    unsigned int p:1;
    unsigned int q:1;
    unsigned int r:1;
    unsigned int isVisited:1;
    unsigned int time;
    struct node* previous;
}NODE;

//Min halda
int queueCount=0;
NODE *queue;

//Inicializacia premennych
void initQ()
{
    queue = (NODE*)calloc(sizeof(NODE),QUEUE_SIZE);    //min halda
    queueCount=0;
}
//Vymenni obsah premennych
void swapQ(NODE *p1, NODE *p2)
{
    NODE temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}
//Usporiada haldu
void heapifyQ(int i)
{
    int index, temp = i;
    do
    {
        index=temp;
        if (LEFT(index) < queueCount)
        {
            if((queue[LEFT(index)].time) < (queue[index]).time)
            {
                temp = LEFT(index);
            }
        }
        if (RIGHT(index) < queueCount)
        {
            if((queue[RIGHT(index)].time) < (queue[temp]).time)
            {
                temp = RIGHT(index);
            }
        }

        if (index != temp)
        {
            swapQ(&queue[index], &queue[temp]);
        }
    } while(temp != index);
}
//Vlozi prvok d haldy
void pushQ(NODE o)
{
    queue[queueCount] = o;
    queueCount++;

    int index = queueCount - 1;

    while (index>0)
    {
        if( (queue[PARENT(index)].time) > (queue[index]).time)
        {
            swapQ(&queue[index], &queue[PARENT(index)]);
            index = PARENT(index);
        }
        else
        {
            break;
        }
    }
}
//Vrati koren haldy
NODE popQ()
{
    NODE minimum = queue[0];
    if (queueCount == 1)
    {
        queueCount--;
    }
    else
    {
        queueCount--;
        queue[0] = queue[queueCount];
        heapifyQ(0);
    }
    return minimum;
}
// UTILITA
void printQ()
{
    int i;
    printf("MIN:%d    ",queueCount);
    for(i=0; i<queueCount; i++)
        printf("%d, ",(queue[i]).time);
    printf("\n");
}
int randomize(int seed) {return (unsigned int)((seed * 1103515245 +12345) / 65536) % 139;}
int cmpfunc (const void * a, const void * b){return ( *(int*)a - *(int*)b );}
//TESTOVACIA PROCEDURA
int testQ()
{
    QUEUE_SIZE=1000;
    initQ();
    int i, arr[QUEUE_SIZE];
    NODE o;

    for(i=0; i<QUEUE_SIZE; i++)
    {
        o.time = arr[i] = randomize(i);
        pushQ(o);
    }
    qsort(arr, QUEUE_SIZE, sizeof(int), cmpfunc);
    for(i=0; i<QUEUE_SIZE; i++)
    {
       // printf("q = %d\tarr = %d\n",popQ().time , arr[i]);
        if(popQ().time != arr[i])
            return 1;
    }
    return 0;
}
//Hlavna metoda
int main()
{
    if(testQ())
    {
        printf("Chybna implementacia!\n");
    }
    else
    {
        printf("Implementacia prioritnej haldy OK!\n");
    }

    return 0;
}
