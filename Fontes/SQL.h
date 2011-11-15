/********************************** sqlopr.h **********************************/

/* Implementa��o das opera��es SQL. */

#include <stdio.h>
#include "AlgRel.h"


/***** Fun��es que executam os comandos SQL *****/


/* Descri��o: cria os arquivos de dados e meta-dados para uma rela��o.
   Par�metros: <nome> � o nome da tabela a ser criada;
               <c> � um ponteiro para uma lista c/ os atributos
                    definidos p/ a tabela.
   Valor de retorno: retorna 0 caso n�o ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int createTable (char *nome, Cabecalho *c);

/* Descri��o: executa o comando "INSERT".
   Par�metros: <tab> � o nome da tabela onde os valores devem ser inseridos;
               <vl> � um ponteiro para uma lista c/ os valores a serem
                    inseridos (tupla).
   Valor de retorno: retorna 0 caso n�o ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int insert (char *tab, Tupla *vl);

/* Descri��o: executa o comando "DELETE" com condi��o.
   Par�metros: <tab> � o nome da tabela;
               <atrib> � o atributo da condi��o.
               <valor> � o valor da condi��o.
   Valor de retorno: retorna 0 caso n�o ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int deleteCond(char *tab, char *atrib, char *valor);

/* Descri��o: executa o comando "DELETE" sem condi��o.
   Par�metros: <tab> � o nome da tabela;
   Valor de retorno: retorna 0 caso n�o ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int deleteAll(char *tab);

/* Descri��o: executa o comando "UPDATE" com condi��o.
   Par�metros: <tab> � o nome da tabela;
               <atribSet> � o atributo a ser atualizado.
               <valorSet> � o valor a ser inserido.
               <atribWhere> � o atributo da condi��o.
               <valorWhere> � o valor da condi��o.
   Valor de retorno: retorna 0 caso n�o ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int updateCond(char *tab, char *atribSet, char *valorSet, char *atribWhere, char *valorWhere);

/* Descri��o: executa o comando "UPDATE" sem condi��o.
   Par�metros: <tab> � o nome da tabela;
               <atrib> � o atributo a ser atualizado.
               <valor> � o valor a ser inserido.
   Valor de retorno: retorna 0 caso n�o ocorra nenhum erro.
                     Retorna 1 em caso de erro. */
int updateAll(char *tab, char *atrib, char *valor);

/* Descri��o: cria um arquivo c/ a opera��o alg�brica necess�ria para
              execu��o de um comando "SELECT" (sem "JOIN"), chama a
              fun��o que interpreta e executa tal opera��o, e exibe a
              rela��o resultante na tela.
   Par�metros: <tab> � o nome da tabela onde ser� feita a sele��o;
               <cond> � um ponteiro para a condi��o de sele��o.
               <listaAtrib> � a string que cont�m a lista de atributos da proje��o.
   Valor de retorno: retorna 0 caso n�o ocorra nenhum erro.
                     Retorna 1 em caso de erro.
   Observa��es: se n�o houver condi��o de sele��o,
                o valor de <cond> deve ser NULL.


                Atualiza��o 2010.2
                isCount,isSum,isMin,isMax,isAvg*/

int selectSQL(char *tab, char *listaAtrib, Condicao *cond,int isDistinct, int isOrderBy,int isCount,int isSum,int isMin, int isMax,int isAvg);

/* Descri��o: cria um arquivo c/ a seq��ncia de opera��es alg�bricas
              necess�rias para execu��o de um comando "SELECT" (com "JOIN"),
              chama a fun��o que interpreta e executa tais opera��es,
              e exibe a rela��o resultante na tela.
   Par�metros: <tab1> � o nome da 1� tabela participante da jun��o;
               <tab2> � o nome da 2� tabela participante da jun��o;
               <listaAtrib> � a string que cont�m a lista de atributos da proje��o.
               <jcond> � a condi��o de jun��o;
               <scond> � um ponteiro para a condi��o de sele��o.
   Valor de retorno: retorna 0 caso n�o ocorra nenhum erro.
                     Retorna 1 em caso de erro.
   Observa��es: se n�o houver condi��o de sele��o,
                o valor de <scond> deve ser NULL. */
