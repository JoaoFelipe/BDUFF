/********************************** sqlopr.c **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SQL.h"

int createTable(char *nome, Cabecalho *c)
{
    void strToUpper(char *str);
    /* Passa uma string para seu equivalente c/ letras mai�sculas.
     (Implementada em <Estruturas.c>) */

    char fname[20], fname2[20];
    FILE *ctlstream, *dadstream;
    Cabecalho *p;
    int erro, n;

    /* Vari�veis: fname     -> nome de arquivo.
     ctlstream -> stream do arquivo de meta-dados da rela��o.
     dadstream -> stream do arquivo de dados da rela��o.
     p         -> ponteiro utilizado p/ percorrer <al>.
     erro    -> c�digo de erro encontrado na lista de atributos.
     n         -> grau da rela��o. */

    strcpy(fname, nome);
    strcat(fname, ".ctl");
    if ((ctlstream = fopen(fname, "r")) != NULL)   /*Verifica se a tabela j� existe.*/
    {
        fclose(ctlstream);
        fmemCabecalho(&c);
        erroCT(TABELA_JA_EXISTE, nome);
        return 1;
    }

    if (c)
    {
        p = c;
        do /*Passa os nomes dos atributos p/ mai�sculas.*/
        {
            strToUpper(p->atributo.nome);
            p = p->next;
        }
        while (p != c);
    }
    erro = validaCabecalho(c); /*Verifica se h� erros na lista de atributos.*/

    if (erro)
    {
        fmemCabecalho(&c);
        if (erro != FK_TIPO_ERRADO && erro != FK_TABELA_INEXISTENTE)
            erroCT(erro, nome);
        return 1;
    }

    if ((ctlstream = fopen(fname, "w")) == NULL) /*Cria o arquivo .ctl*/
    {
        fmemCabecalho(&c);
        erroCT(CTL_NAO_CRIADO, nome);
        return 1;
    }

    strcpy(fname, nome);
    strcat(fname, ".dad");
    if ((dadstream = fopen(fname, "w")) == NULL)   /*Cria o arquivo .dad (vazio)*/
    {
        fmemCabecalho(&c);
        fclose(ctlstream);
        strcpy(fname, nome);
        strcat(fname, ".ctl");
        remove(fname);
        erroCT(DAD_NAO_CRIADO, nome);
        return 1;
    }
    fclose(dadstream);

    n = numAtr(c); /*Recupera o grau (n� de atributos) da rela��o.*/
    geraCTL(ctlstream, n, 0, c); /*Gera o arquivo .ctl*/
    fclose(ctlstream);

    p = c;
    FILE *addRef, *tmpRef;
    char linha[80], tabela[20];
    int conta_linha = 0, qnt_linha, x, tipo, k, achou, qnt_tabelas, y;
    do
    {
        conta_linha = 0;
        if (p->atributo.fkey)
        {
            strcpy(fname2, p->atributo.f_referencia);
            strcat(fname2, ".ctl");
            addRef = fopen(fname2, "r");
            tmpRef = fopen("tmpRef.tmp", "w");
            while (!feof(addRef))
            {
                if (conta_linha == 0)
                {
                    fscanf(addRef, "%d,%d\n", &qnt_linha, &x);
                    fprintf(tmpRef, "%d,%d\n", qnt_linha, x);

                }
                else if (conta_linha == (qnt_linha + 1))
                {
                    fscanf(addRef, "%[^\n]\n", linha);

                    achou = 0;

                    for (k = 0, x = 0; k < strlen(linha); k++)
                    {
                        if (linha[k] == ',')
                        {
                            tabela[x] = '\0';
                            x = 0;

                            if (strcasecmp(tabela, p->atributo.f_referencia)
                                    == 0)
                                achou = 1;
                            continue;
                        }
                        tabela[x++] = linha[k];
                    }
                    tabela[x] = '\0';

                    if (strcasecmp(tabela, p->atributo.f_referencia) == 0)
                        achou = 1;
                    if (!achou)
                    {
                        tabela[0] = ',';
                        tabela[1] = '\0';
                        strcat(tabela, nome);
                        strcat(linha, tabela);
                        strToUpper(linha);
                        fprintf(tmpRef, "%s\n", linha);
                        conta_linha = 0;
                        break;
                    }
                    else
                    {
                        fprintf(tmpRef, "%s\n", linha);
                    }

                }
                else
                {
                    fscanf(addRef, "%[^\n]\n", linha);
                    fprintf(tmpRef, "%s\n", linha);
                }
                conta_linha++;
            }
            if (conta_linha == (qnt_linha + 1))
            {
                strcpy(linha, nome);
                strToUpper(linha);
                fprintf(tmpRef, "%s\n", linha);
            }
            fclose(addRef);
            fclose(tmpRef);

            remove(fname2);
            rename("tmpRef.tmp", fname2);
        }
        p = p->next;
    }
    while (c != p);

    fmemCabecalho(&c);
    printf("Tabela criada.\n");
    /*system("pause");*/
    return 0;
}

int insert(char *tab, Tupla *vl)
{
    char ctl[20], dad[20];
    FILE *ctlstream, *dadstream;
    int erroTupla;
    Atributo ordatr;
    Tabela t;

    /* Vari�veis: ctl       -> nome do arquivo de meta-dados.
     dad       -> nome do arquivo de dados.
     ctlstream -> stream do arquivo de meta-dados da rela��o.
     dadstream -> stream do arquivo de dados da rela��o.
     erroTupla    -> c�digo de erro encontrado na tupla a ser inserida.
     ordatr    -> armazena o atributo pelo qual a rela��o �
     ordenada, se houver. */

    strcpy(ctl, tab);
    strcat(ctl, ".ctl");
    if ((ctlstream = fopen(ctl, "r")) == NULL) /*Abre o arquivo .ctl.*/
    {
        fmemTupla(&vl);
        erroInsert(TAB_INEXISTENTE, tab);
        return 1;
    }
    strcpy(dad, tab);
    strcat(dad, ".dad");
    if ((dadstream = fopen(dad, "r")) == NULL) /*Abre o arquivo .dad.*/
    {
        fmemTupla(&vl);
        erroInsert(ERRO_ABRIR_DAD, tab);
        return 1;
    }

    t = decTabela(ctlstream, dadstream);

    fclose(ctlstream);
    fclose(dadstream);

    erroTupla = validaTupla(vl, t); /*Verifica se h� erros na tupla*/
    if (erroTupla) /*a ser inserida.*/
    {
        fmemCabecalho(&t.cabecalho);
        fmemTupla(&vl);
        fmemRelacao(&t.relacao);
        erroInsert(erroTupla, tab);
        return 1;
    }

    if (t.ord) /*Se h� um atributo definido c/ a cl�usula [ORD],*/
    {
        natr(t.cabecalho, t.ord, &ordatr); /*verifica o tipo do atributo*/
        if (ordatr.tipo == 0)
            iordinsertRelacao(&t.relacao, vl, t.ord);
        else
            sordinsertRelacao(&t.relacao, vl, t.ord); /*e insere ordenadamente c/ a
			 fun��o correspondente ao tipo.*/
    }
    else
        insertRelacao(&t.relacao, vl); /*Caso contr�rio, insere no fim.*/
    t.m++; /*Atualiza a cardinalidade da tabela.*/

    if ((ctlstream = fopen(ctl, "w")) == NULL) /*Reescreve o arquivo .ctl*/
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroInsert(ERRO_CRIAR_CTL, tab);
        return 1;
    }
    if ((dadstream = fopen(dad, "w")) == NULL) /*Reescreve o arquivo .dad*/
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroInsert(ERRO_CRIAR_DAD, tab);
        return 1;
    }

    geraCTL(ctlstream, t.n, t.m, t.cabecalho); /*Gera o novo arquivo .ctl*/
    fclose(ctlstream);
    fmemCabecalho(&t.cabecalho);
    geraDAD(dadstream, t.relacao); /*Gera o novo arquivo .dad*/
    fclose(dadstream);
    fmemRelacao(&t.relacao);

    printf("Insercao concluida.\n");
    /*system("pause");*/
    return 0;
}

