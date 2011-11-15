/********************************* Entrada.h *********************************/

/* Leitura, verificação sintática, e interpretação dos comandos sql. */

#include <stdio.h>
#include "SQL.h"



/* Funções de manipulação de strings*/


/* Descrição: compara duas strings, sem diferenciar maiúsculas e minúsculas.
   Parâmetros: s1 e s2 são as strings a serem comparadas. 
   Valor de retorno: maior que zero se s1 > s2, menor que zero se s1 < s2 e 
   zero se s1 = s2*/
int strcasecmp(const char *s1, const char *s2 );

/* Descrição: Busca o primeiro caractere (diferente de nulo ou de espaço)
   em uma dada string.
   Parâmetros: s é a string a ser analisada. 
   Valor de retorno: se algum caractere é encontrado, retorna a posição. Caso 
   contrário, retorna -1.*/
int primeiroCh(char *s);

/* Descrição: Retira as quebras de linha de uma dada string.
   Parâmetros: s é a string a ser analisada. */
void retiraQuebras(char *s);

/* Descrição: Retira os espaços em branco excessivos (i.e., se há mais de um 
   espaço entre dois caracteres, apenas um é deixado) de uma dada string.
   Parâmetros: s é a string a ser analisada. */
void suprimeEspacos(char *s);

/* Descrição: Normaliza (suprime espaços em branco, quebras de linha e 
   tabulações de) uma string s.
   Parâmetros: s é a string a ser normalizada. */
void normaliza(char *s);



/* Funções de manipulação dos comandos e blocos*/


/* Descrição: Verifica se existem num mesmo comando duas palavras-chave (CREATE 
   TABLE, INSERT, SELECT). 
   Parâmetros: cmd é a string que contém o comando.
   Valor de retorno: retorna zero em caso de sucesso. Em caso de erro, retorna 
   1.*/
int checaDuploComando(char *cmd);

/* Descrição: Busca o próximo comando sql em um arquivo. Um comando é delimitado
   por ';'. 
   Parâmetros: buffer é a string na qual o comando será retornado. arq é o
   arquivo de comandos a serem lidos.
   Valor de retorno: retorna zero em caso de sucesso. Em caso de erro, retorna 
   um valor positivo (vide ErrMsgs.c).*/
int proxComando(char *buffer, FILE *arq);

/* Descrição: Busca o próximo delimitador em um comando sql.
   Cada parte de um bloco pode ser delimitada por '(', ')', ',' ou ';'. 
   Parâmetros: cmd é a string que contém um comando sql. tipo é uma variável que 
   retorna o tipo de delimitador encontrado.
   Valor de retorno: retorna a posição do delimitador em caso de
   sucesso. Em caso de erro, retorna -1.*/
int proxDelimitador(char *cmd, int *tipo);

/* Descrição: Busca a próxima palavra numa string. Uma palavra é delimitada por
   um espaço ou por um caractere nulo. 
   Parâmetros: s é a string que contém a palavra.
   Valor de retorno: retorna a posição do fim da palavra.*/
int proxPalavra(char *s);



/* Funções de interpretação e analise sintática dos comandos*/


/* Descrição: Lê um comando sql, o analisa sintaticamente, decodifica o seu 
   tipo e repassa o comando para uma análise 
   específica. 
   Parâmetros: c é a string que contém o comando.
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaComando(char *c);

/* Descrição: analisa um comando CREATE TABLE sintaticamente, decodifica 
   os seus parâmetros (nome da tabela, nome, tipo e características dos 
   atributos) e repassa o comando para a geração da tabela. 
   Parâmetros: cmd é a string que contém o comando(apenas a parte dos parâmetros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaCreate(char *cmd);

/* Descrição: analisa um comando INSERT sintaticamente, decodifica os 
   parâmetros e os passa para a inserção da tupla.
   Parâmetros: cmd é a string que contém o comando(apenas a parte dos parâmetros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaInsert(char *cmd);

/* Descrição: analisa um comando DELETE sintaticamente, decodifica os 
   parâmetros e os passa para a remoção da(s) tupla(s).
   Parâmetros: cmd é a string que contém o comando(apenas a parte dos parâmetros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaDelete(char *cmd);

/* Descrição: analisa um comando UPDATE sintaticamente, decodifica os 
   parâmetros e os passa para a atualização da(s) tupla(s).
   Parâmetros: cmd é a string que contém o comando(apenas a parte dos parâmetros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaUpdate(char *cmd);



/* Descrição: verifica uma condicao sintaticamente.
   Parâmetros: gmcondicao é a string que contém a selecao. c é a condicao que será retornada
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int verificarCondicao(char *gmcondicao, Condicao *c);

/* Descrição: analisa um comando SELECT sintaticamente, decodifica o nome 
   da tabela e a condição de seleção. Caso haja uma condição de join, o comando
   é repassado para decodificação dessa condição. Caso contrário, os parâmetros
   são passados para geração das operações algébricas correspondentes.
   Parâmetros: cmd é a string que contém o comando(apenas a parte dos parâmetros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaSelect(char *cmd);

/* Descrição: analisa a condição de join de um comando SELECT e passa os parâmetros
   para geração das operações algébricas correspondentes.
   Parâmetros: cmd é a string que contém o comando(apenas a parte dos parâmetros).
               listaAtrib é a string que contém a lista de atributos da projeção.
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaJoin(char *listaAtrib, char *cmd);

/* Descrição: decodifica um bloco de parâmetros de um CREATE TABLE (nome,
   tipo, chv e/ou ord) em uma variável do tipo Atributo.
   Parâmetros: a é um ponteiro para uma lista de atributos. s é a string que 
   contém o comando(apenas a parte dos parâmetros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int decAtributo(Atributo *a, char *s);

/* Descrição: Lê um arquivo de comandos sql e os interpreta.
   Parâmetros: arq é um ponteiro para o arquivo com os comandos sql.
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int interpreta(FILE *arq);