int join (char *tab1, char *tab2, char *listaAtrib, Condicao jcond, Condicao *scond);


/***** Fun��es de verifica��o de erros *****/


/* Descri��o: verifica se h� erros em uma lista de atributos.
   Par�metros: <c> � um ponteiro para a lista de atributos a ser verificada.
   Valor de retorno: retorna 0 caso n�o haja erros. Retorna um valor
                     positivo em caso de erro (vide errmsgs.c). */
int validaCabecalho (Cabecalho *c);

/* Descri��o: verifica se h� erros em uma lista de valores (tupla).
   Par�metros: <tup> � um ponteiro para a lista de valores a ser verificada;
               <tab> � a tabela para refer�ncia.
   Valor de retorno: retorna 0 caso n�o haja erros. Retorna um valor
                     positivo em caso de erro (vide errmsgs.c). */
int validaTupla (Tupla *tup, Tabela tab);

/* Descri��o: verifica se h� erros em uma condi��o de sele��o.
   Par�metros: <cond> � a condi��o a ser verificada;
               <c> � um ponteiro para uma lista c/ os atributos da
                    rela��o � qual se refere <cond>.
   Valor de retorno: retorna 0 caso n�o haja erros. Retorna um valor
                     positivo em caso de erro (vide errmsgs.c). */
int validaCondSel (Condicao cond, Cabecalho *c);

/* Descri��o: verifica se h� erros em uma condi��o de jun��o.
   Par�metros: <cond> � a condi��o a ser verificada;
               <c1> � um ponteiro para uma lista c/ os atributos da
                     1� rela��o � qual se refere <cond>;
               <c2> � um ponteiro para uma lista c/ os atributos da
                     2� rela��o � qual se refere <cond>.
   Valor de retorno: retorna 0 caso n�o haja erros. Retorna um valor
                     positivo em caso de erro (vide errmsgs.c). */
int validaCondJun (Condicao cond, Cabecalho *c1, Cabecalho *c2);

/* Descri��o: verifica se h� erros em uma lista de atributos.
   Par�metros: <listaAtrib> � a lista a ser verificada;
               <tab1> � o nome da 1� rela��o � qual se refere <listaAtrib>;
               <tab2> � o nome da 2� rela��o (se houver) � qual se refere <listaAtrib>.
               <c1> � um ponteiro para uma lista c/ os atributos da
                     1� rela��o � qual se refere <listaAtrib>;
               <c2> � um ponteiro para uma lista c/ os atributos da
                     2� rela��o (se houver) � qual se refere <listaAtrib>.
   Valor de retorno: retorna 0 caso n�o haja erros. Retorna um valor
                     positivo em caso de erro (vide errmsgs.c). */
int validaListaAtrib(char **listaAtrib, char *tab1, char *tab2, Cabecalho *c1, Cabecalho *c2);


/***** Fun��es de manipula��o de arquivos *****/


/* Descri��o: gera o arquivo de meta-dados (.ctl) de uma rela��o.
   Par�metros: <stream> � a stream do arquivo (aberto para escrita);
               <n> � o grau da rela��o;
               <m> � a cardinalidade da rela��o;
               <c> � um ponteiro para uma lista c/ os atributos da rela��o. */
void geraCTL(FILE *stream, int n, int m, Cabecalho *c);

/* Descri��o: gera o arquivo de dados (.dad) de uma rela��o.
   Par�metros: <stream> � a stream do arquivo (aberto para escrita);
               <tl> � um ponteiro para uma lista c/ as tuplas da rela��o. */
void geraDAD (FILE *stream, Relacao *r);



/***** Fun��es para exibi��o de rela��es na tela *****/


