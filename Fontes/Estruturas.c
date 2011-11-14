/********************************* Estruturas.c *********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Estruturas.h"

void initCabecalho (Cabecalho **c){
     *c = NULL;
}

void insertCabecalho (Cabecalho **c, Atributo a){
     Cabecalho *na;

     na = (Cabecalho*)malloc(sizeof(Cabecalho));
     if (!na)
     {
        printf("Erro de alocacao de memoria.\n");
        system("pause");
        exit(1);
     }
     if(!a.key){
        a.conta_ref=0;
     }
     na->atributo = a;
     if (!(*c))
     {
        *c = na;
        (*c)->next = *c;
        (*c)->prev = *c;

     }
     else
     {
        na->next = *c;
        na->prev = (*c)->prev;
        na->prev->next = na;
        (*c)->prev = na;
     }
}

int numAtr (Cabecalho *c)
{
    int n;
    Cabecalho *aux;

    n = 0;
    if (c)
    {
        aux = c;
        do
        {
            n++;
            aux = aux->next;
        }while (aux != c);
    }
    return n;
}

void natr (Cabecalho *c, int n, Atributo *atributo)
{
     int i;
     Cabecalho *aux;

     if (c)
     {
         i = 1;
         aux = c;
         do
         {
             if (i == n) *atributo = aux->atributo;
             i++;
             aux = aux->next;
         }while ((aux != c) && (i <= n));
     }
}

int poschv (Cabecalho *c)
{
    int p;
    Cabecalho *aux;

    if (c)
    {
        p = 1;
        aux = c;
        do
        {
            if (aux->atributo.key) return p;
            p++;
            aux = aux->next;
        }while (aux != c);
    }
    return 0;
}

int posord (Cabecalho *c)
{
    int p;
    Cabecalho *aux;

    if (c)
    {
        p = 1;
        aux = c;
        do
        {
            if (aux->atributo.ord) return p;
            p++;
            aux = aux->next;
        }while (aux != c);
    }
    return 0;
}

int posfkey (Cabecalho *c)
{
    int p;
    Cabecalho *aux;

    if (c)
    {
        p = 1;
        aux = c;
        do
        {
            if (aux->atributo.fkey) return p;
            p++;
            aux = aux->next;
        }while (aux != c);
    }
    return 0;
}

int posatr (char *atributo, Cabecalho *c)
{
    int p;
    Cabecalho *aux;

    if (c)
    {
        p = 1;
        aux = c;
        do
        {
            if (!strcasecmp(aux->atributo.nome, atributo)) return p;
            p++;
            aux = aux->next;
        }while (aux != c);
    }
    return 0;
}

void fmemCabecalho (Cabecalho **c)
{
     Cabecalho *aux;
     if (*c)
     {
        aux = (*c)->next;
        while (aux != (*c))
        {
              fmemAtributo(&(aux->atributo));
              aux = aux->next;
              free(aux->prev);
        }
        free(aux);
     }

}

/*---------------------------------------------------------------------*/

void initTupla (Tupla **t)
{
     *t = NULL;
}

void insertTupla (Tupla **t, char *val)
{
     Tupla *nv;

     nv = (Tupla*)malloc(sizeof(Tupla));
     if (!nv)
     {
        printf("Erro de alocacao de memoria.\n");
        system("pause");
        exit(1);
     }
     strcpy(nv->val, val);
     if (!(*t))
     {
        *t = nv;
        (*t)->next = *t;
        (*t)->prev = *t;
     }
     else
     {
        nv->next = *t;
        nv->prev = (*t)->prev;
        nv->prev->next = nv;
        (*t)->prev = nv;
     }
}

int numVal (Tupla *t)
{
    int n;
    Tupla *aux;

    n = 0;
    if (t)
    {
        aux = t;
        do
        {
            n++;
            aux = aux->next;
        }while (aux != t);
    }
    return n;
}

void nval (Tupla *t, int n, char *val)
{
     int i;
     Tupla *aux;

     val[0] = '\0';
     if (t)
     {
         i = 1;
         aux = t;
         do
         {
             if (i == n) strcpy(val, aux->val);
             i++;
             aux = aux->next;
         }while ((aux != t) && (i <= n));
     }
}

void fmemTupla (Tupla **t)
{
     Tupla *aux;

     if (*t)
     {
        aux = (*t)->next;
        while (aux != (*t))
        {
              aux = aux->next;
              free(aux->prev);
        }
        free(aux);
     }
}

/*---------------------------------------------------------------------*/

void initRelacao (Relacao **r)
{
     *r = NULL;
}

void insertRelacao (Relacao **r, Tupla *tupla)
{
     Relacao *nt;

     nt = (Relacao*)malloc(sizeof(Relacao));
     if (!nt)
     {
        printf("Erro de alocacao de memoria.\n");
        system("pause");
        exit(1);
     }
     nt->tupla = tupla;
     if (!(*r))
     {
        *r = nt;
        (*r)->next = *r;
        (*r)->prev = *r;
     }
     else
     {
        nt->next = *r;
        nt->prev = (*r)->prev;
        nt->prev->next = nt;
        (*r)->prev = nt;
     }
}

