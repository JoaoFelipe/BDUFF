/********************************* ErrMsgs.h *********************************/

/* Mensagens de erro.*/

#include <stdlib.h>
#include "Estruturas.h"

#define PNTVRG_FALTANDO  1
#define CMD_DESCONHECIDO 2
#define PAR_ESQ_FALTANDO 3
#define NOMEATR_FALTANDO 4
#define TIPOATR_FALTANDO 5
#define TIPO_INVALIDO    6
#define IDT_DESCONHECIDO 7
#define ESPEC_DUPLICADA  8
#define PARAM_EXCEDENTES 9
#define VALUES_FALTANDO  10
#define FROM_FALTANDO    11
#define WHERE_FALTANDO   13
#define COMPARACAO_INVLD 14
#define JOIN_FALTANDO    15
#define ON_FALTANDO      16
/*2010.2*/
#define COUNT_ERRADO     17
#define MIN_ERRADO       18
#define MAX_ERRADO       19
#define SUM_ERRADO       20
#define AVG_ERRADO       21

#define TABELA_JA_EXISTE 1
#define CTL_NAO_CRIADO   2
#define DAD_NAO_CRIADO   3
#define NENHUM_ATRIBUTO  4
#define ATRIB_DUPLICADO  5
#define CHAVE_INDEFINIDA 6
#define MAIS_QUE_UM_CHV  7
#define MAIS_QUE_UM_FCHV 8
#define MAIS_QUE_UM_ORD  9
#define FK_TABELA_INEXISTENTE 10
#define FK_TIPO_ERRADO   11

#define TAB_INEXISTENTE  1
#define ERRO_ABRIR_DAD   2
#define N_VALS_INCORRETO 3
#define TIPO_INCORRETO   4
#define VIOLACAO_CHAVE   5
#define ERRO_CRIAR_CTL   6
#define ERRO_CRIAR_DAD   7

#define TAB_INEXISTENTE  1
#define ATR_INEXISTENTE  2
#define TIPO_INCORRETO_S 3
#define ERRO_CRIAR_ARQ   4
#define ERRO_ABRIR_CTL   5
#define ERRO_ABRIR_DAD_S 6
#define ATR_N_QUALIFIC   7
#define ERRO_REFERENCIAL 8

#define TAB_INEXISTENTE  1
#define ATR1_INEXISTENTE 2
#define ATR1_N_QUALIFIC  3
#define ATR2_INEXISTENTE 4
#define ATR2_N_QUALIFIC  5
#define ATRS_MESMA_REL   6
#define ATRS_TIPOS_DIF   7

#define ERRO_ABRIR_DAD_D 8

#define ERRO_REFERENCIA_UP 9

#define SET_FALTANDO     8


/* Descrição: imprime mensagens de erro relativas à sintaxe.
   Parâmetros: numErro -> código do erro.
   			   comando -> comando (número) em que ocorreu o erro. */
void erroSintaxe (int numErro, int comando);

/* Descrição: imprime mensagens de erro relativas ao comando CREATE TABLE.
   Parâmetros: numErro -> código do erro.
   			   tab -> nome da tabela cujo comando de criação gerou o erro. */
void erroCT (int numErro, char *tab);

/* Descrição: imprime mensagens de erro relativas ao comando INSERT.
   Parâmetros: numErro -> código do erro.
   			   tab -> nome da tabela em que ocorreu o erro. */
void erroInsert (int numErro, char *tab);

/* Descrição: imprime mensagens de erro relativas ao comando DELETE.
   Parâmetros: numErro -> código do erro.
   			   tab -> nome da tabela em que ocorreu o erro. */
void erroDelete (int numErro, char *tab);

/* Descrição: imprime mensagens de erro relativas ao comando DELETE.
   Parâmetros: numErro -> código do erro.
   			   tab -> nome da tabela em que ocorreu o erro. */
void erroUpdate (int numErro, char *tab);

/* Descrição: imprime mensagens de erro relativas ao comando UPDATE.
   Parâmetros: numErro -> código do erro.
   			   tab -> tabela em que ocorreu o erro.
   			   atr -> atributo referente ao erro. */
void erroSelect (int numErro, char *tab, char *atr);

/* Descrição: imprime mensagens de erro relativas ao comando SELECT c/ junção.
   Parâmetros: numErro -> código do erro.
   			   tab1 -> primeira tabela da junção.
   			   tab2 -> segunda tabela da junção.
   			   cond -> condição de junção. */
void erroJoin (int numErro, char *tab1, char *tab2, Condicao cond);