/* Descri��o: exibe os nomes dos atributos de uma rela��o formatados
              como cabe�alho de uma tabela.
   Par�metros: <c> � um ponteiro para uma lista c/ os atributos da rela��o;
               <l> � a largura das colunas da tabela
                       (todas t�m a mesma largura). */
void imprimeCabecalho (Cabecalho *c, int fsize);

/* Descri��o: exibe uma tupla de uma rela��o formatada como uma linha
              de uma tabela.
   Par�metros: <t> � um ponteiro para uma lista c/ os valores da tupla;
               <l> � a largura das colunas da tabela
                       (todas t�m a mesma largura). */
void imprimeTupla(Tupla *t, int l);

/* Descri��o: imprime uma tabela formatada.
   Par�metros: <nome> � o nome da rela��o a ser exibida;
               <t> � a tabela a ser exibida.

               2010.2 isCount,isMin,isMax,isAvg,isSum adicionados*/

void imprimeTabela(char *nome, Tabela t,int isCount,int isSum,int isMin, int isMax,int isAvg);


/***** Outras fun��es utilizadas internamente *****/


/* Descri��o: verifica se uma string representa uma constante inteira.
   Par�metros: <str> � a string a ser verificada.
   Valor de retorno: retorna 1 caso a string represente uma constante inteira.
                     Retorna 0 em caso contr�rio. */
int isint(char *str);

/* Descri��o: verifica se uma string representa uma constante do tipo
              cadeia de caracteres (se est� entre " ").
   Par�metros: <str> � a string a ser verificada.
   Valor de retorno: retorna 1 caso a string represente uma constante
                     do tipo cadeia de caracteres. Retorna 0 em
                     caso contr�rio. */
int isstr(char *str);

/* Descri��o: remove o 1� e o �ltimo caracteres de uma string.
   Par�metros: <str> � a string a ser modificada.
   Observa��es: utilizada para remover as " " de uma constante do tipo
                cadeia de caracteres. */
void removeAspas(char *str);

/* Descri��o: gera um nome formatado (se poss�vel, que n�o esteja em uso)
              para um arquivo de opera��es da �lgebra relacional (.alg).
   Par�metros: <alg> � a vari�vel string que armazenar� o nome gerado. */
void geraNomeAlg(char *alg);

/* Descri��o: identifica a tabela (dentre 2 candidatas) � qual um
              atributo pertence.
   Par�metros: <atr> � o nome do atributo (pode estar qualificado c/ o
                     nome de uma rela��o;
               <tab1> � o nome da 1� rela��o candidata;
               <tab2> � o nome da 2� rela��o candidata;
               <al1> � um ponteiro para uma lista c/ os atributos da
                     1� rela��o candidata;
               <al2> � um ponteiro para uma lista c/ os atributos da
                     2� rela��o candidata.
   Valor de retorno: - Retorna 1 caso o atributo perten�a � 1� rela��o
                       candidata.
                     - Retorna 2 caso o atributo perten�a � 2� rela��o
                       candidata.
                     - Retorna 3 caso o atributo perten�a a ambas as
                       rela��es e n�o esteja qualificado.
                     - Retorna 0 caso o atributo n�o perten�a a
                       nenhuma das rela��es ou caso n�o perten�a �
                       rela��o cujo nome o qualifica. */
int atrtab (char *atr, char *tab1, char *tab2, Cabecalho *al1, Cabecalho *al2);

/* Descri��o: troca os valores de 2 strings, um pelo outro.
   Par�metros: <str1> � a 1� string a ser trocada;
               <str2> � a 2� string a ser trocada.
   Observa��es: utilizada para trocar nomes de atributos*/
void swap(char *str1, char *str2);

/* Descri��o: Conta o no. de v�rgulas em uma string.
   Par�metros: s � a string onde ser� feita a contagem.
   Valor de retorno: no. de v�rgulas em s. */
int contaVirgulas(char* s);