int deleteCond(char *tab, char *atrib, char *valor)
{
    char ctl[20], dad[20], ctl2[20];
    int posAtrib, erroComp;
    FILE *ctlstream, *dadstream;
    Tabela t;
    Cabecalho *c;
    Condicao cond;
    Relacao *auxRel;
    char removeuPrimeiro;
    Tupla *tupla;


    /* Vari�veis: ctl       -> nome do arquivo de meta-dados.
     dad       -> nome do arquivo de dados.
     ctlstream -> stream do arquivo de meta-dados da rela��o.
     dadstream -> stream do arquivo de dados da rela��o. */
    char eu[20];
    strcpy(eu, tab);
    strcpy(ctl, tab);
    strcat(ctl, ".ctl");

    if ((ctlstream = fopen(ctl, "r")) == NULL) /*Abre o arquivo .ctl.*/
    {
        erroDelete(TAB_INEXISTENTE, tab);
        return 1;
    }
    strcpy(dad, tab);
    strcat(dad, ".dad");
    if ((dadstream = fopen(dad, "r")) == NULL) /*Abre o arquivo .dad.*/
    {
        erroDelete(ERRO_ABRIR_DAD, tab);
        return 1;
    }
    /*printf("abrindo tabela");*/
    t = decTabela(ctlstream, dadstream);
    /*printf("tabela aberta\n");*/
    fclose(ctlstream);
    fclose(dadstream);

    strcpy(cond.atr1, atrib);
    strcpy(cond.val, valor);

    erroComp = validaCondSel(cond, t.cabecalho);
    
    if (erroComp)
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroDelete(erroComp, tab);
        return 1;
    }
    
    if (t.m > 0)
    {
        posAtrib = 0;
        c = t.cabecalho;
        
        while (strcasecmp(atrib, (*c).atributo.nome))
        {
          
            posAtrib++;
            c = (*c).next;
        }
        auxRel = t.relacao;
        do
        {
            tupla = (*auxRel).tupla;
            removeuPrimeiro = 0;
            int i;
            for (i = 0; i < posAtrib; i++)
                tupla = tupla->next;
            if (!strcmp(valor, tupla->val))
            {
                if (auxRel == auxRel->next)
                {
                    free(auxRel);
                    auxRel = NULL;
                    t.relacao = NULL;
                }
                else
                {
                    if (auxRel == t.relacao)
                    {
                        t.relacao = t.relacao->next;
                        removeuPrimeiro = 1;
                    }
                    auxRel->prev->next = auxRel->next;
                    auxRel->next->prev = auxRel->prev;
                    Relacao *auxRel2 = auxRel;
                    auxRel = auxRel->next;
                    free(auxRel2);
                }
                t.m--;
            }
            else
                auxRel = auxRel->next;
        }
        while ((auxRel != t.relacao) || removeuPrimeiro);
    }

    if (c->atributo.key)
    {
        Referenciada *ref = c->atributo.f_referenciada;
        char tab[20], att[20], nValor[50];
        Cabecalho *c_ref, *cc_ref;
        int n_ref, m_ref;

        if (ref != NULL)
        {
            FILE *f_ref;
            do
            {

                strcpy(ctl2, ref->nome);
                strcat(ctl2, ".ctl");
                f_ref = fopen(ctl2, "r");
                c_ref = decCatalogo(f_ref, &n_ref, &m_ref);
                cc_ref = c_ref;
                do
                {
                    if (cc_ref->atributo.fkey && strcasecmp(
                                cc_ref->atributo.f_referencia, eu) == 0)
                    {

                        if (cc_ref->atributo.tipo_ref == CASCADE)
                        {

                            if (deleteCond(ref->nome, cc_ref->atributo.nome,
                                           valor) > 0)
                            {
                                fmemCabecalho(&t.cabecalho);
                                fmemRelacao(&t.relacao);
                                fclose(f_ref);
                                return 1;
                            }
                        }
                        else
                        {

                            fmemCabecalho(&t.cabecalho);
                            fmemRelacao(&t.relacao);
                            erroUpdate(ERRO_REFERENCIA_UP, ref->nome);
                            fclose(f_ref);
                            return 1;
                        }

                    }
                    cc_ref = cc_ref->next;
                }
                while (cc_ref != c_ref);
                fclose(f_ref);
                ref = ref->next;
            }
            while (ref != c->atributo.f_referenciada);
        }
    }

    if ((ctlstream = fopen(ctl, "w")) == NULL) /*Reescreve o arquivo .ctl*/
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroDelete(ERRO_CRIAR_CTL, tab);
        return 1;
    }
    if ((dadstream = fopen(dad, "w")) == NULL) /*Reescreve o arquivo .dad*/
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroDelete(ERRO_CRIAR_DAD, tab);
        return 1;
    }
    geraCTL(ctlstream, t.n, t.m, t.cabecalho); /*Gera o novo arquivo .ctl*/
    fclose(ctlstream);
    fmemCabecalho(&t.cabecalho);
    geraDAD(dadstream, t.relacao); /*Gera o novo arquivo .dad*/
    fclose(dadstream);
    fmemRelacao(&t.relacao);

    printf("Delecao concluida.\n");
    /*system("pause");*/
    return 0;
}

int deleteAll(char *tab)
{
    char ctl[20], dad[20], ctl2[20];
    FILE *ctlstream, *dadstream;
    Tabela t;

    /* Vari�veis: ctl       -> nome do arquivo de meta-dados.
     dad       -> nome do arquivo de dados.
     ctlstream -> stream do arquivo de meta-dados da rela��o.
     dadstream -> stream do arquivo de dados da rela��o. */
    char eu[20];
    strcpy(eu, tab);
    strcpy(ctl, tab);
    strcat(ctl, ".ctl");
    if ((ctlstream = fopen(ctl, "r")) == NULL) /*Abre o arquivo .ctl.*/
    {
        erroDelete(TAB_INEXISTENTE, tab);
        return 1;
    }
    strcpy(dad, tab);
    strcat(dad, ".dad");
    if ((dadstream = fopen(dad, "r")) == NULL) /*Abre o arquivo .dad.*/
    {
        erroDelete(ERRO_ABRIR_DAD, tab);
        return 1;
    }

    t = decTabela(ctlstream, dadstream);
    /*faz cascade*/

    fclose(ctlstream);
    fclose(dadstream);
    t.m = 0; /*Atualiza a cardinalidade da tabela para 0, pois a tabela
	 ser� esvaziada.*/

    if ((ctlstream = fopen(ctl, "w")) == NULL) /*Reescreve o arquivo .ctl*/
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroDelete(ERRO_CRIAR_CTL, tab);
        return 1;
    }
    if ((dadstream = fopen(dad, "w")) == NULL) /*Reescreve o arquivo .dad*/
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroDelete(ERRO_CRIAR_DAD, tab);
        return 1;
    }
    Cabecalho *c = t.cabecalho;
    do
    {
        if (c->atributo.key)
        {
            Referenciada *ref = c->atributo.f_referenciada;
            char tab[20], att[20], nValor[50];
            Cabecalho *c_ref, *cc_ref;
            Relacao *rel_ref;
            Tupla *tupla_ref;
            int n_ref, m_ref;
            int poschv = t.chv;
            int indice;
            char val[30];

            if (ref != NULL)
            {
                FILE *f_ref;
                do
                {

                    strcpy(ctl2, ref->nome);
                    strcat(ctl2, ".ctl");
                    f_ref = fopen(ctl2, "r");
                    c_ref = decCatalogo(f_ref, &n_ref, &m_ref);
                    cc_ref = c_ref;
                    do
                    {
                        if (cc_ref->atributo.fkey && strcasecmp(
                                    cc_ref->atributo.f_referencia, eu) == 0)
                        {

                            if (cc_ref->atributo.tipo_ref == CASCADE)
                            {
                                rel_ref = t.relacao;
                                do
                                {
                                    tupla_ref = rel_ref->tupla;
                                    indice = 0;
                                    do
                                    {
                                        indice++;
                                        if (indice == poschv)
                                        {

                                            if (deleteCond(ref->nome,
                                                           cc_ref->atributo.nome,
                                                           tupla_ref->val) > 0)
                                            {
                                                fmemCabecalho(&t.cabecalho);
                                                fmemRelacao(&t.relacao);
                                                fclose(f_ref);
                                                return 1;
                                            }
                                        }
                                        tupla_ref = tupla_ref->next;
                                    }
                                    while (tupla_ref != rel_ref->tupla);
                                    rel_ref = rel_ref->next;
                                }
                                while (rel_ref != t.relacao);
                            }
                            else
                            {

                                erroUpdate(ERRO_REFERENCIA_UP, ref->nome);
                                fmemCabecalho(&t.cabecalho);
                                fmemRelacao(&t.relacao);
                                fclose(f_ref);
                                return 1;
                            }

                        }
                        cc_ref = cc_ref->next;
                    }
                    while (cc_ref != c_ref);
                    fclose(f_ref);
                    ref = ref->next;
                }
                while (ref != c->atributo.f_referenciada);
            }
        }
        c = c->next;
    }
    while (c != t.cabecalho);
    fmemRelacao(&t.relacao);
    geraCTL(ctlstream, t.n, t.m, t.cabecalho); /*Gera o novo arquivo .ctl*/
    fclose(ctlstream);
    fmemCabecalho(&t.cabecalho);
    t.relacao = NULL;
    geraDAD(dadstream, t.relacao); /*Gera o novo arquivo .dad*/
    fclose(dadstream);
    fmemRelacao(&t.relacao);
    printf("Delecao concluida.\n");
    /*system("pause");*/
    return 0;
}

