// zadanie3.c -- Zadanie 3 - Popolvar
// Ondrej Harnusek, 28.11.2016 15:16:59

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0

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
    struct node* cameFrom;
}NODE;

int width, height;                      // rozmery mapy
char **map;                             // mapa
int sizePQ;                             // velkost prioritneho radu
NODE *queue;                            // priority rad

void printSimpleTab(NODE tab[2][2][2][2][height][width], int a, int b, int c, int d)                    // VYTLACI TABULKU
{
    int x,y;

    for(y=0; y<height; y++)
    {// vyska
        for(x=0; x<width; x++)
        {// sirka
            (tab[a][b][c][d][y][x].time == INT_MAX)? printf(">< ") : printf("%2d ",tab[a][b][c][d][y][x].time );
        }
        printf("\n");
    }
}
void printMap()                                                             // VYTLACI MAPU
{
    int i,j;

    printf("\nwidth=%d height=%d\n",width, height);
    for(i=0; i<height; i++)
    {
        for(j=0; j<width; j++)
            printf("%c ",map[i][j]);

        printf("\n");
    }
}

void init(char **mapa, int n, int m, NODE tab[2][2][2][2][n][m])            // inicializacia globalnych premennych
{
// nastavenie globalnych premennych
    width = m;                  // sirka
    height = n;                 // vyska
    map = mapa;                 // mapa

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
                        }
// alokacia pamate pre PQ
    sizePQ = 0;
    queue = (NODE*)malloc(16*m*n*sizeof(NODE));

// najdenie princezien a prepisanie
    p=0;
    for(y=0; y<height; y++)
        for(x=0; x<width; x++)
            if(map[y][x]=='P')
            {
                map[y][x]+=p;
                p++;
            }

}

int getTime(char box)                                                       // vrati cas prechodu daneho policka
{
    if(box == 'C' || box == 'D' || box == 'P' || box == 'Q' || box == 'R')
        return 1;
    else if(box >= '0' && box <= '9')
        return 1;
    else if(box == 'H')
        return 2;
    else
        return -1;
}

void relax(NODE* from, int y, int x, int d, int p, int q, int r, NODE tab[2][2][2][2][height][width])
{
    int newTime = from->time + getTime(map[y][x]);
    if(map[y][x] == 'D')
    {
        sizePQ = 1;
        d = TRUE;
    }
    else if(d == TRUE && map[y][x] == 'P')
    {
        //sizePQ = 0;
        p = TRUE;
    }
    else if(d == TRUE && map[y][x] == 'Q')
    {
        //sizePQ = 0;
        q = TRUE;
    }
    else if(d == TRUE && map[y][x] == 'R')
    {
        //sizePQ = 0;
        r = TRUE;
    }
    //printf("new time %d\n", newTime);
    //printf("relax: [%d,%d] with time '%d'\n", y ,x,tab[d][p][q][r][y][x].time );

    if(newTime < tab[d][p][q][r][y][x].time)
    {
        NODE neigh;
        neigh.x = x;
        neigh.y = y;
        neigh.d = d;
        neigh.p = p;
        neigh.q = q;
        neigh.r = r;
        neigh.cameFrom = from;
        neigh.time = newTime;
        //neigh.isVisited = TRUE;
        put(neigh);
        tab[d][p][q][r][y][x] = neigh;
    }
}

int * getPath(NODE *n, int *dlzka_cesty)                        // vypocita hladanu cestu
{
    //printf("[%2d,%2d]\n",n->y, n->x);
    map[n->y][n->x] = '*';
    while(n->x!=0 || n->y!=0 || n->d!=0 || n->p!=0 || n->q!=0 || n->r!=0 )
    {
        n=n->cameFrom;
        //printf("[%2d,%2d]\n",n->y, n->x);
        map[n->y][n->x] = '*';
    }

    /*int size = tab[y][x][g][d][p1][p2][p3].time;
    int count = size;
    int *path = (int *)malloc(2*size*sizeof(int));
    int i,j,k,l,m,n,o;

    map[y][x] = '*';

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

        map[y][x] = '*';
        count--;
        path[count*2] = x;
        path[count*2+1] = y;
    }

    *dlzka_cesty = size-count+1;
    return path+(count*2);*/
}

