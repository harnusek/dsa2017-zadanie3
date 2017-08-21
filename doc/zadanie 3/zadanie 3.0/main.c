// zadanie3.c -- Zadanie 3 - Popolvar
// Ondrej Harnusek, 28.11.2016 15:16:59

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define TIME_MAX 2147483646
typedef struct vertex                   // zaznam o vrchole v grafe
{
    unsigned int x:10;
    unsigned int y:10;
    unsigned int generator:1;
    unsigned int dragon:1;
    unsigned int princess_1:1;
    unsigned int princess_2:1;
    unsigned int princess_3:1;
}VERTEX;

typedef struct tabNode                  // zaznam o predchadzajucom policku
{
    unsigned int time;
    VERTEX from;
}TAB_NODE;

typedef struct pqNode                   // zaznam v prioritnom rade
{
    VERTEX state;
    unsigned int time;
    //VERTEX prev;
}PQ_NODE;

int width, height;                      // rozmery mapy
char **map;                             // mapa
TAB_NODE *******tab;                    // tabulka vsetkych stavov
int done;
int sizePQ;                             // velkost prioritneho radu
PQ_NODE *queue;                         // priority rad

void put(VERTEX state, int time)         // vlozi zaznam do prioritneho radu
{
    int i,par_i;
    PQ_NODE temp;
  	sizePQ++;
    queue[sizePQ].state = state;
    queue[sizePQ].time = time;
    //korekcie
    i=sizePQ;        //nova
    par_i=(sizePQ/2);//jej rodic
    while(par_i > 0 && queue[par_i].time > time)
    {
        temp=queue[i];
        queue[i]=queue[par_i];
        queue[par_i]=temp;

        i=par_i;        //++
        par_i/=2;       //++
    }
}

void heapify(int parent)                // zoradi prioritny rad
{
    int bigger;
    int left = 2*parent;
    int right = left+1;
    PQ_NODE temp;

    if(left<=sizePQ && queue[left].time < queue[parent].time)
        bigger=left;
    else
        bigger=parent;
    if(right<=sizePQ && queue[right].time < queue[bigger].time)
        bigger=right;
    if(bigger!=parent)
    {
        temp=queue[parent];
        queue[parent]=queue[bigger];
        queue[bigger]=temp;
        heapify(bigger);
    }
}

void delMin()                           // vymaze minimum z radu
{
    queue[1] = queue[sizePQ];
	sizePQ--;                                   // odstranenie
	heapify(1);                                 // korekcie
}

void printMap()                         // VYTLACI MAPU
{
    int i,j;

    printf("width=%d height=%d\n",width, height);
    for(i=0; i<height; i++)
    {
        for(j=0; j<width; j++)
            printf("%c ",map[i][j]);

        printf("\n");
    }
}

void printTab()                         // VYTLACI TABULKU
{
    int x,y,g,d,p1,p2,p3;

    for(y=0; y<height; y++)
    {// vyska
        for(x=0; x<width; x++)
        {// sirka
            for(g=0; g<2; g++)
            {// generator
                for(d=0; d<2; d++)
                {// drak
                    for(p1=0; p1<2; p1++)
                    {// 1.princezna
                        for(p2=0; p2<2; p2++)
                        {// 2.princezna
                            for(p3=0; p3<2; p3++)
                                (tab[y][x][g][d][p1][p2][p3].time == TIME_MAX)? printf("x ") : printf("%d ",tab[y][x][g][d][p1][p2][p3].time );
                        }
                    }
                }
            }printf("\n");
        }
        printf("\n");
    }
}

void printSimpleTab()                   // VYTLACI TABULKU
{
    int x,y;

    for(y=0; y<height; y++)
    {// vyska
        for(x=0; x<width; x++)
        {// sirka
            (tab[y][x][0][0][0][0][0].time == TIME_MAX)? printf(" x ") : printf("%2d ",tab[y][x][0][0][0][0][0].time );
        }
        printf("\n");
    }
}

