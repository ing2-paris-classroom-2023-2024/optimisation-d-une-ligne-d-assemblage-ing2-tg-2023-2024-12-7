#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Operation
{
    int num;
    struct Operation *exclu[10];
    int nmbEx;
    struct Operation *prec[10];
    int nmbPr;
    float temps;
    int nmbAnt;
    int couleurWelsh;
    int triTopo;
};


struct Station
{
    float temps;
    struct Operation Op[50];
    int nmbOp;
    int couleur;
};

struct Operation newOp(int _num)
{
    struct Operation New;
    New.num = _num;
    New.nmbEx = 0;
    New.temps = 0;
    New.nmbPr = 0;
    New.nmbAnt = 0;
    New.couleurWelsh = -1;

    return New;
}

struct Station NewStation()
{
    struct Station New;
    New.temps = 0;
    New.nmbOp = 0;
    New.couleur = 0;

    return New;
}

void CompterAnt(struct Operation AllOp[100],int nmb)
{
    for(int i = 0 ; i < nmb ; i++)
    {
        for(int j = 0 ; j < nmb ; j++)
        {
            for(int l = 0 ; l < AllOp[j].nmbPr ; l++)
            {
                if(AllOp[j].prec[l]->num == AllOp[i].num)
                {
                    AllOp[i].nmbAnt++;
                }
            }
        }
    }
}
void ajouterOperation(struct Station * _station,struct Operation _operation)
{
    _station->temps += _operation.temps;
    _station->Op[_station->nmbOp] = _operation;
    _station->nmbOp++;
}

void afficherStation(struct Station  _station)
{
    printf("Liste des operations: ");
    for(int i = 0 ; i < _station.nmbOp ; i++)
    {
        printf("%d ",_station.Op[i].num);
    }
    printf("\nTemps total d'execution: %f\n",_station.temps);
}

int existe(struct Operation AllOp[100],int nmb, int num)
{
    for(int i = 0 ; i <nmb ; i++)
    {
        if(AllOp[i].num == num)
            return i;
    }
    return 200;
}