void dijkstra(NODE tab[2][2][2][2][height][width], int *dlzka_cesty)                          // upraveny dijkstrov algoritmus
{
    int done = 0;
    NODE src;
    src.x = src.y = src.d = src.p = src.q = src.r = 0;
    src.time = getTime(map[0][0]);
    src.cameFrom = &tab[FALSE][FALSE][FALSE][FALSE][0][0];
    tab[FALSE][FALSE][FALSE][FALSE][0][0] = src;
    put(src);

    NODE *curr;
    int x,y,d,p,q,r;
    while(done<400)
    {
        x = queue[1].x;
        y = queue[1].y;
        d = queue[1].d;
        p = queue[1].p;
        q = queue[1].q;
        r = queue[1].r;
       // printf("%d|%d|%d|%d|%d|%d\n",d,p,q,r,y,x);
        tab[d][p][q][r][y][x].isVisited = TRUE;
        curr = &tab[d][p][q][r][y][x];

        /*if(p == TRUE && q == TRUE && r == TRUE)
        {
            printf("end\n");
            map[y][x] = 'X';
            break;
        }*/

        //map[y][x] = ' ';


        // relaxacia susedov
        if(y<height-1 && tab[d][p][q][r][y+1][x].isVisited == FALSE && map[y+1][x]!='N')
            relax(curr, y+1,x,d,p,q,r, tab);

        if(y>0 && tab[d][p][q][r][y-1][x].isVisited == FALSE && map[y-1][x]!='N')
            relax(curr, y-1,x,d,p,q,r, tab);

        if(x<width-1 && tab[d][p][q][r][y][x+1].isVisited == FALSE && map[y][x+1]!='N')
            relax(curr, y,x+1,d,p,q,r, tab);

        if(x>0 && tab[d][p][q][r][y][x-1].isVisited == FALSE && map[y][x-1]!='N')
            relax(curr, y,x-1,d,p,q,r, tab);

        delMin();
        done ++;
    }
    getPath(curr, dlzka_cesty);
    printf("%d|%d|%d|%d|%d|%d\n",d,p,q,r,y,x);
    //printf("[%2d,%2d]\n",tab[d][p][q][r][y][x].y, tab[d][p][q][r][y][x].x);
}

void printHelp(NODE tab[2][2][2][2][height][width])                        // VYTLACI TABULKU
{
    int x,y;

    for(y=0; y<height; y++)
    {// vyska
        for(x=0; x<width; x++)
        {// sirka
            if(map[y][x]=='X')
                printf("[*%d,%d*] ",tab[0][0][0][0][y][x].cameFrom->y, tab[0][0][0][0][y][x].cameFrom->x);
            else if(tab[0][0][0][0][y][x].time!=INT_MAX)
                printf("[%2d,%2d] ",tab[0][0][0][0][y][x].cameFrom->y, tab[0][0][0][0][y][x].cameFrom->x);
            else
                printf("[     ] ");
                //printf("[%2d,%2d] ",tab[0][0][0][0][y][x].y, tab[0][0][0][0][y][x].x);
        }
        printf("\n");
    }
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)  // vrati najkratsiu cestu
{
    NODE tab[2][2][2][2][n][m];
    // inicializacia
    init(mapa, n, m, tab);
    printMap();
    dijkstra(tab, dlzka_cesty);

    printSimpleTab(tab, 0,0,0,0);
    printMap();
    //testPQ();
    free(queue);
    //printHelp(tab);
    return NULL;
}

int main()                                                                  //TESTER
{
    int i, dlzka_cesty;

    int n=10, m=15, t=0;
    char map[10][15] = {
                        {'C', 'C', 'C', 'H', 'H', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'C', 'C', 'H', 'H', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'C', 'C', 'H', 'H', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'C', 'C', 'H', 'H', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'1', 'C', '1', 'C', 'C', 'C', 'C', 'C', 'C', 'P', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'P', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'P', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'D'}
                        };
    char * mapa[10] = { map[0], map[1], map[2], map[3], map[4], map[5], map[6], map[7], map[8], map[9]};
    //printf("size = %dB\n",sizeof(NODE));
    int *cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);

    if(cesta == NULL)
    {
        printf("\ncesta je NULL\n");
        return 1;
    }

    for(i=0; i<dlzka_cesty; ++i)
        printf("%d %d\n", cesta[i*2], cesta[i*2+1]);


    return 0;
}
