// zadanie3.c -- Zadanie 3 - Popolvar
// Ondrej Harnusek, 28.11.2016 15:16:59

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define TRUE 1
#define FALSE 0
#define PARENT(i) ((i-1)/2)
#define LEFT(i) (2*i + 1)
#define RIGHT(i) (2*i + 2)

typedef struct vertex                     // zaznam
{
    unsigned int x:16;
    unsigned int y:16;
    unsigned int isVisited:1;
    unsigned int time:31;
    struct vertex* previous;
}VERTEX;
typedef struct queueNode                     // zaznam
{
    unsigned int x:13;
    unsigned int y:13;
    unsigned int g:1;
    unsigned int d:1;
    unsigned int p:1;
    unsigned int q:1;
    unsigned int r:1;
    unsigned int priority;
}QUEUE_NODE;
typedef struct teleport                     // zaznam
{
    unsigned int x:16;
    unsigned int y:16;
    struct teleport* next;
}TELEPORT;

int width = 0, height = 0;      // rozmery mapy
char **map = NULL;              // mapa
int queueCount = 0;             // pocet prvkov radu
QUEUE_NODE *queue = NULL;              // prioritny rad
TELEPORT *teleports[10];

//**PRIORITY QUEUE**
/**Inicializacia premennych*/
void initQ(int queueSize)
{
    queue = (QUEUE_NODE*)calloc(sizeof(QUEUE_NODE),queueSize);    //min halda
    queueCount=0;
}
/**Vymenni obsah premennych*/
void swapQ(QUEUE_NODE *p1, QUEUE_NODE *p2)
{
    QUEUE_NODE temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}
