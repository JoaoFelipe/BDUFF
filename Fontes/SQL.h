/********************************** sqlopr.h **********************************/

/* Implementação das operações SQL. */

#include <stdio.h>
#include "AlgRel.h"


/***** Funções que executam os comandos SQL *****/


/* Descrição: cria os arquivos de dados e meta-dados para uma relação.
   Parâmetros: <nome> é o nome da tabela a ser criada;
               <c> é um ponteiro para uma lista c/ os atributos
                    definidos p/ a tabela.
   Valor de retorno: retorna 0 caso não ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int createTable (char *nome, Cabecalho *c);

/* Descrição: executa o comando "INSERT".
   Parâmetros: <tab> é o nome da tabela onde os valores devem ser inseridos;
               <vl> é um ponteiro para uma lista c/ os valores a serem
                    inseridos (tupla).
   Valor de retorno: retorna 0 caso não ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int insert (char *tab, Tupla *vl);

/* Descrição: executa o comando "DELETE" com condição.
   Parâmetros: <tab> é o nome da tabela;
               <atrib> é o atributo da condição.
               <valor> é o valor da condição.
   Valor de retorno: retorna 0 caso não ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int deleteCond(char *tab, char *atrib, char *valor);

/* Descrição: executa o comando "DELETE" sem condição.
   Parâmetros: <tab> é o nome da tabela;
   Valor de retorno: retorna 0 caso não ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int deleteAll(char *tab);

/* Descrição: executa o comando "UPDATE" com condição.
   Parâmetros: <tab> é o nome da tabela;
               <atribSet> é o atributo a ser atualizado.
               <valorSet> é o valor a ser inserido.
               <atribWhere> é o atributo da condição.
               <valorWhere> é o valor da condição.
   Valor de retorno: retorna 0 caso não ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int updateCond(char *tab, char *atribSet, char *valorSet, char *atribWhere, char *valorWhere);

/* Descrição: executa o comando "UPDATE" sem condição.
   Parâmetros: <tab> é o nome da tabela;
               <atrib> é o atributo a ser atualizado.
               <valor> é o valor a ser inserido.
   Valor de retorno: retorna 0 caso não ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int updateAll(char *tab, char *atrib, char *valor);

/* Descrição: cria um arquivo c/ a operação algébrica necessária para
              execução de um comando "SELECT" (sem "JOIN"), chama a
              função que interpreta e executa tal operação, e exibe a
              relação resultante na tela.
   Parâmetros: <tab> é o nome da tabela onde será feita a seleção;
               <cond> é um ponteiro para a condição de seleção.
               <listaAtrib> é a string que contém a lista de atributos da projeção.
   Valor de retorno: retorna 0 caso não ocorra nenhum erro.
                     Retorna 1 em caso de erro.
   Observações: se não houver condição de seleção,
                o valor de <cond> deve ser NULL.


                Atualização 2010.2
                isCount,isSum,isMin,isMax,isAvg*/

int selectSQL(char *tab, char *listaAtrib, Condicao *cond,int isDistinct, int isOrderBy,int isCount,int isSum,int isMin, int isMax,int isAvg);

/* Descrição: cria um arquivo c/ a seqüência de operações algébricas
              necessárias para execução de um comando "SELECT" (com "JOIN"),
              chama a função que interpreta e executa tais operações,
              e exibe a relação resultante na tela.
   Parâmetros: <tab1> é o nome da 1ª tabela participante da junção;
               <tab2> é o nome da 2ª tabela participante da junção;
               <listaAtrib> é a string que contém a lista de atributos da projeção.
               <jcond> é a condição de junção;
               <scond> é um ponteiro para a condição de seleção.
   Valor de retorno: retorna 0 caso não ocorra nenhum erro.
                     Retorna 1 em caso de erro.
   Observações: se não houver condição de seleção,
                o valor de <scond> deve ser NULL. */
int join (char *tab1, char *tab2, char *listaAtrib, Condicao jcond, Condicao *scond);


/***** Funções de verificação de erros *****/


/* Descrição: verifica se há erros em uma lista de atributos.
   Parâmetros: <c> é um ponteiro para a lista de atributos a ser verificada.
   Valor de retorno: retorna 0 caso não haja erros. Retorna um valor
                     positivo em caso de erro (vide errmsgs.c). */
int validaCabecalho (Cabecalho *c);

/* Descrição: verifica se há erros em uma lista de valores (tupla).
   Parâmetros: <tup> é um ponteiro para a lista de valores a ser verificada;
               <tab> é a tabela para referência.
   Valor de retorno: retorna 0 caso não haja erros. Retorna um valor
                     positivo em caso de erro (vide errmsgs.c). */
int validaTupla (Tupla *tup, Tabela tab);

/* Descrição: verifica se há erros em uma condição de seleção.
   Parâmetros: <cond> é a condição a ser verificada;
               <c> é um ponteiro para uma lista c/ os atributos da
                    relação à qual se refere <cond>.
   Valor de retorno: retorna 0 caso não haja erros. Retorna um valor
                     positivo em caso de erro (vide errmsgs.c). */
int validaCondSel (Condicao cond, Cabecalho *c);

/* Descrição: verifica se há erros em uma condição de junção.
   Parâmetros: <cond> é a condição a ser verificada;
               <c1> é um ponteiro para uma lista c/ os atributos da
                     1ª relação à qual se refere <cond>;
               <c2> é um ponteiro para uma lista c/ os atributos da
                     2ª relação à qual se refere <cond>.
   Valor de retorno: retorna 0 caso não haja erros. Retorna um valor
                     positivo em caso de erro (vide errmsgs.c). */
