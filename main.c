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
void C1(struct Operation AllOp[100],int nmb)
{

    int couleur[100];

    int cmp = 0;

    int nmbCouleur = coloration(couleur,AllOp,nmb);

    struct Station AllStation[15];
    for (int i = 0 ; i < 15 ; i++)
    {
        AllStation[i] = NewStation();

    }




    for(int i = 1 ; i < nmbCouleur ; i++ )
    {
        for(int j = 0 ; j < nmb ; j++)
        {
            if(couleur[j] == i)
            {
                ajouterOperation(&AllStation[i-1],AllOp[j]);
            }
        }

    }


    for(int i = 0 ; i < nmbCouleur-1 ; i++ )
    {
        printf("\nSTATION %d\n",i+1);
        afficherStation(AllStation[i]);
    }


}


void triTopologique(int ordre[100],struct Operation AllOp[100],int nmb)
{

    int decouvertBy[100];
    int couleur[100];
    for(int i = 0 ; i< nmb ; i++)
    {
        couleur[i]=0;
        ordre[i] = -1;
        decouvertBy[i] = -1;
    }

    int cmp = 0;
    while(cmp != nmb)
    {
        int jeton = rand()%nmb;
        int depart = jeton;

        int out = 0;
        while(out == 0 && couleur[jeton] != 2)
        {
            couleur[jeton] = 1;
            int find = 0;
            do
            {
                find = 0;
                for(int i = 0 ; i < AllOp[jeton].nmbPr ; i++)
                {
                    if(couleur[existe(AllOp,nmb,AllOp[jeton].prec[i]->num)] == 0)
                    {
                        decouvertBy[existe(AllOp,nmb,AllOp[jeton].prec[i]->num)] = jeton;
                        jeton = existe(AllOp,nmb,AllOp[jeton].prec[i]->num);
                        couleur[jeton] = 1;
                        find = 1;
                        break;
                    }
                }
            }
            while(find == 1);

            couleur[jeton] = 2;
            ordre[nmb-cmp] = jeton;
            cmp++;

            if(jeton == depart)
            {
                out = 1;
            }
            else
            {
                jeton = decouvertBy[jeton];
            }
        }
    }

}

void C2(struct Operation AllOp[100],int nmb,float tempsCycle)
{
    int ordre[100];
    triTopologique(ordre,AllOp,nmb);

    int cmp;
    int nmbStation = 0;
    struct Station AllStation[15];
    for (int i = 0 ; i < 15 ; i++)
    {
        AllStation[i] = NewStation();

    }



    for(int i = 1 ; i < nmb+1 ; i++)
    {
        if(AllStation[nmbStation].temps + AllOp[ordre[i]].temps < tempsCycle)
        {
            ajouterOperation(&AllStation[nmbStation],AllOp[ordre[i]]);
        }
        else
        {
            nmbStation++;
            NewStation(AllStation[nmbStation]);
            ajouterOperation(&AllStation[nmbStation],AllOp[ordre[i]]);
        }
    }

    for(int i = 0 ; i < nmbStation +1 ; i++ )
    {
        printf("\nSTATION %d\n",i+1);
        afficherStation(AllStation[i]);
    }

}
void C3(struct Operation AllOp[100],int nmb,float tempsCycle)
{
    int couleur[100];

    int nmbCouleur = coloration(couleur,AllOp,nmb);

    struct Station AllStation[100];
    for (int i = 0 ; i < 15 ; i++)
    {
        AllStation[i] = NewStation();

    }

    int ajout =0;

    for(int i = 1 ; i < nmbCouleur ; i++ )
    {
        for(int j = 0 ; j < nmb ; j++)
        {
            if(couleur[j] == i)
            {
                if(AllStation[i-1+ajout].temps + AllOp[j].temps < tempsCycle)
                {
                    ajouterOperation(&AllStation[i-1+ajout],AllOp[j]);
                }
                else
                {
                    ajout++;
                    ajouterOperation(&AllStation[i-1+ajout],AllOp[j]);
                }
            }
        }

    }


    for(int i = 0 ; i < nmbCouleur-1+ajout ; i++ )
    {
        printf("\nSTATION %d\n",i+1);
        afficherStation(AllStation[i]);
    }

}