int updateCond(char *tab, char *atribSet, char *valorSet, char *atribWhere,
               char *valorWhere)
{

    char ctl[20], dad[20], ctl2[20];
    int posAtrib, posAtribWhere, erroComp, erroTupla;
    FILE *ctlstream, *dadstream;
    Tabela t;
    Cabecalho *c, *c2;
    Condicao cond;
    Relacao *auxRel, *auxListaRel;
    Atributo ordatr;
    Tupla *tupla, *tupla2;

    /* Vari�veis: ctl       -> nome do arquivo de meta-dados.
     dad       -> nome do arquivo de dados.
     ctlstream -> stream do arquivo de meta-dados da rela��o.
     dadstream -> stream do arquivo de dados da rela��o. */
    char eu[20];
    strcpy(eu, tab);
    strcpy(ctl, tab);
    strcat(ctl, ".ctl");
    if ((ctlstream = fopen(ctl, "r")) == NULL) /*Abre o arquivo .ctl.*/
    {
        erroUpdate(TAB_INEXISTENTE, tab);
        return 1;
    }
    strcpy(dad, tab);
    strcat(dad, ".dad");
    if ((dadstream = fopen(dad, "r")) == NULL) /*Abre o arquivo .dad.*/
    {
        erroUpdate(ERRO_ABRIR_DAD_D, tab);
        return 1;
    }

    t = decTabela(ctlstream, dadstream);

    fclose(ctlstream);
    fclose(dadstream);

    strcpy(cond.atr1, atribSet);
    strcpy(cond.val, valorSet);

    erroComp = validaCondSel(cond, t.cabecalho);
    if (erroComp)
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroUpdate(erroComp, tab);
        return 1;
    }

    strcpy(cond.atr1, atribWhere);
    strcpy(cond.val, valorWhere);

    erroComp = validaCondSel(cond, t.cabecalho);
    if (erroComp)
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroUpdate(erroComp, tab);
        return 1;
    }
    if (t.m > 0)
    {
        posAtrib = posAtribWhere = 0;
        c2 = c = t.cabecalho;
        while (strcasecmp(atribSet, (*c).atributo.nome))
        {
            posAtrib++;
            c = (*c).next;
        }

        if ((*c).atributo.nome)
            removeAspas(valorSet);
        while (strcasecmp(atribWhere, (*c2).atributo.nome))
        {
            posAtribWhere++;
            c2 = (*c2).next;
        }
        if ((*c2).atributo.nome)
            removeAspas(valorWhere);
        auxRel = t.relacao;
        t.relacao = NULL;
        do
        {
            auxListaRel = auxRel;
            tupla2 = tupla = auxRel->tupla;
            int i;
            for (i = 0; i < posAtribWhere; i++)
                tupla2 = tupla2->next;
            erroTupla = validaTupla2(tupla, t); /*Verifica se h� erros na tupla*/
            if (erroTupla) /*a ser inserida.*/
            {
                fmemCabecalho(&t.cabecalho);
                fmemRelacao(&t.relacao);
                fmemRelacao(&auxRel);
                erroInsert(erroTupla, tab);
                return 1;
            }
            if (!strcmp(valorWhere, tupla2->val))
            {
                for (i = 0; i < posAtrib; i++)
                    tupla = tupla->next;
                strcpy(tupla->val, valorSet);
            }
            if (auxRel == auxRel->next)
            {
                auxRel = NULL;
            }
            else
            {
                auxRel->prev->next = auxRel->next;
                auxRel->next->prev = auxRel->prev;
                auxRel = auxRel->next;
            }
            if (t.ord) /*Se h� um atributo definido c/ a cl�usula [ORD],*/
            {
                natr(t.cabecalho, t.ord, &ordatr); /*verifica o tipo do atributo*/
                if (ordatr.tipo == 0)
                    iordinsertRelacao(&t.relacao, auxListaRel->tupla, t.ord);
                else
                    sordinsertRelacao(&t.relacao, auxListaRel->tupla, t.ord); /*e insere ordenadamente c/ a
					 fun��o correspondente ao tipo.*/
            }
            else
            {
                insertRelacao(&t.relacao, auxListaRel->tupla);
            }
        }
        while (auxRel);

    }
    if (c->atributo.key)
    {
        Referenciada *ref = c->atributo.f_referenciada;
        char tab[20], att[20], nValor[50];
        Cabecalho *c_ref, *cc_ref;
        int n_ref, m_ref;

        if (ref != NULL)
        {
            FILE *f_ref;
            do
            {

                strcpy(ctl2, ref->nome);
                strcat(ctl2, ".ctl");
                f_ref = fopen(ctl2, "r");
                c_ref = decCatalogo(f_ref, &n_ref, &m_ref);
                cc_ref = c_ref;
                do
                {
                    if (cc_ref->atributo.fkey && strcasecmp(
                                cc_ref->atributo.f_referencia, eu) == 0)
                    {

                        if (cc_ref->atributo.tipo_ref == CASCADE)
                        {

                            if (updateCond(ref->nome, cc_ref->atributo.nome,
                                           valorSet, cc_ref->atributo.nome, valorWhere)
                                    > 0)
                            {
                                fmemCabecalho(&t.cabecalho);
                                fmemRelacao(&t.relacao);
                                fclose(f_ref);
                                return 1;
                            }
                        }
                        else
                        {

                            fmemCabecalho(&t.cabecalho);
                            fmemRelacao(&t.relacao);
                            erroUpdate(ERRO_REFERENCIA_UP, ref->nome);
                            fclose(f_ref);
                            return 1;
                        }

                    }
                    cc_ref = cc_ref->next;
                }
                while (cc_ref != c_ref);
                fclose(f_ref);
                ref = ref->next;
            }
            while (ref != c->atributo.f_referenciada);
        }
    }
    if ((ctlstream = fopen(ctl, "w")) == NULL) /*Reescreve o arquivo .ctl*/
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroUpdate(ERRO_CRIAR_CTL, tab);
        return 1;
    }
    if ((dadstream = fopen(dad, "w")) == NULL) /*Reescreve o arquivo .dad*/
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroUpdate(ERRO_CRIAR_DAD, tab);
        return 1;
    }

    geraCTL(ctlstream, t.n, t.m, t.cabecalho); /*Gera o novo arquivo .ctl*/
    fclose(ctlstream);
    fmemCabecalho(&t.cabecalho);
    geraDAD(dadstream, t.relacao); /*Gera o novo arquivo .dad*/
    fclose(dadstream);
    fmemRelacao(&t.relacao);

    printf("Atualizacao concluida.\n");
    /*system("pause");*/
    return 0;
}

int updateAll(char *tab, char *atrib, char *valor)
{
    char ctl[20], dad[20];
    int posAtrib, erroComp, erroTupla;
    FILE *ctlstream, *dadstream;
    Tabela t;
    Cabecalho *c;
    Condicao cond;
    Relacao *auxRel, *auxListaRel;
    Atributo ordatr;
    Tupla *tupla;

    /* Vari�veis: ctl       -> nome do arquivo de meta-dados.
     dad       -> nome do arquivo de dados.
     ctlstream -> stream do arquivo de meta-dados da rela��o.
     dadstream -> stream do arquivo de dados da rela��o. */

    strcpy(ctl, tab);
    strcat(ctl, ".ctl");
    if ((ctlstream = fopen(ctl, "r")) == NULL) /*Abre o arquivo .ctl.*/
    {
        erroUpdate(TAB_INEXISTENTE, tab);
        return 1;
    }
    strcpy(dad, tab);
    strcat(dad, ".dad");
    if ((dadstream = fopen(dad, "r")) == NULL) /*Abre o arquivo .dad.*/
    {
        erroUpdate(ERRO_ABRIR_DAD_D, tab);
        return 1;
    }

    printf("abre tabela\n");
    t = decTabela(ctlstream, dadstream);
    printf("tabela aberta\n");
    fclose(ctlstream);
    fclose(dadstream);

    strcpy(cond.atr1, atrib);
    strcpy(cond.val, valor);

    erroComp = validaCondSel(cond, t.cabecalho);
    printf("condi��o validada com erro = %i", erroComp);
    if (erroComp)
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroUpdate(erroComp, tab);
        return 1;
    }
    if (t.m > 0)
    {
        posAtrib = 0;
        c = t.cabecalho;
        while (strcasecmp(atrib, (*c).atributo.nome))
        {
            posAtrib++;
            c = (*c).next;
        }
        auxRel = t.relacao;
        t.relacao = NULL;
        do
        {
            tupla = auxRel->tupla;
            auxListaRel = auxRel;
            erroTupla = validaTupla2(tupla, t);
            if (erroTupla)
            {
                fmemCabecalho(&t.cabecalho);
                fmemRelacao(&t.relacao);
                fmemRelacao(&auxRel);
                erroUpdate(erroTupla, tab);
                return 1;
            }
            int i;
            for (i = 0; i < posAtrib; i++)
                tupla = tupla->next;
            strcpy(tupla->val, valor);
            if (auxRel == auxRel->next)
            {
                auxRel = NULL;
            }
            else
            {
                auxRel->prev->next = auxRel->next;
                auxRel->next->prev = auxRel->prev;
                auxRel = auxRel->next;
            }
            if (t.ord) /*Se h� um atributo definido c/ a cl�usula [ORD],*/
            {
                natr(t.cabecalho, t.ord, &ordatr); /*verifica o tipo do atributo*/
                if (ordatr.tipo == 0)
                    iordinsertRelacao(&t.relacao, auxListaRel->tupla, t.ord);
                else
                    sordinsertRelacao(&t.relacao, auxListaRel->tupla, t.ord); /*e insere ordenadamente c/ a
					 fun��o correspondente ao tipo.*/
            }
            else
            {
                insertRelacao(&t.relacao, auxListaRel->tupla);
            }
        }
        while (auxRel);
    }

    if ((ctlstream = fopen(ctl, "w")) == NULL) /*Reescreve o arquivo .ctl*/
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroUpdate(ERRO_CRIAR_CTL, tab);
        return 1;
    }
    if ((dadstream = fopen(dad, "w")) == NULL) /*Reescreve o arquivo .dad*/
    {
        fmemCabecalho(&t.cabecalho);
        fmemRelacao(&t.relacao);
        erroUpdate(ERRO_CRIAR_DAD, tab);
        return 1;
    }
    geraCTL(ctlstream, t.n, t.m, t.cabecalho); /*Gera o novo arquivo .ctl*/
    fclose(ctlstream);
    fmemCabecalho(&t.cabecalho);
    geraDAD(dadstream, t.relacao); /*Gera o novo arquivo .dad*/
    fclose(dadstream);
    fmemRelacao(&t.relacao);

    printf("Atualizacao concluida.\n");
    /*system("pause");*/
    return 0;
}