/* Passa uma string para seu equivalente c/ letras maiúsculas */
void strToUpper (char *str)
{
     int length, i;

     length = strlen(str);
     for(i=0; i<length; i++)
     {
        str[i] = toupper(str[i]);
     }
}

void strToLower (char *str)
{
     int length, i;

     length = strlen(str);
     for(i=0; i<length; i++)
     {
        str[i] = tolower(str[i]);
     }
}

void iordinsertRelacao (Relacao **r, Tupla *tupla, int n)
{
     Relacao *nt, *aux;
     char tupatr[100], tlatr[100];
     int itupatr, itlatr, inserted;

     nt = (Relacao*)malloc(sizeof(Relacao));
     if (!nt)
     {
        printf("Erro de alocacao de memoria.\n");
        system("pause");
        exit(1);
     }
     nt->tupla = tupla;
     if (!(*r))   /*Se a lista está vazia*/
     {
        *r = nt;
        (*r)->next = *r;
        (*r)->prev = *r;
     }
     else
     {
        nval(tupla, n, tupatr);
        itupatr = atoi(tupatr);
        nval((*r)->tupla, n, tlatr);
        itlatr = atoi(tlatr);
        if (itupatr <= itlatr)           /*Se a chave de ordenação da tupla a*/
        {                                /*ser inserida for menor que a da 1ª*/
           nt->next = *r;               /*tupla na tabela, insere no início.*/
           nt->prev = (*r)->prev;
           nt->prev->next = nt;
           (*r)->prev = nt;
           *r = (*r)->prev;
        }
        else                              /*Caso contrário, procura a posição*/
        {                                 /*correta p/ inserção.*/
           aux = *r;
           inserted = 0;
           do                            /*Enquanto não chegar ao fim da lista*/
           {                             /*e ainda não tiver inserido*/
              nval(aux->tupla, n, tlatr);
              itlatr = atoi(tlatr);
              if (itlatr < itupatr) aux = aux->next;
              else                      /*Se a chave de ordenação da tupla a*/
              {                         /*atual for menor que a da tupla a ser*/
                  nt->next = aux;       /*inserida, avança;*/
                  nt->prev = aux->prev; /*Caso contrário, insere entre a*/
                  nt->prev->next = nt;  /*tupla atual e a anterior.*/
                  aux->prev = nt;
                  inserted = 1;
              }
           }while ((aux != *r) && (!inserted));
           if (!inserted) insertRelacao(r, tupla);    /*Se chegar ao final sem ter*/
        }                                       /*inserido, insere no fim.*/
     }
}

void sordinsertRelacao (Relacao **r, Tupla *tupla, int n)
{
     Relacao *nt, *aux;
     char tupatr[100], tlatr[100];
     int inserted;

     nt = (Relacao*)malloc(sizeof(Relacao));
     if (!nt)
     {
        printf("Erro de alocacao de memoria.\n");
        system("pause");
        exit(1);
     }
     nt->tupla = tupla;
     if (!(*r))   /*Se a lista está vazia*/
     {
        *r = nt;
        (*r)->next = *r;
        (*r)->prev = *r;
     }
     else
     {
        nval(tupla, n, tupatr);
        strToUpper(tupatr);
        nval((*r)->tupla, n, tlatr);
        strToUpper(tlatr);
        if ((strcmp(tlatr, tupatr)) >= 0)/*Se a chave de ordenação da tupla a*/
        {                                /*ser inserida for menor que a da 1ª*/
           nt->next = *r;               /*tupla na tabela, insere no início.*/
           nt->prev = (*r)->prev;
           nt->prev->next = nt;
           (*r)->prev = nt;
           *r = (*r)->prev;
        }
        else                              /*Caso contrário, procura a posição*/
        {                                 /*correta p/ inserção.*/
           aux = *r;
           inserted = 0;
           do                            /*Enquanto não chegar ao fim da lista*/
           {                             /*e ainda não tiver inserido*/
              nval(aux->tupla, n, tlatr);
              strToUpper(tlatr);
              if ((strcmp(tlatr, tupatr)) < 0) aux = aux->next;
              else                      /*Se a chave de ordenação da tupla a*/
              {                         /*atual for menor que a da tupla a ser*/
                  nt->next = aux;       /*inserida, avança;*/
                  nt->prev = aux->prev; /*Caso contrário, insere entre a*/
                  nt->prev->next = nt;  /*tupla atual e a anterior.*/
                  aux->prev = nt;
                  inserted = 1;
              }
           }while ((aux != *r) && (!inserted));
           if (!inserted) insertRelacao(r, tupla);    /*Se chegar ao final sem ter*/
        }                                       /*inserido, insere no fim.*/
     }
}

void fmemRelacao (Relacao **r)
{
     Relacao *aux;

     if (*r)
     {
        aux = (*r)->next;
        while (aux != (*r))
        {
              aux = aux->next;
              fmemTupla(&aux->prev->tupla);
              free(aux->prev);
        }
        free(aux);
     }
}

void fmemAtributo (Atributo *att)
{
    register int i;
    if(att->key && att->conta_ref>0){
         Referenciada *ref = att->f_referenciada;
         while((ref=ref->next)!=att->f_referenciada){
              free(ref);
         }
         free(att->f_referenciada);
         att->conta_ref = 0;
    }
}
