/********************************** AlgRel.h **********************************/

/* Leitura e execução das operações da álgebra relacional. */

#include "ErrMsgs.h"


/* Funções de manipulação de arquivos */


/* Descrição: Copia uma tabela inteira (arquivos .dad e .ctl).
   Parâmetros: out é o nome da tabela de destino.
   			   in é o nome da tabela de origem.*/
void cpyarq(char *out, char *in);

/* Descrição: Desloca o ponteiro de um arquivo para uma determinada linha.
   Parâmetros: arq é um ponteiro para o arquivo. n é o índice da linha de 
   destino.*/
void desloca(FILE *arq, int n);

/* Descrição: recupera as informações sobre uma relação.
   Parâmetros: <stream> é a stream do arquivo de meta-dados da relação
                        (aberto para leitura);
               <n> é um ponteiro para uma variável inteira;
               <m> é um ponteiro para uma variável inteira.
   Valor de retorno: retorna um ponteiro para uma lista c/ os atributos
                     da relação.
   Observações: na variável apontada por <n> será armazenado o grau da relação
                e, na apontada por <m>, a cardinalidade. */
Cabecalho *decCatalogo (FILE *stream, int *n, int *m);

/* Descrição: recupera os dados de uma relação.
   Parâmetros: <stream> é a stream do arquivo de dados da relação
                        (aberto para leitura).
   Valor de retorno: retorna um ponteiro para uma lista c/ as tuplas
                     da relação. */
Relacao *decRelacao(FILE *stream);

/* Descrição: recupera os dados de uma tabela.
   Parâmetros: <ctl> é a stream do arquivo de meta-dados da relação
                        (aberto para leitura).
               <dad> é a stream do arquivo de dados da relação
                        (aberto para leitura);
   Valor de retorno: retorna uma variável do tipo Tabela. */
Tabela decTabela(FILE *ctl, FILE *dad);


/* Funções de manipulação de busca */


/* Descrição: Faz uma busca sequencial dentro de um arquivo de tuplas, em
   relação a determinado atributo.
   Parâmetros: arq é um ponteiro para o arquivo. k é o valor a ser buscado. pos
   é a ordem do atributo (posição dentro da tupla).
   Valor de retorno: retorna a posição da primeira tupla contendo k. Caso não 
   exista, retorna -1.*/
int buscaSeq(FILE *arq, char *k, int pos, int ord, int tipo);


/* Funções da álgebra relacional */


/* Descrição: Executa a operação da álgebra de seleção.
   Parâmetros: a é a tabela de origem. z é a tabela de destino. atrib é o valor 
   a ser comparado (para as tuplas selecionadas). tipo é o tipo do atributo. pos
   é a ordem do atributo da seleção (posição dentro da tupla). chv diz se o
   atributo é chave (nesse caso, basta uma busca). ord diz se a relação está
   ordenada pelo atributo. nTuplas é o
   número de tuplas na relação.*/
void selecao(char *a, char *z, char *atrib, int tipo, int pos, int chv,
                                                      int ord, int nTuplas);

/* Descrição: Executa a operação da álgebra de junção.
   Parâmetros: a e b são as tabelas de origem. z é a tabela de destino. tipo é o
   tipo dos atributos da condição. posA é a ordem do atributo da junção em a
   (posição dentro da tupla). posB é a ordem do atributo da junção em b (posição
   dentro da tupla). chv diz se o atributo é chave da tabela b (nesse caso,
   basta uma busca). ord diz se a relação b está ordenada pelo atributo.
   nTuplas é o número de tuplas na tabela b.*/
void juncao(char *a, char *b, char *z, int tipo, int posA, int posB,
                 int chv, int ord, int nTuplas, char *nomeA);

/* Descrição: Executa a operação da álgebra de projeção.
   Parâmetros: a é a tabela de origem. z é a tabela de destino. cols é um vetor
   que contém os números das colunas a serem exibidas.*/
void projecao(char *a, char *z, int cols[],int isDistinct, int isOrderBy);


/* Funções de interpretação das funções da álgebra (a partir do arquivo .alg) */


/* Descrição: Prepara a chamada da função de seleção propriamente dita. (Lê 
   informações como o número de tuplas e a ordem do atributo a ser comparado)
   Parâmetros: a é a tabela de origem. z é a tabela de destino. c é a condição
   de junção.*/
void executaSelecao(char *a, char *z, Condicao c);

/* Descrição: Prepara a chamada da função de junção propriamente dita. (Lê 
   informações como o número de tuplas e a ordem dos atributos a serem 
   comparados, além de decidir qual a melhor tabela a ser colocada como 1ª)
   Parâmetros: a e b são as tabelas de origem. z é a tabela de destino. c é a 
   condição de junção.*/
void executaJuncao(char *a, char *b, char *z, char *nomeA, Condicao c);

/* Descrição: Prepara a chamada da função de projeção propriamente dita.
   Parâmetros: a é a tabela de origem. z é a tabela de destino. listaAtrib é a
   lista de atributos.*/
void executaProjecao(char *a, char *z, char *listaAtrib,int isDistinct, int isOrderBy);

/* Descrição: Decodifica uma seleção.
   Parâmetros: s é uma string que contém a seleção. a é a tabela de origem. z é
   a tabela de destino. c é a variável que vai receber a condição de seleção.*/
void decSelecao(char *s, char *a, char *z, Condicao *c);

/* Descrição: Decodifica uma junção.
   Parâmetros: s é uma string que contém a junção. a e b são as tabelas de 
   origem. z é a tabela de destino. c é a variável que vai receber a condição de
   junção.*/
void decJuncao(char *s, char *a, char *b, char *z, Condicao *c);

/* Descrição: Decodifica uma projeção.
   Parâmetros: s é uma string que contém a projeção. a é a tabela de origem. z é
   a tabela de destino. listaAtrib é a lista de atributos.*/
void decProjecao(char *s, char *a, char *z, char *listaAtrib);


/* Funções que manipulam atributos dentro de uma tupla */


/* Descrição: Recupera o valor de um atributo dentro de uma tupla.
   Parâmetros: s é uma string que contém uma tupla. pos é a ordem do atributo. 
   valor é a variável que vai receber o valor do atributo.*/
void recuperaValor(char *s, int pos, char *valor);

/* Descrição: junta duas tuplas em uma só.
   Parâmetros: t1 e t2 são strings que contêm tuplas. pos é a ordem do 
   atributo.
   Valor de retorno: ponteiro para a tupla resultante (será o mesmo valor de
   t2).*/
char *juntaTuplas(char *t1, char *t2, int pos);


/* Funções de manipulação do arquivo de entrada */


/* Descrição: Decodifica e executa uma linha do arquivo com as operações 
   algébricas.
   Parâmetros: s é uma string que contém uma operação algébrica.*/
void executaOperacao(char *s, char *nomeA,int isDistinct, int isOrderBy);

/* Descrição: Executa as operações algébricas definidas em um arquivo.
   Parâmetros: alg é um arquivo contendo operações algébricas*/
void executaAlg(FILE *alg,int isDistinct, int isOrderBy);