int selectSQL(char *tab, char *listaAtrib, Condicao *cond,int isDistinct,int isOrderBy,int isCount,int isSum,int isMin,int isMax, int isAvg)
{
    void strToUpper(char *str);
    /* Passa uma string para seu equivalente c/ letras mai�sculas.
     (Implementada em <Estruturas.c>) */

    char ctl[20], dad[20], alg[20], query[9], opr[100], atribCount[3];
    FILE *ctlstream, *dadstream, *algstream;
    Cabecalho *c;
    Relacao *r;
    Tabela t;
    int n, m, erro;

    /* Vari�veis: ctl       -> nome do arquivo de meta-dados.
     dad       -> nome do arquivo de dados.
     alg       -> nome do arquivo de opera��es da �lgebra relacional.
     query     -> nome da rela��o resultante da consulta.
     opr       -> parte da opera��o da �lgebra relacional gerada.
     atribCount       -> numero de atributos em listaAtrib.
     ctlstream -> stream do arquivo de meta-dados da rela��o.
     dadstream -> stream do arquivo de dados da rela��o.
     algstream -> stream do arquivo de opera��es da �lgebra relacional.
     c        -> ponteiro p/ uma lista c/ os atributos da
     rela��o <tab>.
     r        -> ponteiro p/ uma lista c/ as tuplas da
     rela��o <tab>.
     t        -> tabela.
     n         -> grau da rela��o.
     m         -> cardinalidade da rela��o.
     erro  -> c�digo de erro. */
    strcpy(ctl, tab);
    strcat(ctl, ".ctl");
    if ((ctlstream = fopen(ctl, "r")) == NULL) /*Abre o arquivo .ctl.*/
    {
        erroSelect(TAB_INEXISTENTE, tab, "");
        return 1;
    }

    c = decCatalogo(ctlstream, &n, &m);
    erro = validaListaAtrib(&listaAtrib, tab, NULL, c, NULL);
    if (erro)
    {
        fmemCabecalho(&c);
        fclose(ctlstream);
        erroSelect(erro, tab, "");
        return 1;
    }
    opr[0] = '\0';
    

    if (cond) /*Se h� condi��o de sele��o,*/
    {
        fclose(ctlstream);
        strToUpper(cond->atr1);
        erro = validaCondSel(*cond, c); /*verifica se h� erros na condi��o*/
        if (erro)
        {
            fmemCabecalho(&c);
            erroSelect(erro, tab, cond->atr1);
            return 1;
        }
        strcpy(opr, cond->opr);
        strcat(opr, "(");
        strcat(opr, tab);
        strcat(opr, ",");
        strcat(opr, cond->atr1);
        strcat(opr, ",");
        strcat(opr, cond->val);
    } 
    else 
    {
        strcpy(opr, "=(");
        strcat(opr, tab);
        strcat(opr, ",");
        strcat(opr, ",");
    }

    geraNomeAlg(alg); /*Escolhe o nome do arquivo .alg*/
    if ((algstream = fopen(alg, "w")) == NULL) /*Cria o arquivo .alg*/
    {
        if (cond)
            fmemCabecalho(&c);
        erroSelect(ERRO_CRIAR_ARQ, alg, cond->atr1);
        return 1;
    }

    strncpy(query, alg, 8); /*<query> recebe o nome da rela��o resultante*/
    query[8] = '\0';

    strcat(opr, ",");
    strcat(opr, query);

    if (strcasecmp(listaAtrib, "*"))
    {
        strcat(opr, "s)\n");
        strcat(opr, "P(");
        strcat(opr, query);
        strcat(opr, "s,");
        sprintf(atribCount, "%i", contaVirgulas(listaAtrib) + 1); 
        strcat(opr, atribCount);
        strcat(opr, ",");
        strcat(opr, listaAtrib);
        strcat(opr, ",");
        strcat(opr, query);
    }

    strcat(opr, ")\n");

    fprintf(algstream, "%s", opr);/*Escreve a opera��o da �lgebra relacional*/
    fclose(algstream); /*no arquivo .alg.*/
    if (cond)
        fmemCabecalho(&c);
    algstream = fopen(alg, "r"); /*Reabre o arquivo .alg.*/
    
    executaAlg(algstream, isDistinct, isOrderBy); /*Chama a fun��o que executa as opera��es da �lgebra relacional.*/
    strcpy(ctl, query);
    strcat(ctl, ".ctl");
    if ((ctlstream = fopen(ctl, "r")) == NULL)
    {
        erroSelect(ERRO_ABRIR_CTL, query, cond->atr1);/*Abre os arquivos gerados pela execu��o*/
        return 1; /*da opera��o da �lgebra relacional.*/
    }
    strcpy(dad, query);
    strcat(dad, ".dad");
    if ((dadstream = fopen(dad, "r")) == NULL)
    {
        erroSelect(ERRO_ABRIR_DAD_S, query, cond->atr1);
        return 1;
    }

    t = decTabela(ctlstream, dadstream); /*Recupera as informa��es*/
    fclose(ctlstream); /*da rela��o gerada.*/
    fclose(dadstream);

    imprimeTabela(query, t,isCount,isSum,isMin,isMax,isAvg); /*Exibe a rela��o gerada na tela.*/
    return 0;
}

