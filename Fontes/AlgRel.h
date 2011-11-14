/********************************** AlgRel.h **********************************/

/* Leitura e execu��o das opera��es da �lgebra relacional. */

#include "ErrMsgs.h"


/* Fun��es de manipula��o de arquivos */


/* Descri��o: Copia uma tabela inteira (arquivos .dad e .ctl).
   Par�metros: out � o nome da tabela de destino.
   			   in � o nome da tabela de origem.*/
void cpyarq(char *out, char *in);

/* Descri��o: Desloca o ponteiro de um arquivo para uma determinada linha.
   Par�metros: arq � um ponteiro para o arquivo. n � o �ndice da linha de 
   destino.*/
void desloca(FILE *arq, int n);

/* Descri��o: recupera as informa��es sobre uma rela��o.
   Par�metros: <stream> � a stream do arquivo de meta-dados da rela��o
                        (aberto para leitura);
               <n> � um ponteiro para uma vari�vel inteira;
               <m> � um ponteiro para uma vari�vel inteira.
   Valor de retorno: retorna um ponteiro para uma lista c/ os atributos
                     da rela��o.
   Observa��es: na vari�vel apontada por <n> ser� armazenado o grau da rela��o
                e, na apontada por <m>, a cardinalidade. */
Cabecalho *decCatalogo (FILE *stream, int *n, int *m);

/* Descri��o: recupera os dados de uma rela��o.
   Par�metros: <stream> � a stream do arquivo de dados da rela��o
                        (aberto para leitura).
   Valor de retorno: retorna um ponteiro para uma lista c/ as tuplas
                     da rela��o. */
Relacao *decRelacao(FILE *stream);

/* Descri��o: recupera os dados de uma tabela.
   Par�metros: <ctl> � a stream do arquivo de meta-dados da rela��o
                        (aberto para leitura).
               <dad> � a stream do arquivo de dados da rela��o
                        (aberto para leitura);
   Valor de retorno: retorna uma vari�vel do tipo Tabela. */
Tabela decTabela(FILE *ctl, FILE *dad);


/* Fun��es de manipula��o de busca */


/* Descri��o: Faz uma busca sequencial dentro de um arquivo de tuplas, em
   rela��o a determinado atributo.
   Par�metros: arq � um ponteiro para o arquivo. k � o valor a ser buscado. pos
   � a ordem do atributo (posi��o dentro da tupla).
   Valor de retorno: retorna a posi��o da primeira tupla contendo k. Caso n�o 
   exista, retorna -1.*/
int buscaSeq(FILE *arq, char *k, int pos, int ord, int tipo);


/* Fun��es da �lgebra relacional */


/* Descri��o: Executa a opera��o da �lgebra de sele��o.
   Par�metros: a � a tabela de origem. z � a tabela de destino. atrib � o valor 
   a ser comparado (para as tuplas selecionadas). tipo � o tipo do atributo. pos
   � a ordem do atributo da sele��o (posi��o dentro da tupla). chv diz se o
   atributo � chave (nesse caso, basta uma busca). ord diz se a rela��o est�
   ordenada pelo atributo. nTuplas � o
   n�mero de tuplas na rela��o.*/
void selecao(char *a, char *z, char *atrib, int tipo, int pos, int chv,
                                                      int ord, int nTuplas);

/* Descri��o: Executa a opera��o da �lgebra de jun��o.
   Par�metros: a e b s�o as tabelas de origem. z � a tabela de destino. tipo � o
   tipo dos atributos da condi��o. posA � a ordem do atributo da jun��o em a
   (posi��o dentro da tupla). posB � a ordem do atributo da jun��o em b (posi��o
   dentro da tupla). chv diz se o atributo � chave da tabela b (nesse caso,
   basta uma busca). ord diz se a rela��o b est� ordenada pelo atributo.
   nTuplas � o n�mero de tuplas na tabela b.*/