void printHelp()                        // VYTLACI TABULKU
{
    int x,y;

    for(y=0; y<height; y++)
    {// vyska
        for(x=0; x<width; x++)
        {// sirka
            printf("[%2d,%2d] ",tab[y][x][0][0][0][0][0].from.y, tab[y][x][0][0][0][0][0].from.x);
        }
        printf("\n");
    }
}

int getTime(char box)                   // vrati cas prechodu daneho policka
{
    if(box == 'C' || box == 'D' || box == 'G' || box == 'P')
        return 1;
    else if(box == 'H')
        return 2;
    else
        return -1;
}

void setVertex(VERTEX *status, int x, int y, int g, int d, int p1, int p2, int p3)      // nastavi premenne v strukture
{
    (*status).x = x;
    (*status).y = y;
    (*status).generator = g;
    (*status).dragon = d;
    (*status).princess_1 = p1;
    (*status).princess_2 = p2;
    (*status).princess_3 = p3;

}

void init(char **mapa, int n, int m)    // inicializacia globalnych premennych
{
    width = m;                  // sirka
    height = n;                 // vyska
    map = mapa;                 // mapa
    int x,y,g,d,p1,p2,p3;
    // ALOKACIA PAMATE
// tabulka vsetkych stavov
    //TAB_NODE tab[n][m][2][2][2][2][2];
    tab = (TAB_NODE*******) malloc(n * sizeof(TAB_NODE******));

    for(y=0; y<n; y++)
    {
        tab[y] = (TAB_NODE******) malloc(m * sizeof(TAB_NODE *****));                           // vyska
        for(x=0; x<m; x++)
        {
            tab[y][x] = (TAB_NODE*****) malloc(2 * sizeof(TAB_NODE ****));                      // sirka
            for(g=0; g<2; g++)
            {
                tab[y][x][g] = (TAB_NODE****) malloc(2 * sizeof(TAB_NODE ***));                 // generator
                for(d=0; d<2; d++)
                {
                    tab[y][x][g][d] = (TAB_NODE***) malloc(2 * sizeof(TAB_NODE **));            // drak
                    for(p1=0; p1<2; p1++)
                    {
                        tab[y][x][g][d][p1] = (TAB_NODE**) malloc(2 * sizeof(TAB_NODE *));      // 1.princezna
                        for(p2=0; p2<2; p2++)
                        {
                            tab[y][x][g][d][p1][p2] = (TAB_NODE*) malloc(2 * sizeof(TAB_NODE)); // 2.princezna
                            for(p3=0; p3<2; p3++)
                                tab[y][x][g][d][p1][p2][p3].time = TIME_MAX;                    // inicializacia
                        }
                    }
                }
            }
        }
    }

// prioritna rada
    sizePQ = 0;
    queue = (PQ_NODE*)malloc(1000*sizeof(PQ_NODE));

    VERTEX status;
    tab[0][0][FALSE][FALSE][FALSE][FALSE][FALSE].time = getTime(map[0][0]);
    setVertex(&status, 0, 0, FALSE, FALSE, FALSE, FALSE, FALSE);
    tab[0][0][FALSE][FALSE][FALSE][FALSE][FALSE].from = status;
    put(status,getTime(map[0][0]));
}

void relax(int y, int x, int g, int d, int p1, int p2, int p3)                          // relaxacia susedov
{
    int newTime = queue[1].time + getTime(map[y][x]);
    VERTEX neigh;
    setVertex(&neigh, x, y, g, d, p1, p2, p3);

    //printf("[%d,%d] = %d\n",y,x,newTime);
    //printf("%d\n",tab[y][x][g][d][p1][p2][p3].time);
    if(newTime < tab[y][x][g][d][p1][p2][p3].time)
    {
        tab[y][x][g][d][p1][p2][p3].time = newTime;
        tab[y][x][g][d][p1][p2][p3].from = queue[1].state;
        //printf("[%d,%d]->[%d,%d]\n",queue[1].state.y,queue[1].state.x,y,x);
        /*if(map[y][x]=='D')
            printf("\n");*/
        put(neigh, newTime);
    }
}

