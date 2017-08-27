// zadanie3.c -- Zadanie 3 - Popolvar
// Ondrej Harnusek, 28.11.2016 15:16:59

#include <stdio.h>
#include <stdlib.h>
#define TIME_MAX 1023
#define TRUE 1
#define FALSE 0
#define PARENT(i) ((i-1)/2)
#define LEFT(i) (2*i + 1)
#define RIGHT(i) (2*i + 2)
const char GENERATOR='G', DRAGON='D',FIRST_PRINCESS ='P', SECOND_PRINCESS='Q', THIRD_PRINCESS='R';
const char SIMPLE_PATH='C', DOUBLE_PATH='H',NO_PATH ='N';
typedef struct vertex                     // zaznam
{
    unsigned int x:10;
    unsigned int y:10;
    unsigned int isVisited:1;
    unsigned int time:10;
    struct vertex* previous;
}VERTEX;
typedef struct queueNode                     // zaznam
{
    unsigned int x:10;
    unsigned int y:10;
    unsigned int g:1;
    unsigned int d:1;
    unsigned int p:1;
    unsigned int q:1;
    unsigned int r:1;
    unsigned int teleported:1;
    unsigned int priority:10;
}QUEUE_NODE;
typedef struct teleport                     // zaznam
{
    unsigned int x:10;
    unsigned int y:10;
    struct teleport* next;
}TELEPORT;

int width = 0, height = 0;      // rozmery mapy
char **map = NULL;              // mapa
int queueCount = 0;             // pocet prvkov radu
QUEUE_NODE *queue = NULL;       // prioritny rad
TELEPORT *teleports[10];

