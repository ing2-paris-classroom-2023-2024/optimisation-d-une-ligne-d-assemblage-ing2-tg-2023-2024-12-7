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
