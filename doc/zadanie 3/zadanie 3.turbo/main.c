// zadanie3.c -- Zadanie 3 - Popolvar
// Ondrej Harnusek, 28.11.2016 15:16:59

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0

typedef struct node                     // zaznam o vrchole v grafe
{
    unsigned int x:10;
    unsigned int y:10;
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

void put(NODE state)                    // vlozi zaznam do prioritneho radu
{
    int i,par_i;
    int time = state.time;
    NODE temp;
  	sizePQ++;
    queue[sizePQ] = state;

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
    NODE temp;

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

void printSimpleTab(NODE tab[2][2][2][2][height][width])                    // VYTLACI TABULKU
{
    int x,y;

    for(y=0; y<height; y++)
    {// vyska
        for(x=0; x<width; x++)
        {// sirka
            (tab[0][0][0][0][y][x].time == INT_MAX)? printf(">< ") : printf("%2d ",tab[0][0][0][0][y][x].time );
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
void testPQ()                                                               // VYPRAZNI A VYPISE PQ
{
    NODE temp;
    int c[] = {5,4,8,32,104,22,96,74,52,23};
    int i;
    for(i=0; i<10; i++)
    {
        temp.x = i;
        temp.y = i;
        temp.time = c[i];
        put(temp);
        printf("%d\n", c[i]);
    }
    //sizePQ = 0;//vyprazdnenie

    printf("\n%d\n\n", sizePQ);
    while(sizePQ)
    {
        printf("[%d,%d]=%d",queue[1].x, queue[1].y, queue[1].time);
        delMin();
        printf("        s=%d\n", sizePQ);
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
    if(box == 'C' || box == 'D' || box == 'G' || box == 'P')
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

    printf("new time %d\n", newTime);
    printf("relax: [%d,%d] with time '%d'\n", y ,x,tab[d][p][q][r][y][x].time );

    if(newTime < tab[d][p][q][r][y][x].time)
    {
        NODE neigh;
        neigh.x = x;
        neigh.y = y;
        neigh.d = d;
        neigh.p = p;
        neigh.q = q;
        neigh.r = r;
        if(map[y][x] == 'D') neigh.d = TRUE;
        else if(map[y][x] == 'P') neigh.p = TRUE;
        else if(map[y][x] == 'Q') neigh.q = TRUE;
        else if(map[y][x] == 'R') neigh.r = TRUE;
        neigh.cameFrom = from;
        neigh.time = newTime;
        neigh.isVisited = TRUE;
        put(neigh);
    }
}

void dijkstra(NODE tab[2][2][2][2][height][width])                          // upraveny dijkstrov algoritmus
{
    int done = 0;
    NODE src;
    src.x = src.y = src.d = src.p = src.q = src.r = 0;
    src.time = getTime(map[0][0]);
    src.cameFrom = &tab[FALSE][FALSE][FALSE][FALSE][0][0];
    put(src);

    int x,y,d,p,q,r;
    x = y = d = p = q = r = 0;
    //int from_x,from_y,from_d,from_p,from_q,from_r;
    while(done<200)
    {
        /*from_x = queue[1].cameFrom->x;
        from_y = queue[1].cameFrom->y;
        from_d = queue[1].cameFrom->d;
        from_p = queue[1].cameFrom->p;
        from_q = queue[1].cameFrom->q;
        from_r = queue[1].cameFrom->r;*/

        x = queue[1].x;
        y = queue[1].y;
        d = queue[1].d;
        p = queue[1].p;
        q = queue[1].q;
        r = queue[1].r;

        if(d == TRUE)
        {
            done = 9999999;
            map[y][x] = 'X';
            break;
        }


        tab[d][p][q][r][y][x] = queue[1];
        tab[d][p][q][r][y][x].isVisited = TRUE;
        map[y][x] = ' ';
        NODE *curr = &tab[d][p][q][r][y][x];

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
    //tab[FALSE][FALSE][FALSE][FALSE][0][0] = queue[1];
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)  // vrati najkratsiu cestu
{
    NODE tab[2][2][2][2][n][m];
    // inicializacia
    init(mapa, n, m, tab);

    dijkstra(tab);

    printSimpleTab(tab);
    printMap();
    //testPQ();
    free(queue);
    return NULL;
}

int main()                                                                  //TESTER
{
    int i, dlzka_cesty;

    int n=10, m=15, t=0;
    char map[10][15] = {
                        {'C', 'C', 'N', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'C', 'N', 'N', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'C', 'C', 'C', 'N', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'C', 'C', 'H', 'C', 'C', 'C', 'C', 'C', 'D', 'C', 'C', 'C', 'C', 'C'},
                        {'H', 'N', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'N', 'N', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'1', 'C', '1', 'C', 'C', 'C', 'C', 'C', 'C', 'P', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'P', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                        {'C', 'C', 'C', 'C', 'C', 'C', 'P', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'}
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