int join(char *tab1, char *tab2, char *listaAtrib, Condicao jcond,
         Condicao *scond)
{
    void strToUpper(char *str);
    /* Passa uma string para seu equivalente c/ letras mai�sculas.
     (Implementada em <Estruturas.c>) */

    char ctl[20], dad[20], alg[20], query[9], sopr[100], jopr[100], popr[100],
    atribCount[3];
    FILE *ctlstream1, *ctlstream2, *dadstream1, *dadstream2, *algstream;
    Cabecalho *c1, *c2;
    Relacao *r;
    Tabela t;
    int n, m, erro, jcondA1, jcondA2, scondA1;

    /* Vari�veis: ctl        -> nome do arquivo de meta-dados.
     dad        -> nome do arquivo de dados.
     alg        -> nome do arquivo de opera��es da �lgebra relacional.
     query      -> nome da rela��o resultante da consulta.
     sopr       -> parte da opera��o da �lgebra relacional de sele��o gerada.
     popr       -> parte da opera��o da �lgebra relacional de proje��o gerada.
     jopr       -> parte da opera��o da �lgebra relacional de jun��o gerada.
     atribCount       -> numero de atributos em listaAtrib.
     ctlstream1 -> stream do arquivo de meta-dados da rela��o 1.
     ctlstream2 -> stream do arquivo de meta-dados da rela��o 2.
     dadstream1 -> stream do arquivo de dados da rela��o 1.
     dadstream2 -> stream do arquivo de dados da rela��o 2.
     algstream  -> stream do arquivo de opera��es da �lgebra relacional.
     c1        -> ponteiro p/ uma lista c/ os atributos da
     rela��o 1.
     c2        -> ponteiro p/ uma lista c/ os atributos da
     rela��o 2.
     r         -> ponteiro p/ uma lista c/ as tuplas da
     rela��o <tab>.
     t         -> tabela.
     n          -> grau da rela��o.
     m          -> cardinalidade da rela��o.
     erro   -> c�digo de erro.
     jcondA1    -> n� da rela��o � qual pertence o atributo 1
     da condi��o de jun��o.
     jcondA2    -> n� da rela��o � qual pertence o atributo 2
     da condi��o de jun��o.
     scondA1    -> n� da rela��o � qual pertence o atributo
     da condi��o de sele��o. */
    strcpy(ctl, tab1);
    strcat(ctl, ".ctl");
    if ((ctlstream1 = fopen(ctl, "r")) == NULL) /*Abre os arquivos .ctl.*/
    {
        erroJoin(TAB_INEXISTENTE, tab1, "", jcond);
        return 1;
    }
    strcpy(ctl, tab2);
    strcat(ctl, ".ctl");
    if ((ctlstream2 = fopen(ctl, "r")) == NULL)
    {
        erroJoin(TAB_INEXISTENTE, tab2, "", jcond);
        return 1;
    }
    /*Recupera as informa��es das rela��es*/
    c1 = decCatalogo(ctlstream1, &n, &m);
    fclose(ctlstream1);
    c2 = decCatalogo(ctlstream2, &n, &m);
    fclose(ctlstream2);

    erro = validaListaAtrib(&listaAtrib, tab1, tab2, c1, c2);
    if (erro)
    {
        fmemCabecalho(&c1);
        fclose(ctlstream1);
        fmemCabecalho(&c2);
        fclose(ctlstream2);
        erroSelect(erro, "", "");
        return 1;
    }

    strToUpper(jcond.atr1);
    jcondA1 = atrtab(jcond.atr1, tab1, tab2, c1, c2);
    /*Verifica a que tabela o atributo 1 pertence.*/
    if (jcondA1 == 0) /*Se n�o pertence a nenhuma*/
    {
        fmemCabecalho(&c1);
        fmemCabecalho(&c2);
        erroJoin(ATR1_INEXISTENTE, tab1, tab2, jcond);
        return 1;
    }
    if (jcondA1 == 3) /*Se pertence a ambas*/
    {
        fmemCabecalho(&c1);
        fmemCabecalho(&c2);
        erroJoin(ATR1_N_QUALIFIC, tab1, tab2, jcond);
        return 1;
    }
    strToUpper(jcond.atr2);
    jcondA2 = atrtab(jcond.atr2, tab1, tab2, c1, c2);
    /*Verifica a que tabela o atributo 2 pertence.*/
    if (jcondA2 == 0) /*Se n�o pertence a nenhuma*/
    {
        fmemCabecalho(&c1);
        fmemCabecalho(&c2);
        erroJoin(ATR2_INEXISTENTE, tab1, tab2, jcond);
        return 1;
    }
    if (jcondA2 == 3) /*Se pertence a ambas*/
    {
        fmemCabecalho(&c1);
        fmemCabecalho(&c2);
        erroJoin(ATR2_N_QUALIFIC, tab1, tab2, jcond);
        return 1;
    }
    if (jcondA1 == jcondA2) /*Se ambos pertencem � mesma tabela*/
    {
        fmemCabecalho(&c1);
        fmemCabecalho(&c2);
        erroJoin(ATRS_MESMA_REL, tab1, tab2, jcond);
        return 1;
    }
    if (jcondA1 == 2)
    {
        swap(jcond.atr1, jcond.atr2);
    }
    /*Se o atributo 1 pertence � tabela 2 e o atributo 2 � tabela 1, troca.*/

    erro = validaCondJun(jcond, c1, c2);
    /*Verifica se h� erros na condi��o de jun��o.*/
    if (erro)
    {
        fmemCabecalho(&c1);
        fmemCabecalho(&c2);
        erroJoin(erro, tab1, tab2, jcond);
        return 1;
    }
    geraNomeAlg(alg); /*Escolhe o nome do arquivo .alg*/
    strncpy(query, alg, 8); /*<query> recebe o nome da rela��o resultante.*/
    query[8] = '\0';
    sopr[0] = '\0';
    jopr[0] = '\0';
    if (!scond) /*Se n�o h� condi��o de sele��o*/
    {
        strcpy(jopr, "J(");
        strcat(jopr, tab1);
        strcat(jopr, ",");
        strcat(jopr, tab2);
        strcat(jopr, ",");
        strcat(jopr, jcond.atr1);
        strcat(jopr, "=");
        strcat(jopr, jcond.atr2);
    }
    else   /*Se h� condi��o de sele��o*/
    {
        strToUpper(scond->atr1);
        scondA1 = atrtab(scond->atr1, tab1, tab2, c1, c2);
        /*Verifica a que tabela pertence o atributo*/
        if (scondA1 == 0) /*Se n�o pertence a nenhuma*/
        {
            fmemCabecalho(&c1);
            fmemCabecalho(&c2);
            erroJoin(ATR1_INEXISTENTE, tab1, tab2, *scond);
            return 1;
        }
        if (scondA1 == 3) /*Se pertence a ambas*/
        {
            fmemCabecalho(&c1);
            fmemCabecalho(&c2);
            erroJoin(ATR1_N_QUALIFIC, tab1, tab2, *scond);
            return 1;
        }
        if (scondA1 == 1) /*Se pertence � tabela 1*/
        {
            erro = validaCondSel(*scond, c1);
            /*Verifica se h� erros na condi��o de sele��o.*/
            if (erro)
            {
                fmemCabecalho(&c1);
                fmemCabecalho(&c2);
                erroSelect(erro, tab1, scond->atr1);
                return 1;
            }
        }
        else
        {
            erro = validaCondSel(*scond, c2);
            /*Verifica se h� erros na condi��o de sele��o.*/
            if (erro)
            {
                fmemCabecalho(&c1);
                fmemCabecalho(&c2);
                erroSelect(erro, tab2, scond->atr1);
                return 1;
            }
        }
        strcpy(sopr, scond->opr);
        strcat(sopr, "(");
        if (scondA1 == 1)
            strcat(sopr, tab1);
        else
            strcat(sopr, tab2);
        strcat(sopr, ",");
        strcat(sopr, scond->atr1);
        strcat(sopr, ",");
        strcat(sopr, scond->val);

        strcpy(jopr, "J(");
        if (scondA1 == 1)
        {
            strcat(jopr, query);
            strcat(jopr, "s,");
            strcat(jopr, tab2);
        }
        else
        {
            strcat(jopr, tab1);
            strcat(jopr, ",");
            strcat(jopr, query);
            strcat(jopr, "s");
        }
        strcat(jopr, ",");
        strcat(jopr, jcond.atr1);
        strcat(jopr, "=");
        strcat(jopr, jcond.atr2);
    }
    if (strcasecmp(listaAtrib, "*"))
    {
        strcpy(popr, "P(");
        strcat(popr, query);
        strcat(popr, "j,");
        sprintf(atribCount, "%i", contaVirgulas(listaAtrib) + 1); 
        strcat(popr, atribCount);
        strcat(popr, ",");
        strcat(popr, listaAtrib);
    }
    if ((algstream = fopen(alg, "w")) == NULL) /*Cria o arquivo .alg*/
    {
        fmemCabecalho(&c1);
        fmemCabecalho(&c2);
        erroSelect(ERRO_CRIAR_ARQ, alg, "");
        return 1;
    } /*Escreve as opera��es da �lgebra relacional no arquivo .alg.*/
    if (scond)
        fprintf(algstream, "%s,%ss)\n", sopr, query);
    fprintf(algstream, "%s,%s", jopr, query);
    if (strcasecmp(listaAtrib, "*"))
        fprintf(algstream, "j)\n%s,%s", popr, query);
    fprintf(algstream, ")\n");
    fclose(algstream);
    /*fmemCabecalho(&c1);*/
    

    algstream = fopen(alg, "r"); /*Reabre o arquivo .alg.*/
    executaAlg(algstream, 0, 0); /*Chama a fun��o que executa as opera��es da �lgebra relacional.*/
    strcpy(ctl, query);
    strcat(ctl, ".ctl");
    if ((ctlstream1 = fopen(ctl, "r")) == NULL)   /*Abre os arquivos gerados pela execu��o*/
    {
        erroSelect(ERRO_ABRIR_CTL, query, scond->atr1); /*das opera��es da �lgebra relacional.*/
        return 1;
    }
    strcpy(dad, query);
    strcat(dad, ".dad");
    if ((dadstream1 = fopen(dad, "r")) == NULL)
    {
        erroSelect(ERRO_ABRIR_DAD_S, query, scond->atr1);
        return 1;
    }
    t = decTabela(ctlstream1, dadstream1); /*Recupera as informa��es*/
    fclose(ctlstream1); /*da rela��o gerada.*/
    fclose(dadstream1);
    imprimeTabela(query, t,0,0,0,0,0); /*Exibe a rela��o gerada na tela.*/
    fmemCabecalho(&c1);
    fmemCabecalho(&c2);
    fmemRelacao(&r);
    return 0;
}