int validaCondJun (Condicao cond, Cabecalho *c1, Cabecalho *c2);

/* Descrição: verifica se há erros em uma lista de atributos.
   Parâmetros: <listaAtrib> é a lista a ser verificada;
               <tab1> é o nome da 1ª relação à qual se refere <listaAtrib>;
               <tab2> é o nome da 2ª relação (se houver) à qual se refere <listaAtrib>.
               <c1> é um ponteiro para uma lista c/ os atributos da
                     1ª relação à qual se refere <listaAtrib>;
               <c2> é um ponteiro para uma lista c/ os atributos da
                     2ª relação (se houver) à qual se refere <listaAtrib>.
   Valor de retorno: retorna 0 caso não haja erros. Retorna um valor
                     positivo em caso de erro (vide errmsgs.c). */
int validaListaAtrib(char **listaAtrib, char *tab1, char *tab2, Cabecalho *c1, Cabecalho *c2);


/***** Funções de manipulação de arquivos *****/


/* Descrição: gera o arquivo de meta-dados (.ctl) de uma relação.
   Parâmetros: <stream> é a stream do arquivo (aberto para escrita);
               <n> é o grau da relação;
               <m> é a cardinalidade da relação;
               <c> é um ponteiro para uma lista c/ os atributos da relação. */
void geraCTL(FILE *stream, int n, int m, Cabecalho *c);

/* Descrição: gera o arquivo de dados (.dad) de uma relação.
   Parâmetros: <stream> é a stream do arquivo (aberto para escrita);
               <tl> é um ponteiro para uma lista c/ as tuplas da relação. */
void geraDAD (FILE *stream, Relacao *r);



/***** Funções para exibição de relações na tela *****/


/* Descrição: exibe os nomes dos atributos de uma relação formatados
              como cabeçalho de uma tabela.
   Parâmetros: <c> é um ponteiro para uma lista c/ os atributos da relação;
               <l> é a largura das colunas da tabela
                       (todas têm a mesma largura). */
void imprimeCabecalho (Cabecalho *c, int fsize);

/* Descrição: exibe uma tupla de uma relação formatada como uma linha
              de uma tabela.
   Parâmetros: <t> é um ponteiro para uma lista c/ os valores da tupla;
               <l> é a largura das colunas da tabela
                       (todas têm a mesma largura). */
void imprimeTupla(Tupla *t, int l);

/* Descrição: imprime uma tabela formatada.
   Parâmetros: <nome> é o nome da relação a ser exibida;
               <t> é a tabela a ser exibida.

               2010.2 isCount,isMin,isMax,isAvg,isSum adicionados*/

void imprimeTabela(char *nome, Tabela t,int isCount,int isSum,int isMin, int isMax,int isAvg);


/***** Outras funções utilizadas internamente *****/


/* Descrição: verifica se uma string representa uma constante inteira.
   Parâmetros: <str> é a string a ser verificada.
   Valor de retorno: retorna 1 caso a string represente uma constante inteira.
                     Retorna 0 em caso contrário. */
int isint(char *str);

/* Descrição: verifica se uma string representa uma constante do tipo
              cadeia de caracteres (se está entre " ").
   Parâmetros: <str> é a string a ser verificada.
   Valor de retorno: retorna 1 caso a string represente uma constante
                     do tipo cadeia de caracteres. Retorna 0 em
                     caso contrário. */
int isstr(char *str);

/* Descrição: remove o 1º e o último caracteres de uma string.
   Parâmetros: <str> é a string a ser modificada.
   Observações: utilizada para remover as " " de uma constante do tipo
                cadeia de caracteres. */
void removeAspas(char *str);

/* Descrição: gera um nome formatado (se possível, que não esteja em uso)
              para um arquivo de operações da álgebra relacional (.alg).
   Parâmetros: <alg> é a variável string que armazenará o nome gerado. */
void geraNomeAlg(char *alg);

/* Descrição: identifica a tabela (dentre 2 candidatas) à qual um
              atributo pertence.
   Parâmetros: <atr> é o nome do atributo (pode estar qualificado c/ o
                     nome de uma relação;
               <tab1> é o nome da 1ª relação candidata;
               <tab2> é o nome da 2ª relação candidata;
               <al1> é um ponteiro para uma lista c/ os atributos da
                     1ª relação candidata;
               <al2> é um ponteiro para uma lista c/ os atributos da
                     2ª relação candidata.
   Valor de retorno: - Retorna 1 caso o atributo pertença à 1ª relação
                       candidata.
                     - Retorna 2 caso o atributo pertença à 2ª relação
                       candidata.
                     - Retorna 3 caso o atributo pertença a ambas as
                       relações e não esteja qualificado.
                     - Retorna 0 caso o atributo não pertença a
                       nenhuma das relações ou caso não pertença à
                       relação cujo nome o qualifica. */
int atrtab (char *atr, char *tab1, char *tab2, Cabecalho *al1, Cabecalho *al2);

/* Descrição: troca os valores de 2 strings, um pelo outro.
   Parâmetros: <str1> é a 1ª string a ser trocada;
               <str2> é a 2ª string a ser trocada.
   Observações: utilizada para trocar nomes de atributos*/
void swap(char *str1, char *str2);

/* Descrição: Conta o no. de vírgulas em uma string.
   Parâmetros: s é a string onde será feita a contagem.
   Valor de retorno: no. de vírgulas em s. */
int contaVirgulas(char* s);