void chargementDonnes(struct Operation AllOp[100],int *nmb,float *tempsCycle)
{
    /// RECUPERATION DU TEMPS DE CYCLE

    FILE* fic = fopen("../temps_cycle.txt", "r");

    if (!fic)
    {
        fprintf(stderr, "erreur temps_cycle.txt\n");
        exit(EXIT_FAILURE);
    }

    fscanf(fic, "%f",tempsCycle);
    fclose(fic);

    /// RECUPERATION DU TEMPS DES OPERATIONS

    fic = fopen("../operations.txt", "r");

    if (!fic)
    {
        fprintf(stderr, "erreur operations.txt\n");
        exit(EXIT_FAILURE);
    }

    int num;
    float tmp;
    while (fscanf(fic, "%d %f", &num, &tmp) != EOF)
    {
        int i = existe(AllOp,*nmb,num);
        if(i != 200)
        {
            AllOp[i].temps = tmp;
        }
        else
        {
            AllOp[(*nmb)] = newOp(num);
            AllOp[(*nmb)].temps = tmp;
            (*nmb)++;
        }
    }
    fclose(fic);

    /// RECUPERATION DES EXCLUSIONS

    fic = fopen("../exclusions.txt", "r");

    if (!fic)
    {
        fprintf(stderr, "Impossible d'ouvrir exclusions.txt\n");
        exit(EXIT_FAILURE);
    }

    int n1, n2;
    while (fscanf(fic, "%d %d", &n1, &n2) != EOF)
    {
        int i = existe(AllOp,*nmb,n1);
        if(i == 200)
        {
            AllOp[(*nmb)] = newOp(n1);
            (*nmb)++;
        }
        i = existe(AllOp,*nmb,n2);
        if(i == 200)
        {
            AllOp[(*nmb)] = newOp(n2);
            (*nmb)++;
        }

        int i1 = existe(AllOp,*nmb,n1);
        int i2 = existe(AllOp,*nmb,n2);
        AllOp[i1].exclu[AllOp[i1].nmbEx] = &AllOp[i2];
        AllOp[i1].nmbEx++;
        AllOp[i2].exclu[AllOp[i2].nmbEx] = &AllOp[i1];
        AllOp[i2].nmbEx++;

    }

    fclose(fic);

    /// RECUPERATION DES PRECEDENCES

    fic = fopen("../precedences.txt", "r");

    if (!fic)
    {
        fprintf(stderr, "erreur precedences.txt\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fic,"%d %d", &n1, &n2) != EOF)
    {
        int i = existe(AllOp,*nmb,n1);
        if(i == 200)
        {
            AllOp[(*nmb)] = newOp(n1);
            (*nmb)++;
        }
        i = existe(AllOp,*nmb,n2);
        if(i == 200)
        {
            AllOp[(*nmb)] = newOp(n2);
            (*nmb)++;
        }

        int i1 = existe(AllOp,*nmb,n1);
        int i2 = existe(AllOp,*nmb,n2);
        AllOp[i1].prec[AllOp[i1].nmbPr] = &AllOp[i2];
        AllOp[i1].nmbPr++;

    }

    fclose(fic);

}

void voirLesDonnees(struct Operation AllOp[100],int nmb,float tempsCycle)
{
    int choix=0;
    while(choix!=5)
    {
        printf("\n\nVOIR LES DONNEES\n");
        printf("\n1-Voir les donnees d'exclusion");
        printf("\n2-Voir les donnees de precedence");
        printf("\n3-Voir les temps d'opeation");
        printf("\n4-Voir le temps de cycle");
        printf("\n5-Retour au menu\n");

        scanf("%d",&choix);

        printf("\n");

        if(choix == 1)
        {
            for(int i = 0 ; i< nmb ; i++)
            {
                printf("\n%d exclusions: ",AllOp[i].num);
                for(int j = 0 ; j < AllOp[i].nmbEx ; j++)
                {
                    printf("%d ",AllOp[i].exclu[j]->num);
                }
            }
        }
        else if (choix == 2)
        {
            for(int i = 0 ; i< nmb ; i++)
            {
                printf("\n%d suivant: ",AllOp[i].num);
                for(int j = 0 ; j < AllOp[i].nmbPr ; j++)
                {
                    printf("%d ",AllOp[i].prec[j]->num);
                }
            }
        }
        else if( choix == 3)
        {
            for(int i = 0 ; i< nmb ; i++)
            {
                printf("%d temps : %.2fs\n",AllOp[i].num,AllOp[i].temps);
            }
        }
        else if( choix == 4)
        {
            printf("temps cycle : %.2fs\n",tempsCycle);
        }
        else if( choix == 5)
        {

        }

    }
}

int trierDegres(const void *a, const void *b)
{
    return ((struct Operation *)b)->nmbEx - ((struct Operation *)a)->nmbEx;
}



int coloration(int couleur[100],struct Operation AllOp[100],int nmb)
{
    for (int i = 0; i < nmb; i++)
    {
        couleur[i] = 0;
        AllOp[i].couleurWelsh = -1;
    }

    int couleurAct = 1;
    int cmp = 0;
    while(cmp != nmb)
    {
        for(int i = 0 ; i< nmb ; i++)
        {
            if(couleur[i] == 0)
            {

                int test = 0;
                for(int j = 0 ; j < AllOp[i].nmbEx ; j++)
                {

                    if(couleur[existe(AllOp,nmb,AllOp[i].exclu[j]->num)]  == couleurAct)
                        test = 1;
                }

                if(test == 0)
                {
                    couleur[i] = couleurAct;
                    cmp++;
                }


            }
        }
        couleurAct++;
    }

    return couleurAct;

}