int validaCabecalho(Cabecalho *c)
{
    int numOrd, numChv, numFk;
    Cabecalho *p1, *p2, *cab;
    void strToLower(char *str);
    /* Cabecalho *decCatalogo (FILE *stream, int *n, int *m);*/
    /* Vari�veis: numOrd -> n� de atributos definidos c/ a cl�usula [ORD].
     numChv -> n� de atributos definidos c/ a cl�usula [CHV].
     p1     -> ponteiro utilizado p/ percorrer <c>.
     p2     -> ponteiro utilizado p/ percorrer <c>. */

    if (!(c))
        return NENHUM_ATRIBUTO; /*Se a lista est� vazia, n�o h� nenhum atributo.*/
    else
    {
        numOrd = 0;
        numChv = 0;
        numFk = 0;
        p1 = c;
        do
        {
            if (p1->atributo.fkey)
            {
                FILE *ctlstream;
                char fname[20];
                strToLower(p1->atributo.f_referencia);
                strcpy(fname, p1->atributo.f_referencia);
                strcat(fname, ".ctl");
                if ((ctlstream = fopen(fname, "r")) == NULL)   /*Verifica se a tabela j� existe.*/
                {

                    erroCT(FK_TABELA_INEXISTENTE, p1->atributo.f_referencia);
                    return FK_TABELA_INEXISTENTE;
                }
                int n, m;
                cab = decCatalogo(ctlstream, &n, &m);
                p2 = cab;
                do
                {
                    if (p2->atributo.key)
                    {
                        if (p2->atributo.tipo != p1->atributo.tipo)
                        {
                            fclose(ctlstream);
                            erroCT(FK_TIPO_ERRADO, p1->atributo.f_referencia);
                            fmemCabecalho(&cab);
                            fclose(ctlstream);
                            return FK_TIPO_ERRADO;
                        }
                        p2 = cab;
                    }
                    else
                        p2 = p2->next;
                }
                while (p2 != cab);
                fmemCabecalho(&cab);
                fclose(ctlstream);
            }
            if (p1->atributo.ord == 1)
                numOrd++;
            if (p1->atributo.key == 1)
                numChv++;
            if (p1->atributo.fkey == 1)
                numFk++;
            p2 = p1->next;
            while (p2 != c) /*Verifica se h� atributos duplicados.*/
            {
                if (!strcmp(p1->atributo.nome, p2->atributo.nome))
                    return ATRIB_DUPLICADO;
                p2 = p2->next;
            }
            p1 = p1->next;
        }
        while (p1 != c);

        if (numChv > 1)
            return MAIS_QUE_UM_CHV; /*Se foi definida mais de uma chave.*/
        /* if (numFk > 1) return MAIS_QUE_UM_FCHV;    /*Se foi definida mais de uma chave estrangeira.*/
        if (numOrd > 1)
            return MAIS_QUE_UM_ORD; /*Se h� mais de um atributo c/ [ORD].*/
    }
    return 0; /*Se chegou at� aqui, n�o h� erro.*/
}

int validaTupla(Tupla *tupla, Tabela tab)
{
    Cabecalho *atrs;
    Tupla *vals;
    Relacao *tups;
    char valschv[100], tupchv[100];

    /* Vari�veis: atrs    -> ponteiro utilizado p/ percorrer o cabe�calho.
     vals    -> ponteiro utilizado p/ percorrer <tupla>.
     tups    -> ponteiro utilizado p/ percorrer a rela��o (tuplas).
     valschv -> valor do atributo chave em <tupla>.
     tupchv  -> armazena o valor do atributo chave de cada
     tupla acessada na rela��o. */

    /*Se o n� de valores na lista �*/
    if ((numVal(tupla)) != tab.n)
        return N_VALS_INCORRETO; /*menor que o n� de atributos da*/
    /*rela��o*/
    atrs = tab.cabecalho;
    vals = tupla;
    do /*Verifica se os tipos dos valores*/
    {
        /*s�o compat�veis c/ os dos atributos*/
        if (atrs->atributo.tipo == 0) /*da rela��o.*/
        {
            if (!isint(vals->val))
                return TIPO_INCORRETO;
        }
        else
        {
            if (!isstr(vals->val))
                return TIPO_INCORRETO;
            else
                removeAspas(vals->val);
        }

        if (atrs->atributo.fkey)
        {
            char arqN[30];
            FILE *arqR;
            FILE *arqC;

            strcpy(arqN, atrs->atributo.f_referencia);
            strcat(arqN, ".dad");
            arqR = fopen(arqN, "r");
            strcpy(arqN, atrs->atributo.f_referencia);
            strcat(arqN, ".ctl");
            arqC = fopen(arqN, "r");

            Tabela tab = decTabela(arqC, arqR);
            Tupla *t;
            Relacao *r;
            r = tab.relacao;
            int count, achou;
            achou = 0;
            if (tab.chv == 0)
            {
                fclose(arqR);
                fclose(arqC);
                return ERRO_REFERENCIAL;
            }
            do
            {
                count = 1;
                t = r->tupla;
                do
                {
                    if (count == tab.chv)
                    {
                        if (strcmp(t->val, vals->val) == 0)
                            achou = 1;
                        t = r->tupla;
                    }
                    else
                        t = t->next;
                    count++;
                }
                while (t != r->tupla);
                r = r->next;
            }
            while (r != tab.relacao && !achou);
            fclose(arqR);
            fclose(arqC);
            if (!achou)
                return ERRO_REFERENCIAL;
        }

        atrs = atrs->next;
        vals = vals->next;
    }
    while (atrs != tab.cabecalho);

    if (tab.chv > 0)
    {
        nval(tupla, tab.chv, valschv); /*<valschv> recebe o valor da chave na tupla.*/
        if (tab.relacao) /*Verifica se o valor da chave*/
        {
            /*j� existe na rela��o.*/
            tups = tab.relacao;
            do
            {
                nval(tups->tupla, tab.chv, tupchv);
                if (!strcmp(valschv, tupchv))
                    return VIOLACAO_CHAVE;
                tups = tups->next;
            }
            while (tups != tab.relacao);
        }
    }

    return 0; /*Se chegou at� aqui, n�o h� erros.*/
}

int validaTupla2(Tupla *tupla, Tabela tab)
{
    Cabecalho *atrs;
    Tupla *vals;
    Relacao *tups;
    char valschv[100], tupchv[100];

    /* Vari�veis: atrs    -> ponteiro utilizado p/ percorrer o cabe�calho.
     vals    -> ponteiro utilizado p/ percorrer <tupla>.
     tups    -> ponteiro utilizado p/ percorrer a rela��o (tuplas).
     valschv -> valor do atributo chave em <tupla>.
     tupchv  -> armazena o valor do atributo chave de cada
     tupla acessada na rela��o. */

    nval(tupla, tab.chv, valschv); /*<valschv> recebe o valor da chave na tupla.*/
    if (tab.chv == 0)
        return 0;
    if (tab.relacao) /*Verifica se o valor da chave*/
    {
        /*j� existe na rela��o.*/
        tups = tab.relacao;
        do
        {
            nval(tups->tupla, tab.chv, tupchv);
            if (!strcmp(valschv, tupchv))
                return VIOLACAO_CHAVE;
            tups = tups->next;
        }
        while (tups != tab.relacao);
    }

    return 0; /*Se chegou at� aqui, n�o h� erros.*/
}

int validaCondSel(Condicao cond, Cabecalho *c)
{
    int patr;
    Atributo atributo;

    /* Vari�veis: patr -> posi��o do atributo envolvido na condi��o em <c>.
     atributo  -> armazena o atributo que ocupa a posi��o <patr>
     em <c>. */

    patr = posatr(cond.atr1, c); /*Recupera a posi��o do atributo de*/
    if (!patr)
        return ATR_INEXISTENTE; /*compara��o em <c>.*/
    natr(c, patr, &atributo); /*<atributo> recebe o atributo desta posi��o.*/
    if (atributo.tipo == INTEGER) /*Verifica se a constante de compara��o � de tipo*/
    {
        /*compat�vel c/ o atributo.*/
        if (!isint(cond.val))
            return TIPO_INCORRETO_S;
    }
    else if (!isstr(cond.val))
        return TIPO_INCORRETO_S;
    return 0; /*Se chegou at� aqui, n�o h� erros.*/
}

int validaCondJun(Condicao cond, Cabecalho *c1, Cabecalho *c2)
{
    int patr;
    Atributo atr1, atr2;

    /* Vari�veis: patr -> armazena a posi��o de cada atributo na lista
     de atributos correspondente.
     atr1 -> armazena o atributo correspondente a <c1> (rela��o 1).
     atr2 -> armazena o atributo correspondente a <c2> (rela��o 2).*/

    patr = posatr(cond.atr1, c1);
    natr(c1, patr, &atr1);
    patr = posatr(cond.atr2, c2);
    natr(c2, patr, &atr2);
    if (atr1.tipo != atr2.tipo)
        return ATRS_TIPOS_DIF; /*Verifica se os atributos s�o de*/
    return 0; /*tipos compat�veis.*/
}