int DFS(struct Operation AllOp[100],int nmb,int D,int A,int couleur[100])
{
    int pile[100];
    int cmp = 0;
    pile[cmp] = D;

    do
    {

        int t = cmp;

        for(int i = 0 ; i < AllOp[pile[t]].nmbPr ; i++)
        {

            if(AllOp[pile[t]].prec[i]->num == AllOp[A].num)
            {

                return 0;
            }
            else
            {


                if(couleur[existe(AllOp,nmb,AllOp[pile[t]].prec[i]->num)] == 1)
                {
                    pile[cmp] = existe(AllOp,nmb,AllOp[pile[t]].prec[i]->num);
                    cmp++;
                }

            }

        }
        cmp--;


    }
    while(cmp >= 0);

    return 15;
}

void C4(struct Operation AllOp[100],int nmb)
{

    int couleur[100];
    int ordre[100];
    int decouvert[100];
    for(int i = 0 ; i< nmb ; i++)
        decouvert[i] = 0;


    int nmbCouleur = coloration(couleur,AllOp,nmb);
    triTopologique(ordre,AllOp,nmb);

    struct Station listeStation[15];
    for (int i = 0 ; i < 15 ; i++)
    {
        listeStation[i] = NewStation();

    }
    int nmbStation = 0;
    int cmp = 0;


    while(cmp < nmb)
    {

        for(int i = 1 ; i < nmb+1 ; i++)
        {
            //printf("Analyse %d\n",AllOp[ordre[i]].num);
            if(decouvert[ordre[i]] == 0)
            {
                //printf("couleur station: %d\n",listeStation[nmbStation].couleur);
                if(listeStation[nmbStation].couleur == couleur[ordre[i]] || listeStation[nmbStation].couleur == 0 )
                {
                    //printf("L'operation est bien de la bonne couleur\n");
                    if(AllOp[ordre[i]].nmbAnt == 0)
                    {
                        //printf("L'operation est une racine on l'ajoute a la station en cours\n");
                        ajouterOperation(&listeStation[nmbStation],AllOp[ordre[i]]);
                        decouvert[ordre[i]] = 1;
                        cmp++;
                        listeStation[nmbStation].couleur = couleur[ordre[i]];
                        //printf("%d est decouvert\n",AllOp[ordre[i]].num);
                        // system("pause");
                    }
                    else
                    {
                        //printf("on cherche si on peut acceder a la satation par des op deja decouverte\n");
                        int found = 15;
                        for(int j = 0 ; j < nmb ; j++)
                        {
                            if(decouvert[j] == 1)
                            {
                                //printf("Peut t'on aller de %d a %d\n",AllOp[j].num,AllOp[ordre[i]].num);
                                // system("pause");
                                if(DFS(AllOp,nmb,j,ordre[i],decouvert) == 0)
                                {
                                    found = 0;
                                    //printf("oui\n");
                                    break;
                                }
                                else
                                {
                                    //printf("non\n");

                                }

                            }
                        }
                        if(found == 0)
                        {

                            ajouterOperation(&listeStation[nmbStation],AllOp[ordre[i]]);
                            decouvert[ordre[i]] = 1;
                            cmp++;
                            listeStation[nmbStation].couleur = couleur[ordre[i]];
                            //printf("%d est decouvert",AllOp[ordre[i]].num);
                            // system("pause");
                        }
                    }

                }
            }
            else
            {
                // printf("%d est deja decouvert\n",AllOp[ordre[i]].num);
                // printf("cmp= %d\n",cmp);
            }

        }
        nmbStation++;
        NewStation(listeStation[nmbStation]);

    }

    for(int i = 0 ; i < nmbStation  ; i++ )
    {
        printf("\nSTATION %d\n",i+1);
        afficherStation(listeStation[i]);
    }

}

