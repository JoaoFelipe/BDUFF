/********************************* ErrMsgs.c *********************************/

#include <stdio.h>
#include "ErrMsgs.h"

void erroSintaxe (int numErro, int comando){
     printf("Erro st%d no comando %d: ", numErro, comando);
     switch(numErro)
     {
        case PNTVRG_FALTANDO: printf("';' faltando.\n");
                break;
        case CMD_DESCONHECIDO: printf("comando desconhecido.\n");
                break;
        case PAR_ESQ_FALTANDO: printf("'(' nao encontrado.\n");
                break;
        case NOMEATR_FALTANDO: printf("nome do atributo faltando.\n");
                break;
        case TIPOATR_FALTANDO: printf("tipo do atributo faltando.\n");
                break;
        case TIPO_INVALIDO: printf("tipo invalido.\n");
                break;
        case IDT_DESCONHECIDO: printf("identificador desconhecido.\n");
                break;
        case ESPEC_DUPLICADA: printf("especificacao duplicada.\n");
				break;
        case PARAM_EXCEDENTES: printf("excesso de parametros.\n");
				break;
        case VALUES_FALTANDO: printf("'values' nao encontrado.\n");
				break;
        case FROM_FALTANDO: printf("'FROM' nao encontrado.\n");
				break;
        case 12: printf("caractere invalido no comando.\n");
				break;
        case WHERE_FALTANDO: printf("'WHERE' nao encontrado.\n");
				break;
        case COMPARACAO_INVLD: printf("Formato de comparacao invalido.\n");
				break;
        case JOIN_FALTANDO: printf("'JOIN' nao encontrado.\n");
				break;
        case ON_FALTANDO: printf("'ON' nao encontrado.\n");
				break;
        /*2010.2*/
        case COUNT_ERRADO: printf("Sintaxe de Count errada.\n");
				break;
		case MIN_ERRADO: printf("Sintaxe de Min errada.");
		        break;
        case MAX_ERRADO: printf("Sintaxe de Max errada.");
                break;
        case SUM_ERRADO: printf("Sintaxe de Sum errada.");
                break;
        case AVG_ERRADO: printf("Sintaxe do Avg errada.");
                break;
     }
     system("pause");
     exit(1);
}

void erroCT (int numErro, char *tab)
{
     printf("Erro ct%d: ", numErro);
     switch(numErro)
     {
        case TABELA_JA_EXISTE: printf("a tabela %s ja existe.\n", tab);
                break;
        case CTL_NAO_CRIADO: printf("o arquivo %s.ctl nao pode ser criado.\n", tab);
                break;
        case DAD_NAO_CRIADO: printf("o arquivo %s.dad nao pode ser criado.\n", tab);
                break;
        case NENHUM_ATRIBUTO: printf("nenhum atributo definido p/ a tabela %s.\n", tab);
                break;
        case ATRIB_DUPLICADO: printf("atributos duplicados na tabela %s.\n", tab);
                break;
        case CHAVE_INDEFINIDA: printf("nenhuma chave definida p/ a tabela %s.\n", tab);
                break;
        case MAIS_QUE_UM_CHV: printf("mais de uma chave definida p/ a tabela %s.\n", tab);
                break;
        case MAIS_QUE_UM_FCHV: printf("mais de uma chave estrangeira definida p/ a tabela %s.\n", tab);
                break;
        case FK_TABELA_INEXISTENTE: printf("a tabela %s foi referenciada mas é inexistente .\n",tab);
                break;
        case FK_TIPO_ERRADO: printf("atributo referenciado a tabela %s de tipo diferente a chave estrangeira definida.\n",tab);
                break;
        case MAIS_QUE_UM_ORD: printf("mais de um atributo definido c/ a clausula [ORD]");
                printf(" p/ a tabela %s.\n", tab);
     }
     system("pause");
}

void erroInsert (int numErro, char *tab)
{
     printf("Erro ins%d: ", numErro);
     switch(numErro)
     {
        case TAB_INEXISTENTE: printf("a tabela %s nao existe.\n", tab);
                break;
        case ERRO_ABRIR_DAD: printf("o arquivo %s.dad nao pode ser aberto.\n", tab);
                break;
        case N_VALS_INCORRETO: printf("no. de valores incompativel c/ a tabela %s.\n", tab);
                break;
        case TIPO_INCORRETO: printf("tipo(s) incompativel(is) c/ a tabela %s.\n", tab);
                break;
        case VIOLACAO_CHAVE: printf("restricao de chave primaria violada na tabela");
                printf(" %s.\n", tab); break;
        case ERRO_CRIAR_CTL: printf("o arquivo %s.ctl nao pode ser criado.\n", tab);
                break;
        case ERRO_CRIAR_DAD: printf("o arquivo %s.dad nao pode ser criado.\n", tab);
             break;
        case ERRO_REFERENCIAL: printf("erro referencial.\n",tab);
     }
     system("pause");
}