int validaListaAtrib(char **listaAtrib, char *tab1, char *tab2, Cabecalho *c1,
                     Cabecalho *c2)
{
    int i, j, jNome, ind, achouA, achouB, iAtrib;
    char atrib[60], nome[30], temp[30], temp2[100], listaNormalizada[200];
    Atributo atr, cabA[10], cabB[10];

    int existe1, existe2, n, m;

    if (!strcasecmp(*listaAtrib, "*"))
        return 0;

    j = 0;
    temp[0] = 0;
    listaNormalizada[0] = 0;
    do
    {
        /* pega um atributo */
        i = 0;
        nome[0] = 0;
        atrib[0] = 0;
        do
        {
            ++i;
        }
        while (((*listaAtrib)[j + i] != ',') && ((*listaAtrib)[j + i] != 0));
        strncpy(atrib, &((*listaAtrib)[j]), i);
        atrib[i] = 0;
        j += i;
        /* fim */

        /* pega o nome da tabela e o atributo, caso o mesmo seja referenciado pela tabela */
        iAtrib = 0;
        do
        {
            ++iAtrib;
        }
        while ((atrib[iAtrib] != '.') && (atrib[iAtrib] != 0));
        if (atrib[iAtrib] == '.')
        {
            strncpy(nome, atrib, iAtrib);
            nome[iAtrib] = 0;

            ++iAtrib;
            jNome = iAtrib;
            iAtrib = 0;
            do
            {
                ++iAtrib;
            }
            while (atrib[jNome + iAtrib] != 0);
            strncpy(temp, &atrib[jNome], iAtrib);
            temp[iAtrib] = 0;
            strcpy(atrib, temp);
            atrib[iAtrib] = 0;

            if (!strcasecmp(nome, tab1))
            {
                if (!posatr(atrib, c1))
                    return ATR_INEXISTENTE;
                else
                {
                    if (listaNormalizada[0])
                        strcat(listaNormalizada, ",");
                    if ((c2 != NULL) && posatr(atrib, c2))
                    {
                        strcat(listaNormalizada, nome);
                        strcat(listaNormalizada, ".");
                    }
                    strcat(listaNormalizada, atrib);
                    continue;
                }
            }
            else if ((c2 != NULL) && !strcasecmp(nome, tab2))
            {
                if (!posatr(atrib, c2))
                    return ATR_INEXISTENTE;
                else
                {
                    if (listaNormalizada[0])
                        strcat(listaNormalizada, ",");
                    if (posatr(atrib, c1))
                    {
                        strcat(listaNormalizada, nome);
                        strcat(listaNormalizada, ".");
                    }
                    strcat(listaNormalizada, atrib);
                    continue;
                }
            }
            else
                return TAB_INEXISTENTE;
        }
        /* fim */

        /* verifica se esse atributo existe na tabela */
        achouA = posatr(atrib, c1);
        if (c2 == NULL)
            achouB = 0;
        else
            achouB = posatr(atrib, c2);

        if (!(achouA || achouB))
            return ATR_INEXISTENTE;
        if (achouA && achouB)
            return ATR_N_QUALIFIC;

        if (listaNormalizada[0])
            strcat(listaNormalizada, ",");
        strcat(listaNormalizada, atrib);
    }
    while ((*listaAtrib)[j++] != 0);

    strcpy(*listaAtrib, listaNormalizada);
    return 0;
}

void geraCTL(FILE *stream, int n, int m, Cabecalho *c)
{
    Cabecalho *p;
    /* Vari�veis: p -> ponteiro utilizado p/ percorrer <c>. */
    fprintf(stream, "%d,%d\n", n, m);
    p = c;
    Referenciada *ref = NULL;
    int qnt = 0;
    do
    {
        fprintf(stream, "%s,", p->atributo.nome);
        if (p->atributo.tipo == INTEGER)
            fprintf(stream, "I");
        else
            fprintf(stream, "C");
        if (p->atributo.ord)
            fprintf(stream, ",ord");
        if (p->atributo.key)
        {
            fprintf(stream, ",chv");
            ref = p->atributo.f_referenciada;
            qnt = p->atributo.conta_ref;
        }
        if (p->atributo.fkey)
            fprintf(stream, ",fk:%s:%s", p->atributo.f_referencia,
                    (p->atributo.tipo_ref == RESTRICT ? "restr" : "casc"));

        fprintf(stream, "\n");

        p = p->next;
    }
    while (p != c);

    if (qnt > 0)
    {
        Referenciada *r = ref;
        do
        {
            fprintf(stream, "%s", r->nome);
            r = r->next;
            if (r != ref)
                fprintf(stream, ",");
        }
        while (r != ref);
        fprintf(stream, "\n");
    }
}

void geraDAD(FILE *stream, Relacao *r)
{
    Relacao *tups;
    Tupla *t;

    /* Vari�veis: tups -> ponteiro utilizado p/ percorrer <tl>.
     t   -> ponteiro utilizado p/ percorrer <tl>. */

    if (r)
    {
        tups = r;
        do
        {
            fprintf(stream, "%s", tups->tupla->val);
            t = tups->tupla->next;
            while (t != tups->tupla)
            {
                fprintf(stream, ",%s", t->val);
                t = t->next;
            }
            fprintf(stream, "\n");
            tups = tups->next;
        }
        while (tups != r);
    }
}

void imprimeCabecalho(Cabecalho *c, int l)
{
    Cabecalho *p;
    int i;
    char temp[20];

    /* Vari�veis: p -> ponteiro utilizado p/ percorrer <c>.
     i -> indice.
     temp -> armazena nome das colunas.*/

    if (c)
    {
        p = c;
        do /*Escreve os nomes dos atributos separados por '|' e alinhados*/
        {
            /*� esquerda, ocupando <l> caracteres.*/
            if (strchr(p->atributo.nome, '.'))
            {
                i = 0;
                while (p->atributo.nome[i] != '.')
                {
                    i++;
                }
                i++;
                strcpy(temp, &p->atributo.nome[i]);
            }
            else
                strcpy(temp, p->atributo.nome);

            printf("|%-*.*s", l, l, temp);
            p = p->next;
        }
        while (p != c);
        printf("\n");
    }
}

void imprimeTupla(Tupla *t, int l)
{
    Tupla *p;

    /* Vari�veis: p -> ponteiro utilizado p/ percorrer <vl>. */

    if (t)
    {
        p = t;
        do /*Escreve os valores da tupla separados por '|' e alinhados*/
        {
            /*� esquerda, ocupando <fsize> caracteres.*/
            printf("|%-*.*s", l, l, p->val);
            p = p->next;
        }
        while (p != t);
        printf("\n");
    }
}

void imprimeTabela(char *nome, Tabela t,int isCount,int isSum,int isMin, int isMax,int isAvg)
{
    int i, w, l, nwin;
    Relacao *p;

    /* Vari�veis: i     -> vari�vel de controle de loop; contador.
     w     -> armazena o n�mero da janela exibida em cada instante.
     l -> largura das colunas da tabela.
     nwin  -> n� de janelas necess�rias p/ exibir a tabela.
     p     -> ponteiro utilizado p/ percorrer <tl>. */

    if (t.m == 0) /*Se n�o h� nenhuma tupla na rela��o*/
    {
        printf("%s:\n", nome);
        printf("Nenhuma tupla na relacao.\n");
        /* system("pause");*/
    }
    /*isCount 2010.2*/
    if(isAvg)
    {
        /*2010.2*/
        nwin = (int) (t.m / 19) + 1;
        l = (int) (79 / t.n) - 1;
        w=0;
        printf("%s (%d/%d):\n", nome, w, nwin);/*escreve o nome da rela��o
				 e o n� da janela,*/
        for (i = 1; i <= 79; i++)
            printf("-");
        printf("\n");

        p = t.relacao;
        char teste[100];
        strcpy(teste,p->tupla->val);
        int isNumeric=1;

        /*Verifica se valores selecionados s�o num�ricos, sen�o n�o � poss�vel executar Min*/
        i=0;
        while(i<strlen(teste) && isNumeric)
        {
            if(!isdigit(teste[i]))
                isNumeric=0;
            i++;
        }


        if(isNumeric)
        {
            float average = 0;
            int total = 0;

            do
            {
                float valorTeste = atof(p->tupla->val);
                average += valorTeste;
                total++;
                p = p->next;
            }
            while(p != t.relacao);

            average = average / total;

            imprimeCabecalho(t.cabecalho, l); /*escreve o cabe�alho da tabela,*/
            printf("O valor da media eh %.2f\n\n\n",average);
        }
        else
        {
            printf("Impossivel calcular a media, valor nao numerico");
        }

    }
    else if(isSum)
    {
        /*2010.2*/
        nwin = (int) (t.m / 19) + 1;
        l = (int) (79 / t.n) - 1;
        w=0;
        printf("%s (%d/%d):\n", nome, w, nwin);/*escreve o nome da rela��o
				 e o n� da janela,*/
        for (i = 1; i <= 79; i++)
            printf("-");
        printf("\n");

        p = t.relacao;
        char teste[100];
        strcpy(teste,p->tupla->val);
        int isNumeric=1;

        /*Verifica se valores selecionados s�o num�ricos, sen�o n�o � poss�vel executar Min*/
        i=0;
        while(i<strlen(teste) && isNumeric)
        {
            if(!isdigit(teste[i]))
                isNumeric=0;
            i++;
        }


        if(isNumeric)
        {
            float soma = 0;

            do
            {
                float valorTeste = atof(p->tupla->val);
                soma += valorTeste;
                p = p->next;
            }
            while(p != t.relacao);

            imprimeCabecalho(t.cabecalho, l); /*escreve o cabe�alho da tabela,*/
            printf("O valor da soma eh %.2f\n\n\n",soma);
        }
        else
        {
            printf("Impossivel calcular a soma, valor nao numerico");
        }
    }
    else if(isCount)
    {
        nwin = (int) (t.m / 19) + 1;
        w=0;
        printf("%s (%d/%d):\n", nome, w, nwin);/*escreve o nome da rela��o
				 e o n� da janela,*/
        for (i = 1; i <= 79; i++)
            printf("-");
        printf("\n");

        p = t.relacao;
        int Soma = 0;
        do
        {
            Soma++;
            p= p->next;
        }
        while(p != t.relacao);

        printf("Numero de elementos: %d \n\n",Soma);
    }
    else if(isMin)
    {
        /*2010.2*/
        nwin = (int) (t.m / 19) + 1;
        l = (int) (79 / t.n) - 1;
        w=0;
        printf("%s (%d/%d):\n", nome, w, nwin);/*escreve o nome da rela��o
				 e o n� da janela,*/
        for (i = 1; i <= 79; i++)
            printf("-");
        printf("\n");

        p = t.relacao;
        char teste[100];
        strcpy(teste,p->tupla->val);
        int isNumeric=1;

        /*Verifica se valores selecionados s�o num�ricos, sen�o n�o � poss�vel executar Min*/
        i=0;
        while(i<strlen(teste) && isNumeric)
        {
            if(!isdigit(teste[i]))
                isNumeric=0;
            i++;
        }


        if(isNumeric)
        {
            float Min = atof(p->tupla->val);
            p = p->next;

            while(p != t.relacao)
            {
                float valorTeste = atof(p->tupla->val);
                if(valorTeste < Min)
                    Min = valorTeste;
                p = p->next;
            }

            imprimeCabecalho(t.cabecalho, l); /*escreve o cabe�alho da tabela,*/
            printf("O menor valor do atributo eh %.2f\n\n\n",Min);
        }
        else
        {
            printf("Impossivel calcular minimo, valor nao numerico");
        }
    }
    else if(isMax)
    {
        /*2010.2*/
        nwin = (int) (t.m / 19) + 1;
        l = (int) (79 / t.n) - 1;
        w=0;
        printf("%s (%d/%d):\n", nome, w, nwin);/*escreve o nome da rela��o
				 e o n� da janela,*/
        for (i = 1; i <= 79; i++)
            printf("-");
        printf("\n");

        p = t.relacao;
        char teste[100];
        strcpy(teste,p->tupla->val);
        int isNumeric=1;

        /*Verifica se valores selecionados s�o num�ricos, sen�o n�o � poss�vel executar Min*/
        i=0;
        while(i<strlen(teste) && isNumeric)
        {
            if(!isdigit(teste[i]))
                isNumeric=0;
            i++;
        }


        if(isNumeric)
        {
            float Max = atof(p->tupla->val);
            p = p->next;

            while(p != t.relacao)
            {
                float valorTeste = atof(p->tupla->val);
                if(valorTeste > Max)
                    Max = valorTeste;
                p = p->next;
            }

            imprimeCabecalho(t.cabecalho, l); /*escreve o cabe�alho da tabela,*/
            printf("O maior valor do atributo eh %.2f\n\n\n",Max);
        }
        else
        {
            printf("Impossivel calcular minimo, valor nao numerico");
        }
    }
    else
    {
        nwin = (int) (t.m / 19) + 1; /*Calcula o n� de janelas necess�rias,
		 considerando 19 linhas (tuplas) por janela.*/
        l = (int) (79 / t.n) - 1; /*Calcula o tamanho dos campos. Cada linha da
		 janela tem 80 caracteres. 1 � reservado p/
		 quebra de linha. Os demais s�o divididos
		 igualmente pelo n� de atributos.*/
        i = 20; /*<i> guarda o n� de tupla exibidas na janela atual menos 1.*/
        w = 0; /*<w> guarda o n� da janela atual.*/
        p = t.relacao;
        if (t.m != 0) {
            do
            {
                if (i > 19) /*Se vai come�ar a exibir uma janela,*/
                {
                    w++; /*atualiza o n� da janela,*/
                    printf("%s (%d/%d):\n", nome, w, nwin);/*escreve o nome da rela��o
                    e o n� da janela,*/
                    for (i = 1; i <= 79; i++)
                        printf("-");
                    printf("\n");
                    imprimeCabecalho(t.cabecalho, l); /*escreve o cabe�alho da tabela,*/
                    for (i = 1; i <= 79; i++)
                        printf("-");
                    printf("\n");
                    i = 1; /*e reinicia o contador de linhas.*/
                }
                imprimeTupla(p->tupla, l); /*Escreve cada linha (tupla) da tabela.*/
                i++; /*Atualiza o contador de linhas.*/
                /* if (i > 19) system("pause");*//*Se encerrou uma janela, pausa.*/
                p = p->next;
            }
            while (p != t.relacao);
        } 
        /* system("pause");*/
    }
}