int * getPath(int y, int x, int g, int d, int p1, int p2, int p3, int *dlzka_cesty)                        // vypocita hladanu cestu
{
    int size = tab[y][x][g][d][p1][p2][p3].time;
    int count = size;
    int *path = (int *)malloc(2*size*sizeof(int));
    int i,j,k,l,m,n,o;
    //printf("\t\t\t[%d, %d]\t%d%d%d%d%d\n", y,x,g,d,p1,p2,p3);
    //map[y][x] = '*';

    path[count*2] = x;
    path[count*2+1] = y;
    while(x!=0 || y!=0)
    {
        i = tab[y][x][g][d][p1][p2][p3].from.x;
        j = tab[y][x][g][d][p1][p2][p3].from.y;
        k = tab[y][x][g][d][p1][p2][p3].from.generator;
        l = tab[y][x][g][d][p1][p2][p3].from.dragon;
        m = tab[y][x][g][d][p1][p2][p3].from.princess_1;
        n = tab[y][x][g][d][p1][p2][p3].from.princess_2;
        o = tab[y][x][g][d][p1][p2][p3].from.princess_3;
        x = i;
        y = j;
        g = k;
        d = l;
        p1 = m;
        p2 = n;
        p3 = o;
        //printf("\t\t\t[%d, %d]\t%d%d%d%d%d\n", j,i,k,l,m,n,o);
        //map[y][x] = '*';
        count--;
        path[count*2] = x;
        path[count*2+1] = y;
    }

    *dlzka_cesty = size-count+1;
    return path+(count*2);
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)              // vrati najkratsiu cestu
{
    int x, y, g, d, p1, p2, p3;

// inicializacia
    init(mapa, n, m);

// relaxacia
    while(!done)
    {
        x = queue[1].state.x;
        y = queue[1].state.y;
        g = queue[1].state.generator;
        d = queue[1].state.dragon;
        p1 = queue[1].state.princess_1;
        p2 = queue[1].state.princess_2;
        p3 = queue[1].state.princess_3;

        if(map[y][x]=='D')
        {
            done=1;
            break;
        }

        //map[y][x]=' ';
        //printf("\nall %d \t\t\t\t[%d,%d]\n",tab[y][x][g][d][p1][p2][p3].time, y,x);

        if(y<height-1 && tab[y+1][x][g][d][p1][p2][p3].time == TIME_MAX && map[y+1][x]!='N')
        {
            relax(y+1,x,g,d,p1,p2,p3);
        }

        if(x<width-1 && tab[y][x+1][g][d][p1][p2][p3].time == TIME_MAX && map[y][x+1]!='N')
        {
            relax(y,x+1,g,d,p1,p2,p3);
        }

        if(y>0 && tab[y-1][x][g][d][p1][p2][p3].time == TIME_MAX && map[y-1][x]!='N')
        {
            relax(y-1,x,g,d,p1,p2,p3);
        }

        if(x>0 && tab[y][x-1][g][d][p1][p2][p3].time == TIME_MAX && map[y][x-1]!='N')
        {
            relax(y,x-1,g,d,p1,p2,p3);
        }

        delMin();
    }


    //printHelp();
    //printSimpleTab();
    printf("\n");
    printMap();
    printf("\n");


// vratenie cesty
    return getPath(y,x,g,d,p1,p2,p3,dlzka_cesty);;
}

int main()                              //TESTER
{
    int i, dlzka_cesty;

    int n=10, m=10, t=0;
    char map[10][10] = {
                        {'H', 'H', 'N', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'H', 'N', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'H', 'H', 'G', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'H', 'N', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'C', 'N', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'N', 'C', 'N', 'N', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'D', 'C', 'C', 'C', 'C', 'C', 'C'}
                        };
    char * mapa[10] = { map[0], map[1], map[2], map[3], map[4], map[5], map[6], map[7], map[8], map[9]};

    int *cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);

    if(cesta== NULL)
    {
        printf("\nCesta je NULL!\n");
        return 1;
    }

    for(i=0; i<dlzka_cesty; ++i)
        printf("%d %d\n", cesta[i*2], cesta[i*2+1]);

    return 0;
}