//**PRIORITY QUEUE**
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
    if(box == SIMPLE_PATH || box == DRAGON || box == FIRST_PRINCESS || box == SECOND_PRINCESS || box == THIRD_PRINCESS || box == GENERATOR)
        return 1;
    else if(box >= '0' && box <= '9')
        return 1;
    else if(box == DOUBLE_PATH)
        return 2;
    else
        return -666;
}
/**inicializacia globalnych premennych*/
void initialize(VERTEX tab[2][2][2][2][2][height][width])
{
// inicializacia tabulky
    int g,d,p,q,r,x,y;
    for(g=0; g<2; g++)
        for(d=0; d<2; d++)
            for(p=0; p<2; p++)
                for(q=0; q<2; q++)
                    for(r=0; r<2; r++)
                        for(y=0; y<height; y++)
                            for(x=0; x<width; x++)
                            {
                                tab[g][d][p][q][r][y][x].time = TIME_MAX;
                                tab[g][d][p][q][r][y][x].isVisited = FALSE;
                                tab[g][d][p][q][r][y][x].x = x;
                                tab[g][d][p][q][r][y][x].y = y;
                            }
//min halda
    queue = (QUEUE_NODE*)calloc(sizeof(QUEUE_NODE),32*height*width*sizeof(QUEUE_NODE));
    queueCount=0;
    QUEUE_NODE src;
    src.d = src.p = src.q = src.r = src.x = src.y = 0;
    tab[FALSE][FALSE][FALSE][FALSE][FALSE][0][0].time = src.priority = getTime(map[0][0]);
    tab[FALSE][FALSE][FALSE][FALSE][FALSE][0][0].previous = NULL;
    pushQ(src);

// najdenie princezien a teleportov
    int index=0, character;
    TELEPORT **node = NULL;
    for(y=0; y<height; y++)
    {
        for(x=0; x<width; x++)
        {
            character = map[y][x]-'0';
            if(map[y][x]==FIRST_PRINCESS)//princezna
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
VERTEX* relaxVertex(VERTEX* from, int y, int x, int g, int d, int p, int q, int r, VERTEX tab[2][2][2][2][2][height][width])
{
    if(y>=height || y<0 || x>=width || x<0
        || tab[g][d][p][q][r][y][x].isVisited == TRUE || map[y][x]==NO_PATH)//no go
    {
        return NULL;
    }
//Rozhodovanie popolvara
    if(map[y][x] == GENERATOR) g = TRUE;
    else if(d == TRUE && map[y][x] == FIRST_PRINCESS) p = TRUE;
    else if(d == TRUE && map[y][x] == SECOND_PRINCESS) q = TRUE;
    else if(d == TRUE && map[y][x] == THIRD_PRINCESS) r = TRUE;
//Relaxacia
    int newTime = from->time + getTime(map[y][x]);
    if(newTime < tab[g][d][p][q][r][y][x].time)
    {
        tab[g][d][p][q][r][y][x].time = newTime;
        tab[g][d][p][q][r][y][x].previous = from;
        QUEUE_NODE neigh;
        neigh.x = x;
        neigh.y = y;
        neigh.g = g;
        neigh.d = d;
        neigh.p = p;
        neigh.q = q;
        neigh.r = r;
        neigh.priority = newTime;
        neigh.teleported = FALSE;
        pushQ(neigh);
    }
    if(d == TRUE && p == TRUE && q == TRUE && r == TRUE)
    {
        return &tab[g][d][p][q][r][y][x];
    }
    return NULL;
}
/** Vyriesenie teleportov*/
void visitTeleports(VERTEX* from, QUEUE_NODE node, VERTEX tab[2][2][2][2][2][height][width])
{
    int index = map[node.y][node.x] - '0';
    TELEPORT *tele;

    tele = teleports[index];
    while(tele!=NULL)
    {
        node.x = tele->x;
        node.y = tele->y;
        node.teleported = TRUE;
        if(tab[node.g][node.d][node.p][node.q][node.r][node.y][node.x].isVisited == FALSE)
        {
            tab[node.g][node.d][node.p][node.q][node.r][node.y][node.x].time = node.priority;
            tab[node.g][node.d][node.p][node.q][node.r][node.y][node.x].previous = from;
            pushQ(node);
        }
        tele = tele->next;
    }
}
/** Vygeneruje cestu*/
int *generatePath(VERTEX *final, int *pathSize)
{
    int i,*path;
    VERTEX *v = final;
    *pathSize = 0;
    //printf("TIME: %d\n",final->time);
    while(v!=NULL)
    {
        (*pathSize)++;
        v = v->previous;
    }
    path = ((int*)malloc(2*(*pathSize)*sizeof(int)));
    for(i=(*pathSize)-1; i>=0; --i){
        path[i*2] = final->x;
        path[i*2+1] = final->y;
        final = final->previous;
    }
    return path;
}
/**Navstivenie vrcholu*/
int * visitVertex(QUEUE_NODE min, VERTEX tab[2][2][2][2][2][height][width], int *pathSize)
{
    int x = min.x;
    int y = min.y;
    int g = min.g;
    int d = min.d;
    int p = min.p;
    int q = min.q;
    int r = min.r;
    if(tab[g][d][p][q][r][y][x].isVisited == TRUE)  //je uz navstiveny
    {
        return NULL;
    }
    tab[g][d][p][q][r][y][x].isVisited = TRUE; // NAVSTIVENE POLICKO

    int i,shift[]={-1,1};
    VERTEX *final, *from = &tab[g][d][p][q][r][y][x];
    if(g == TRUE && min.teleported == FALSE && map[y][x]>='0' && map[y][x]<='9')//NASIEL SOM TELEPORT
    {
        visitTeleports(from,min,tab);
    }
    else if(map[y][x] == DRAGON && d == FALSE)//NASIEL SOM DRAKA
    {
        //queueCount = 0;
        d = TRUE;
    }
    //relaxacia susedov
    for(i=0; i<2; i++)
    {
        final = relaxVertex(from, y+shift[i],x,g,d,p,q,r,tab);
        if(final != NULL) return generatePath(final, pathSize);
        final = relaxVertex(from, y,x+shift[i],g,d,p,q,r,tab);
        if(final != NULL) return generatePath(final, pathSize);
    }
    return NULL;
}
/** Uvolnenie globalnych premennych*/
void terminate(VERTEX tab[2][2][2][2][2][height][width])
{
    free(queue);//rad
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
            free(del);//teleport
        }
    }
}
/** Dijkstrov algoritmus s prioritnym radom*/
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)
{
    width = m;
    height = n;
    map = mapa;

    QUEUE_NODE min;
    VERTEX tab[2][2][2][2][2][height][width];//stavovy priestor
    int *path = NULL;
    initialize(tab);

    while(path==NULL)
    {
        if(queueCount==0) break; //Popolvar nema riesenie
        min = popQ();
        path = visitVertex(min,tab,dlzka_cesty);
    }
    terminate(tab);
    return path;
}
//**TEST**
int randomize(int seed) {return (unsigned int)((seed * 1103515245 +12345) / 65536) % 139;}
int cmpfunc (const void * a, const void * b){return ( *(int*)a - *(int*)b );}
/** Otestovanie prioritneho radu*/
int testQ()
{
/** Otestovanie na danej mape*/
    int size=100;
    queue = (QUEUE_NODE*)calloc(sizeof(QUEUE_NODE),size);    //min halda
    queueCount=0;
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
int testMap(char **mapa, int n, int m, int t)
{
    int generatorWork = FALSE, teleported = FALSE;
    int i, x, y;
    int timePath=0, *dlzka_cesty;
    int *cesta = zachran_princezne(mapa, n, m, -1, &dlzka_cesty);

    if(cesta==NULL) return 1;
    for(i=0;i<dlzka_cesty;++i)
    {
        x = cesta[i*2];
        y = cesta[i*2+1];
        if(map[y][x]>='0' && map[y][x]<='9')
        {
             if(teleported)
             {
                 teleported = FALSE;
                 timePath-=getTime(map[y][x]);
             }
             else teleported = TRUE;
         }
         timePath+=getTime(map[y][x]);
    }
    if(t == timePath) return 0; //Je to optimalna cesta?
    //TLAC
    printf("[%d,%d] optim.time=%d / my.time=%d\n",n, m, t,timePath);
    int j;
    for(i=0;i<dlzka_cesty;++i)
    {
        printf("%d %d\n", cesta[i*2], cesta[i*2+1]);
        map[cesta[i*2+1]][cesta[i*2]] = '.';
    }
    for(i=0; i<height; i++)
    {
        for(j=0; j<width; j++)
            printf("%c ",map[i][j]);

        printf("\n");
    }
    printf("\n");
    return 1;
}
/** Otestovanie algritmu popolvara*/
int test()
{
    int COUNT = 3;
    int width, height;
    int time,i,j,c,index;
    FILE *fp = fopen("maps.txt","r");

    for(index=0; index<COUNT; index++)
    {
        //nacitaj
        fscanf(fp,"%d %d %d",&height, &width, &time);
        char map[height][width];
        fgetc(fp);
        for(i=0; i<height;i++)
            for(j=0; j<width; j++)
            {
                fscanf(fp,"%c ",&c);
                map[i][j] = c;
            }
        char * mapa[height];
        for(i=0; i<height;i++)
            mapa[i] = map[i];
        //otestuj
        if(testMap(mapa, height, width, time))
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}
/**Hlavna funkcia*/
int main()
{
    if(testQ())
        printf("Chybna implementacia prioritnej haldy!\n");
    else
        printf("Implementacia prioritnej haldy OK!\n");

    if(test())
        printf("Chybna implementacia algoritmu!\n");
    else
        printf("Implementacia algoritmu OK!\n");

    return 0;
}