int isint(char *str)
{
    int length, i;

    /* Vari�veis: length -> tamanho de <str>.
     i      -> vari�vel de controle de loop. */

    length = strlen(str);
    for (i = 0; i < length; i++)
    {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

int isstr(char *str)
{
    int length;

    /* Vari�veis: length -> tamanho de <str>. */

    length = strlen(str);
    if (str[0] != '"')
        return 0; /*Verifica se o 1� caractere � ".*/
    if (str[length - 1] != '"')
        return 0; /*Verifica se o �ltimo caractere � ".*/
    return 1;
}

void removeAspas(char *str)
{
    int length, i;

    /* Vari�veis: length -> tamanho de <str>.
     i      -> vari�vel de controle de loop. */

    length = strlen(str);
    for (i = 1; i < (length - 1); i++)
    {
        str[i - 1] = str[i]; /*Desloca todos os caracteres 1 posi��o � esquerda.*/
    }
    str[length - 2] = '\0'; /*Elimina o �ltimo caractere.*/
}

/* Coloca em <alg> o 1o. nome de arquivo .alg que n�o estiver sendo
 utilizado.
 Formato de nome de arquivo .alg: query_XX.alg,
 onde XX � um n�mero hexadecimal.*/
void geraNomeAlg(char *alg)
{
    char hex[3];
    int novo;
    FILE *algstream;

    /* Vari�veis: hex       -> string que representa um n�mero hexadecimal.
     novo     -> se o nome escolhido for novo, novo == 1.
     Se j� estiver sendo utilizado, novo == 0.
     algstream -> stream de arquivo de opera��es da �lgebra relacional. */

    strcpy(alg, "query_");
    strcpy(hex, "00");
    strcat(alg, hex);
    strcat(alg, ".alg");
    novo = 0;
    while (!novo) /*Testa nomes de arquivos .alg p/ n�o*/
    {
        /*sobrescrever os existentes.*/
        if ((algstream = fopen(alg, "r")) != NULL)
        {
            fclose(algstream);
            if (hex[1] == '9')
                hex[1] = 'A';
            else if (hex[1] == 'F')
            {
                hex[1] = '0';
                if (hex[0] == '9')
                    hex[0] = 'A';
                else if (hex[0] == 'F')
                {
                    hex[0] = '0'; /*Se todos os nomes est�o sendo utilizados,*/
                    novo = 1; /*reutiliza o 1�.*/
                }
                else
                    hex[0]++;
            }
            else
                hex[1]++;
            strcpy(alg, "query_");
            strcat(alg, hex);
            strcat(alg, ".alg");
        }
        else
            novo = 1;
    }
}

int atrtab(char *atributo, char *tab1, char *tab2, Cabecalho *al1,
           Cabecalho *al2)
{
    void strToUpper(char *str);
    /* Passa uma string para seu equivalente c/ letras mai�sculas.
     (Implementada em <Estruturas.c>) */

    int temp, len1, len2, lenA, i;
    char t1[20], t2[20];

    /* Vari�veis: temp -> armazena o valor de retorno.
     len1 -> tamanho do nome da rela��o 1.
     len2 -> tamanho do nome da rela��o 2.
     lenA -> tamanho do nome do atributo.
     i    -> vari�vel de controle de loop.
     t1   -> armazena o nome da rela��o 1 em caixa alta.
     t2   -> armazena o nome da rela��o 2 em caixa alta. */

    temp = 0;
    if (strchr(atributo, '.')) /*Se h� um '.' em <atributo>, h� uma qualifica��o.*/
    {
        strcpy(t1, tab1);
        strcpy(t2, tab2);
        strToUpper(t1);
        strToUpper(t2);

        len1 = strlen(t1);
        len2 = strlen(t2);
        lenA = strlen(atributo);
        if (!strncmp(atributo, t1, len1))
            temp = 1; /*Se est� qualificado c/ <tab1>*/
        else if (!strncmp(atributo, t2, len2))
            temp = 2;
        /*Se est� qualificado c/ <tab2>*/
        if (temp == 1) /*Verifica se realmente est� em <tab1>.*/
        {
            for (i = (len1 + 1); i <= lenA; i++) /*Apaga o nome da tabela.*/
                atributo[i - (len1 + 1)] = atributo[i];
            if (!posatr(atributo, al1))
                temp = 0;
        }
        else if (temp == 2)   /*Verifica se realmente est� em <tab2>.*/
        {
            for (i = (len2 + 1); i <= lenA; i++) /*Apaga o nome da tabela.*/
                atributo[i - (len2 + 1)] = atributo[i];
            if (!posatr(atributo, al2))
                temp = 0;
        }
    }
    else   /*Se n�o h� qualifica��o*/
    {
        if (posatr(atributo, al1))
            temp = 1; /*Se est� em <tab1>*/
        if (posatr(atributo, al2))
            temp += 2; /*Se est� em <tab2>*/
    }
    return temp;
}

void swap(char *str1, char *str2)
{
    char temp[20];

    /* Vari�veis: temp -> armazena o valor de <atr1> temporariamente. */

    strcpy(temp, str1);
    strcpy(str1, str2);
    strcpy(str2, temp);
}

int contaVirgulas(char* s)
{
    int i = 0;
    int contador = 0;
    while (s[i])
    {
        if (s[i] == ',')
            contador++;
        i++;
    }
    return contador;
}