void C5(struct Operation AllOp[100],int nmb,float tempsCycle)
{

    int couleur[100];
    int ordre[100];
    int decouvert[100];
    for(int i = 0 ; i< nmb ; i++)
        decouvert[i] = 0;




    int nmbCouleur = coloration(couleur,AllOp,nmb);
    triTopologique(ordre,AllOp,nmb);


    struct Station listeStation[15];
    for (int i = 0 ; i < 15 ; i++)
    {
        listeStation[i] = NewStation();

    }

    int nmbStation = 0;
    int cmp = 0;


    while(cmp != nmb)
    {

        for(int i = 1 ; i < nmb+1 ; i++)
        {
            //printf("Analyse %d\n",AllOp[ordre[i]].num);

            if(decouvert[ordre[i]] == 0 && listeStation[nmbStation].temps + AllOp[ordre[i]].temps < tempsCycle)
            {
                //printf("couleur station: %d\n",listeStation[nmbStation].couleur);
                if(listeStation[nmbStation].couleur == couleur[ordre[i]] || listeStation[nmbStation].couleur == 0 )
                {

                    //printf("L'operation est bien de la bonne couleur\n");
                    if(AllOp[ordre[i]].nmbAnt == 0)
                    {
                        //printf("L'operation est une racine on l'ajoute a la station en cours\n");
                        ajouterOperation(&listeStation[nmbStation],AllOp[ordre[i]]);
                        decouvert[ordre[i]] = 1;
                        cmp++;
                        listeStation[nmbStation].couleur = couleur[ordre[i]];
                        //printf("%d est decouvert\n",AllOp[ordre[i]].num);
                        // system("pause");
                    }
                    else
                    {
                        //printf("on cherche si on peut acceder a la satation par des op deja decouverte\n");
                        int found = 15;
                        for(int j = 0 ; j < nmb ; j++)
                        {
                            if(decouvert[j] == 1)
                            {
                                //printf("Peut t'on aller de %d a %d\n",AllOp[j].num,AllOp[ordre[i]].num);
                                // system("pause");
                                if(DFS(AllOp,nmb,j,ordre[i],decouvert) == 0)
                                {
                                    found = 0;
                                    //printf("oui\n");
                                    break;
                                }
                                else
                                {
                                    // printf("non\n");

                                }

                            }
                        }
                        if(found == 0)
                        {

                            ajouterOperation(&listeStation[nmbStation],AllOp[ordre[i]]);
                            decouvert[ordre[i]] = 1;
                            cmp++;
                            listeStation[nmbStation].couleur = couleur[ordre[i]];
                            //printf("%d est decouvert",AllOp[ordre[i]].num);
                            // system("pause");
                        }
                    }

                }
            }
            else
            {
                // printf("%d est deja decouvert\n",AllOp[ordre[i]].num);
            }

        }
        nmbStation++;
        NewStation(listeStation[nmbStation]);

    }


    for(int i = 0 ; i < nmbStation  ; i++ )
    {
        printf("\nSTATION %d\n",i+1);
        afficherStation(listeStation[i]);
    }

}

void CalculerStation(struct Operation AllOp[100],int nmb,float tempsCycle)
{   int choix=0;

    while(choix!=6)

    {
        printf("\nCALCULER LES STATIONS\n");
        printf("\n1-Calculer en fonction des exclusions");
        printf("\n2-Calculer en fonction des precedences et du temps de cycle");
        printf("\n3-Calculer en fonction des exclusions et du temps de cycle");
        printf("\n4-Calculer en fonction des precedences et des exclusions");
        printf("\n5-Calculer final");
        printf("\n6-Retour au menu\n");


        scanf("%d",&choix);
        float tmp = tempsCycle;

        printf("\n");

        if(choix == 1)
        {
            C1(AllOp,nmb);
        }
        else if (choix == 2)
        {
            C2(AllOp,nmb,tmp);
        }
        else if( choix == 3)
        {
            C3(AllOp,nmb,tempsCycle);
        }
        else if( choix == 4)
        {
            C4(AllOp,nmb);
        }
        else if( choix == 5)
        {

            C5(AllOp,nmb,tempsCycle);
        }
        else if( choix == 6)
        {

        }

    }
}