void erroDelete (int numErro, char *tab)
{
     printf("Erro del%d: ", numErro);
     switch(numErro)
     {
        case TAB_INEXISTENTE: printf("a tabela %s nao existe.\n", tab);
                break;
        case ERRO_ABRIR_DAD_D: printf("o arquivo %s.dad nao pode ser aberto.\n", tab);
                break;
        case ATR_INEXISTENTE: printf("o atributo %s nao existe na tabela %s.\n", tab);
                break;
        case TIPO_INCORRETO_S: printf("valor de tipo incompativel c/ o atributo");
                printf(" da tabela %s.\n", tab); break;
        case ERRO_CRIAR_CTL: printf("o arquivo %s.ctl nao pode ser criado.\n", tab);
                break;
        case VIOLACAO_CHAVE: printf("restricao de chave primaria violada na tabela");
                printf(" %s.\n", tab); break;
        case ERRO_CRIAR_DAD: printf("o arquivo %s.dad nao pode ser criado.\n", tab);
     }
     system("pause");
}

void erroUpdate (int numErro, char *tab)
{
     printf("Erro upd%d: ", numErro);
     switch(numErro)
     {
        case TAB_INEXISTENTE: printf("a tabela %s nao existe.\n", tab);
                break;
        case ERRO_ABRIR_DAD_D: printf("o arquivo %s.dad nao pode ser aberto.\n", tab);
                break;
        case ATR_INEXISTENTE: printf("o atributo %s nao existe na tabela %s.\n", tab);
                break;
        case TIPO_INCORRETO_S: printf("valor de tipo incompativel c/ o atributo");
                printf(" da tabela %s.\n", tab); break;
        case ERRO_CRIAR_CTL: printf("o arquivo %s.ctl nao pode ser criado.\n", tab);
                break;
        case ERRO_CRIAR_DAD: printf("o arquivo %s.dad nao pode ser criado.\n", tab);
                break;
        case ERRO_REFERENCIA_UP:printf("Chave estrangeira na tabela %s.\n",tab);
     }
     system("pause");
}

void erroSelect (int numErro, char *tab, char *atr)
{
     printf("Erro sel%d: ", numErro);
     switch(numErro)
     {
        case TAB_INEXISTENTE: printf("a tabela %s nao existe.\n", tab);
                break;
        case ATR_INEXISTENTE: printf("o atributo %s nao existe na tabela %s.\n", atr, tab);
                break;
        case TIPO_INCORRETO_S: printf("valor de tipo incompativel c/ o atributo");
                printf(" %s da tabela %s.\n", atr, tab); break;
        case 4: printf("o arquivo %s nao pode ser criado.\n", tab);
                break;
        case 5: printf("o arquivo %s.ctl nao pode ser aberto.\n", tab);
                break;
        case 6: printf("o arquivo %s.dad nao pode ser aberto.\n", tab);
                break;
        case 7: printf("o atributo ocorre nas 2 relacoes e nao esta qualificado.\n");
     }
     system("pause");
}

void erroJoin (int numErro, char *tab1, char *tab2, Condicao cond)
{
     printf("Erro jn%d: ", numErro);
     switch(numErro)
     {
        case TAB_INEXISTENTE: printf("a tabela %s nao existe.\n", tab1);
                break;
        case ATR_INEXISTENTE: printf("o atributo %s", cond.atr1);
                printf(" nao ocorre na relacao especificada.\n"); break;
        case 3: printf("o atributo %s ocorre nas relacoes", cond.atr1);
                printf(" %s e %s e nao esta qualificado.\n", tab1, tab2); break;
        case 4: printf("o atributo %s", cond.atr2);
                printf(" nao ocorre na relacao especificada.\n"); break;
        case 5: printf("o atributo %s ocorre nas relacoes", cond.atr2);
                printf(" %s e %s e nao esta qualificado.\n", tab1, tab2); break;
        case 6: printf("os atributos %s e %s", cond.atr1, cond.atr2);
                printf(" pertencem a mesma relacao.\n"); break;
        case 7: printf("os atributos %s e %s", cond.atr1, cond.atr2);
                printf(" sao de tipos diferentes.\n");
     }
     system("pause");
}