void juncao(char *a, char *b, char *z, int tipo, int posA, int posB,
                 int chv, int ord, int nTuplas, char *nomeA);

/* Descri��o: Executa a opera��o da �lgebra de proje��o.
   Par�metros: a � a tabela de origem. z � a tabela de destino. cols � um vetor
   que cont�m os n�meros das colunas a serem exibidas.*/
void projecao(char *a, char *z, int cols[],int isDistinct, int isOrderBy);


/* Fun��es de interpreta��o das fun��es da �lgebra (a partir do arquivo .alg) */


/* Descri��o: Prepara a chamada da fun��o de sele��o propriamente dita. (L� 
   informa��es como o n�mero de tuplas e a ordem do atributo a ser comparado)
   Par�metros: a � a tabela de origem. z � a tabela de destino. c � a condi��o
   de jun��o.*/
void executaSelecao(char *a, char *z, Condicao c);

/* Descri��o: Prepara a chamada da fun��o de jun��o propriamente dita. (L� 
   informa��es como o n�mero de tuplas e a ordem dos atributos a serem 
   comparados, al�m de decidir qual a melhor tabela a ser colocada como 1�)
   Par�metros: a e b s�o as tabelas de origem. z � a tabela de destino. c � a 
   condi��o de jun��o.*/
void executaJuncao(char *a, char *b, char *z, char *nomeA, Condicao c);

/* Descri��o: Prepara a chamada da fun��o de proje��o propriamente dita.
   Par�metros: a � a tabela de origem. z � a tabela de destino. listaAtrib � a
   lista de atributos.*/
void executaProjecao(char *a, char *z, char *listaAtrib,int isDistinct, int isOrderBy);

/* Descri��o: Decodifica uma sele��o.
   Par�metros: s � uma string que cont�m a sele��o. a � a tabela de origem. z �
   a tabela de destino. c � a vari�vel que vai receber a condi��o de sele��o.*/
void decSelecao(char *s, char *a, char *z, Condicao *c);

/* Descri��o: Decodifica uma jun��o.
   Par�metros: s � uma string que cont�m a jun��o. a e b s�o as tabelas de 
   origem. z � a tabela de destino. c � a vari�vel que vai receber a condi��o de
   jun��o.*/
void decJuncao(char *s, char *a, char *b, char *z, Condicao *c);

/* Descri��o: Decodifica uma proje��o.
   Par�metros: s � uma string que cont�m a proje��o. a � a tabela de origem. z �
   a tabela de destino. listaAtrib � a lista de atributos.*/
void decProjecao(char *s, char *a, char *z, char *listaAtrib);


/* Fun��es que manipulam atributos dentro de uma tupla */


/* Descri��o: Recupera o valor de um atributo dentro de uma tupla.
   Par�metros: s � uma string que cont�m uma tupla. pos � a ordem do atributo. 
   valor � a vari�vel que vai receber o valor do atributo.*/
void recuperaValor(char *s, int pos, char *valor);

/* Descri��o: junta duas tuplas em uma s�.
   Par�metros: t1 e t2 s�o strings que cont�m tuplas. pos � a ordem do 
   atributo.
   Valor de retorno: ponteiro para a tupla resultante (ser� o mesmo valor de
   t2).*/
char *juntaTuplas(char *t1, char *t2, int pos);


/* Fun��es de manipula��o do arquivo de entrada */


/* Descri��o: Decodifica e executa uma linha do arquivo com as opera��es 
   alg�bricas.
   Par�metros: s � uma string que cont�m uma opera��o alg�brica.*/
void executaOperacao(char *s, char *nomeA,int isDistinct, int isOrderBy);

/* Descri��o: Executa as opera��es alg�bricas definidas em um arquivo.
   Par�metros: alg � um arquivo contendo opera��es alg�bricas*/
void executaAlg(FILE *alg,int isDistinct, int isOrderBy);
