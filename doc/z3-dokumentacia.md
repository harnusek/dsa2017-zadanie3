# Zadanie 3 - Dokumentacia - Ondrej Harnusek, ?27.?8.?2017
---

## 1. Pouzite datove struktury
##### Prioritny rad pre Dijkstru- QUEUE_NODE *queue;
* QUEUE_NODE:
    * unsigned int x, y;        //suradnice
    * unsigned int g;           //generator = 2 stavy, t.j nasiel alebo nenasiel
    * unsigned int d;           //drak = 2 stavy, t.j nasiel alebo nenasiel
    * unsigned int p;           //1. princezna = 2 stavy, t.j nasiel alebo nenasiel
    * unsigned int q;           //2. princezna = 2 stavy, t.j nasiel alebo nenasiel
    * unsigned int r;            //3. princezna = 2 stavy, t.j nasiel alebo nenasiel
    * unsigned int teleported;  //bol teleportovany = 2 stavy
    * unsigned int priority;    //cas do toto vrchola

##### Matica vrcholov grafu pre Dijkstru- VERTEX tab[2][2][2][2][2][height][width];
* VERTEX:
    * unsigned int x, y;            //suradnice
    * unsigned int isVisited;       //2 stavy
    * unsigned int time;            //cas do vrcholu
    * struct vertex* previous;      //vrchol odkial prisiel
    
##### Pole odkazov na spajany zoznam teleportov- TELEPORT *teleports[10];
* TELEPORT:
    * unsigned int x, y;        //suradnice
    * struct teleport* next;    //dalsi teleport
---

## 2. Opis pouziteho algoritmu
Stavovy priestor je reprezentovany maticov VERTEX tab[2][2][2][2][2][height][width])
* 2 stavy pre generator (s/bez)
* 2 stavy pre draka (nasiel/nenasiel)
* 2 stavy pre 1. princeznu (nasiel/nenasiel)
* 2 stavy pre 2. princeznu (nasiel/nenasiel)
* 2 stavy pre 3. princeznu (nasiel/nenasiel)
**stavovy priestor su vsetky permutacie = 2^5 = 32 stavov**

**Dijkstrov algoritmus:**
v takto definovano stavovom priestore ho staci raz spustit 
1. inicializacia: 
    * prechod tabulkov: previous=NULL, time=TIME_MAX
    * prechod mapov, najdenie teleportov a princezien
    * vytvorenie binarnej haldy
2. vlozenie prveho vrchola do radu: tab[0][0][0][0][0][0][0]
3. Pokial nenajde vsetky princezne vybera z haldy vrcholy a relaxuje ich susedov
    * relaxacia: aktualizacia casu suseda, ak cas z vrcholu do suseda je mensi ako jeho povodny
    * ak vyberie generator alebo draka nastavi priznak na TRUE
    * ak vyberie princeznu a vyberal uz draka nastavi priznak na TRUE
    * ak vyberie teleport, vlozi do haldy vsetky vrcholy s danym teleportom, s rovnakym casom
    * finalny stav su vsetky princezny==TRUE
4.  Prehladava rekurzivne predchadzajuce stavy finalneho stavu a vytvara cestu
---

## 3. Odhad priestorovej zlozitosti
 stavovy priestor =  32 stavov
 
1. Prioritny rad pre Dijkstru= 8 bajtov * (32 * vyska_mapy * sirka_mapy)
2. Matica vsetkych stavou= 8 bajtov * (32 * vyska_mapy * sirka_mapy)
3. Pole odkazov na spajany zoznam teleportov= 8 bajtov * (pocet_teleportov)

**celkova zlozitost = O((64 * vyska_mapy * sirka_mapy) + pocet_teleportov)**

---

## 4. Odhad casovej zlozitosti
* Inicializacia
    * prechod tabulkov = O(32 * vyska_mapy * sirka_mapy)
    * prechod mapov, najdenie teleportov = O(vyska_mapy * sirka_mapy)
* Prioritny rad je implementovany ako binarna halda s vyskov log(n)
    * operacie pop() a push(x) maju preto zlozitost = O(log n)
* Dijkstrov algoritmus v najhorsom prípade vlozi do priorineho radu a vyberie z neho všetky vrcholy grafu.
    * zlozitost O(log n * vyska_mapy * sirka_mapy)

**celkova zlozitost = O((64 * vyska_mapy * sirka_mapy) + pocet_teleportov)**
  
---

## 5. Testovac

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

5 15 23
C C C C D 1 H H H H H H H N N
H N N N 0 N N N N N N N H N 0
H N N N N N N N N N N N H N P
H N N N N N N N N N N N H 1 P
G N N N N N N N N N N N N N P

7 7 17
D C C C C H P
N C N N N N C
0 C N 0 0 N C
C C N 0 0 N C
C C N N N N C
C P C C C C P
C C C C C C C

9 6 18
C N N N N N
C 3 N 0 D N
C 2 N N N N
C 1 N 1 P N
C 0 N N N N
G N N 2 P N
N N N N N N
N N N 3 P N
N N N N N N

vysku a sirku generuje opacne