/**Usporiada haldu*/
void heapifyQ(int i)
{
    int index, temp = i;
    do
    {
        index=temp;
        if (LEFT(index) < queueCount)
        {
            if((queue[LEFT(index)].priority) < (queue[index]).priority)
            {
                temp = LEFT(index);
            }
        }
        if (RIGHT(index) < queueCount)
        {
            if((queue[RIGHT(index)].priority) < (queue[temp]).priority)
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
/**Vlozi prvok d haldy*/
void pushQ(QUEUE_NODE o)
{
    queue[queueCount] = o;
    queueCount++;

    int index = queueCount - 1;

    while (index>0)
    {
        if( (queue[PARENT(index)].priority) > (queue[index]).priority)
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
/**Vrati koren haldy*/
QUEUE_NODE popQ()
{
    QUEUE_NODE minimum = queue[0];
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
//**DIJKSTRA**
/**Vrati cas prechodu daneho policka*/
int getTime(char box)
{
    if(box == 'C' || box == 'D' || box == 'P' || box == 'Q' || box == 'R')
        return 1;
    else if(box >= '0' && box <= '9')
        return 1;
    else if(box == 'H')
        return 2;
    else
        return -666;
}
/**inicializacia globalnych premennych*/
void initialize(VERTEX tab[2][2][2][2][height][width])
{
// inicializacia tabulky
    int d,p,q,r,x,y;
    for(d=0; d<2; d++)
        for(p=0; p<2; p++)
            for(q=0; q<2; q++)
                for(r=0; r<2; r++)
                    for(y=0; y<height; y++)
                        for(x=0; x<width; x++)
                        {
                            tab[d][p][q][r][y][x].time = INT_MAX;
                            tab[d][p][q][r][y][x].isVisited = FALSE;
                            tab[d][p][q][r][y][x].x = x;
                            tab[d][p][q][r][y][x].y = y;
                        }
//min halda
    queue = (QUEUE_NODE*)calloc(sizeof(QUEUE_NODE),16*height*width*sizeof(QUEUE_NODE));
    queueCount=0;
    QUEUE_NODE src;
    src.d = src.p = src.q = src.r = src.x = src.y = 0;
    tab[FALSE][FALSE][FALSE][FALSE][0][0].time = src.priority = getTime(map[0][0]);
    tab[FALSE][FALSE][FALSE][FALSE][0][0].previous = NULL;
    pushQ(src);

// najdenie princezien a teleportov
    int index=0, character;
    TELEPORT **node = NULL;
    for(y=0; y<height; y++)
    {
        for(x=0; x<width; x++)
        {
            character = map[y][x]-'0';
            if(map[y][x]=='P')//princezna
            {
                map[y][x]+=index;
                index++;
            }
            else if(character>=0 && character<=9)//teleport
            {
                node = &teleports[character];
                while((*node)!=NULL)
                {
                    node = &((*node)->next);
                }
                (*node) = (TELEPORT*)malloc(sizeof(TELEPORT));
                (*node)->next = NULL;
                (*node)->x = x;
                (*node)->y = y;
            }
        }
    }
}
/**Relaxacia suseda*/
VERTEX* relaxVertex(VERTEX* from, int y, int x, int d, int p, int q, int r, VERTEX tab[2][2][2][2][height][width])
{
    if((y>=height) || (y<0) || (x>=width) || (x<0))//hranice
    {
        return NULL;
    }
    if(tab[d][p][q][r][y][x].isVisited == TRUE || map[y][x]=='N')//no go
    {
        return NULL;
    }
    //Popolvar logic
    /*if(p == TRUE || q == TRUE || r == TRUE)
    {
        printf("Nasiel princeznu!\n");
        return &tab[d][p][q][r][y][x];
    }

    if(map[y][x] == 'D')
    {
        queueCount = 0;
        d = TRUE;
    }
    else if(d == TRUE && map[y][x] == 'P')
    {
        p = TRUE;
    }
    else if(d == TRUE && map[y][x] == 'Q')
    {
        q = TRUE;
    }
    else if(d == TRUE && map[y][x] == 'R')
    {
        r = TRUE;
    }*/
    //Relaxacia
    int newTime = from->time + getTime(map[y][x]);
    if(newTime < tab[d][p][q][r][y][x].time)
    {
        tab[d][p][q][r][y][x].time = newTime;
        tab[d][p][q][r][y][x].previous = from;
        //printf("relax:[%d,%d]=%d\t\t{%d,%d,%d,%d}\n",x,y,newTime,d,p,q,r);
        QUEUE_NODE neigh;
        neigh.x = x;
        neigh.y = y;
        neigh.d = d;
        neigh.p = p;
        neigh.q = q;
        neigh.r = r;
        neigh.priority = newTime;
        pushQ(neigh);
    }
    if(map[y][x] == 'D')
    {
        return &tab[d][p][q][r][y][x];
    }
    return NULL;
}
/** Vyriesenie teleportov*/
void visitTeleports(VERTEX* from, QUEUE_NODE node, VERTEX tab[2][2][2][2][height][width])
{
    int index = map[node.y][node.x] - '0';
    TELEPORT *tele;

    tele = teleports[index];
    while(tele!=NULL)
    {
        node.x = tele->x;
        node.y = tele->y;
        if(tab[node.d][node.p][node.q][node.r][node.y][node.x].isVisited == FALSE)
        {
            tab[node.d][node.p][node.q][node.r][node.y][node.x].time = node.priority;
            tab[node.d][node.p][node.q][node.r][node.y][node.x].previous = from;
            pushQ(node);
            printf("[%d,%d]\n",node.x,node.y);
        }
        else
        {
                printf("NOT \n");
        }

    //printf("[%d,%d]\n",node.x,node.y);
        tele = tele->next;
    }
}
/**Navstivenie vrcholu*/
int * visitVertex(QUEUE_NODE min, VERTEX tab[2][2][2][2][height][width], int *pathSize)
{
    int x,y,d,p,q,r,i,shift[]={-1,1};
    x = min.x;
    y = min.y;
    d = min.d;
    p = min.p;
    q = min.q;
    r = min.r;
    if(tab[d][p][q][r][y][x].isVisited == TRUE)  //je uz navstiveny     //je to dobre?
    {
        printf("REPETE: [%d,%d]\t\t{%d,%d,%d,%d}\n",x,y,d,p,q,r);
        return NULL;
    }
    tab[d][p][q][r][y][x].isVisited = TRUE; // NAVSTIVENY
    VERTEX *final, *from = &tab[d][p][q][r][y][x];
    //navsteva teleportov
    if(map[y][x]>='0' && map[y][x]<='9')
    {
        //visitTeleports(from,min,tab);
    }
    //relaxacia susedov
    for(i=0; i<2; i++)
    {
        final = relaxVertex(from, y+shift[i],x,d,p,q,r,tab);
        if(final != NULL) break;
        final = relaxVertex(from, y,x+shift[i],d,p,q,r,tab);
        if(final != NULL) break;
    }
    //vratenie cesty
    if(final != NULL)
    {
        VERTEX *v = final;
        *pathSize = 0;
        while(v!=NULL)
        {
            (*pathSize)++;          map[v->y][v->x] = '.';
            v = v->previous;
        }
        int i,*path = ((int*)malloc(2*(*pathSize)*sizeof(int)));
        for(i=0;i<(*pathSize);++i){
            path[i*2] = final->x;
            path[i*2+1] = final->y;
            final = final->previous;
        }
        return path;
    }
    return NULL;
}
/** Uvolnenie globalnych premennych*/
void terminate(VERTEX tab[2][2][2][2][height][width])
{
    printMap();
    printSimpleTab(tab,0,0,0,0);
    //printTeleports();
    free(queue);//Uvolnenie pamate
    int i;
    TELEPORT *del = NULL, *temp = NULL;
    for(i=0; i<10; i++)
    {
        temp = teleports[i];
        teleports[i] = NULL;
        while(temp!=NULL)
        {
            del = temp;
            temp = temp->next;
            free(del);
        }
    }
}
/** Upraveny dijkstrov algoritmus vrati najkratsiu cestu*/
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)
{
    width = m;                  // sirka
    height = n;                 // vyska
    map = mapa;                 // mapa

    QUEUE_NODE min;
    VERTEX tab[2][2][2][2][height][width];
    int i, *path = NULL;
    initialize(tab);

    while(path==NULL)
    {
        if(queueCount==0)
        {
            printf("Popolvar nema riesenie!\n");
            break;
        }
        min = popQ();
        //printf("pop:[%d,%d]\t\t{%d,%d,%d,%d}\n",x,y,d,p,q,r);
        path = visitVertex(min,tab,dlzka_cesty);
    }
    terminate(tab);
    return path;
}
//**TEST**
void printSimpleTab(VERTEX tab[2][2][2][2][height][width], int a, int b, int c, int d)    // VYTLACI TABULKU
{
    int x,y;

    for(y=0; y<height; y++)
    {// vyska
        for(x=0; x<width; x++)
        {// sirka
            (tab[a][b][c][d][y][x].time == INT_MAX)? printf("OO ") : printf("%2d ",tab[a][b][c][d][y][x].time );
            //printf("'%d' ",tab[a][b][c][d][y][x].isVisited);
        }
        printf("\n");
    }
    printf("\n");
}
void printCameFrom(VERTEX tab[2][2][2][2][height][width])    // VYTLACI TABULKU
{
    int x,y;
    VERTEX *previous = NULL;
    for(y=0; y<height; y++)
    {// vyska
        for(x=0; x<width; x++)
        {// sirka
            previous = tab[0][0][0][0][y][x].previous;
            (previous == NULL)? printf("[ NULL] ") : printf("[%2d,%2d] ",previous->x,previous->y);
        }
        printf("\n");
    }
    printf("\n");
}
void printMap()                                                                         // VYTLACI MAPU
{
    int i,j;

    printf("\nwidth=%d height=%d\n",width, height);
    for(i=0; i<height; i++)
    {
        for(j=0; j<width; j++)
            printf("%c ",map[i][j]);

        printf("\n");
    }
    printf("\n");
}
void printTeleports()                                                                         // VYTLACI TELEPORTY
{
    int i;
    TELEPORT *node = NULL;
    for(i=0; i<10; i++)
    {
        printf("TEL_%d = ",i);
        node = teleports[i];
        while(node!=NULL)
        {
            printf("[%d,%d]",node->x,node->y);
            node = node->next;
        }
        printf("\n");
    }
}
void printQ()
{
    int i;
    printf("MIN:%d    ",queueCount);
    for(i=0; i<queueCount; i++)
        printf("%d, ",(queue[i]).priority);
    printf("\n");
}
int randomize(int seed) {return (unsigned int)((seed * 1103515245 +12345) / 65536) % 139;}
int cmpfunc (const void * a, const void * b){return ( *(int*)a - *(int*)b );}
int testQ()
{
    int size=100;
    initQ(size);
    int i, temp, arr[size];
    QUEUE_NODE o;

    for(i=0; i<size; i++)
    {
        o.priority = arr[i] = randomize(i);
        pushQ(o);
    }
    qsort(arr, size, sizeof(int), cmpfunc);
    for(i=0; i<size; i++)
    {
        temp = popQ().priority;
        //printf("q = %d\tarr = %d\n",temp , arr[i]);
        if(temp != arr[i])
            return 1;
    }
    free(queue);
    return 0;
}
int testDijkstra()
{
    int dlzka_cesty;

    int n=10, m=15, t=0;
    char map[10][15] = {
                        {'C', 'C', 'H', 'H', 'H', 'C', 'H', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', '1', 'H', 'C', '1', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'H', 'H', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'H', 'H', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'H', 'H', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'H', 'H', 'C'},
                        {'1', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'P', 'C', 'C', 'H', 'H', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '1', 'C', 'C'},
                        {'C', 'C', 'C', 'P', '1', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'N', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'P', 'C', 'C', 'C', 'C', 'C', 'C', 'N', 'D'}
                        };
    char * mapa[10] = { map[0], map[1], map[2], map[3], map[4], map[5], map[6], map[7], map[8], map[9]};
    //printf("size = %dB\n",sizeof(NODE));
    int i, *cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
    if(cesta==NULL) return 1;
    for(i=0;i<dlzka_cesty;++i){
            //printf("%d %d\n", cesta[i*2], cesta[i*2+1]);
    }
    return 0;
}
/**Hlavna funkcia*/
int main()
{
    if(testQ())
        printf("Chybna implementacia prioritnej haldy!\n");
    else
        printf("Implementacia prioritnej haldy OK!\n");

    if(testDijkstra())
        printf("Chybna implementacia dijkstrovho algoritmu!\n");
    else
        printf("Implementacia dijkstrovho algoritmu OK!\n");

    return 0;
}
