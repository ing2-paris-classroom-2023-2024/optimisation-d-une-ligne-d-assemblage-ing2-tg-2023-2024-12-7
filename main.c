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
