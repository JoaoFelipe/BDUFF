/********************************* Entrada.h *********************************/

/* Leitura, verifica��o sint�tica, e interpreta��o dos comandos sql. */

#include <stdio.h>
#include "SQL.h"



/* Fun��es de manipula��o de strings*/


/* Descri��o: compara duas strings, sem diferenciar mai�sculas e min�sculas.
   Par�metros: s1 e s2 s�o as strings a serem comparadas. 
   Valor de retorno: maior que zero se s1 > s2, menor que zero se s1 < s2 e 
   zero se s1 = s2*/
int strcasecmp(const char *s1, const char *s2 );

/* Descri��o: Busca o primeiro caractere (diferente de nulo ou de espa�o)
   em uma dada string.
   Par�metros: s � a string a ser analisada. 
   Valor de retorno: se algum caractere � encontrado, retorna a posi��o. Caso 
   contr�rio, retorna -1.*/
int primeiroCh(char *s);

/* Descri��o: Retira as quebras de linha de uma dada string.
   Par�metros: s � a string a ser analisada. */
void retiraQuebras(char *s);

/* Descri��o: Retira os espa�os em branco excessivos (i.e., se h� mais de um 
   espa�o entre dois caracteres, apenas um � deixado) de uma dada string.
   Par�metros: s � a string a ser analisada. */
void suprimeEspacos(char *s);

/* Descri��o: Normaliza (suprime espa�os em branco, quebras de linha e 
   tabula��es de) uma string s.
   Par�metros: s � a string a ser normalizada. */
void normaliza(char *s);



/* Fun��es de manipula��o dos comandos e blocos*/


/* Descri��o: Verifica se existem num mesmo comando duas palavras-chave (CREATE 
   TABLE, INSERT, SELECT). 
   Par�metros: cmd � a string que cont�m o comando.
   Valor de retorno: retorna zero em caso de sucesso. Em caso de erro, retorna 
   1.*/
int checaDuploComando(char *cmd);

/* Descri��o: Busca o pr�ximo comando sql em um arquivo. Um comando � delimitado
   por ';'. 
   Par�metros: buffer � a string na qual o comando ser� retornado. arq � o
   arquivo de comandos a serem lidos.
   Valor de retorno: retorna zero em caso de sucesso. Em caso de erro, retorna 
   um valor positivo (vide ErrMsgs.c).*/
int proxComando(char *buffer, FILE *arq);

/* Descri��o: Busca o pr�ximo delimitador em um comando sql.
   Cada parte de um bloco pode ser delimitada por '(', ')', ',' ou ';'. 
   Par�metros: cmd � a string que cont�m um comando sql. tipo � uma vari�vel que 
   retorna o tipo de delimitador encontrado.
   Valor de retorno: retorna a posi��o do delimitador em caso de
   sucesso. Em caso de erro, retorna -1.*/
int proxDelimitador(char *cmd, int *tipo);

/* Descri��o: Busca a pr�xima palavra numa string. Uma palavra � delimitada por
   um espa�o ou por um caractere nulo. 
   Par�metros: s � a string que cont�m a palavra.
   Valor de retorno: retorna a posi��o do fim da palavra.*/
int proxPalavra(char *s);



/* Fun��es de interpreta��o e analise sint�tica dos comandos*/


/* Descri��o: L� um comando sql, o analisa sintaticamente, decodifica o seu 
   tipo e repassa o comando para uma an�lise 
   espec�fica. 
   Par�metros: c � a string que cont�m o comando.
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaComando(char *c);

/* Descri��o: analisa um comando CREATE TABLE sintaticamente, decodifica 
   os seus par�metros (nome da tabela, nome, tipo e caracter�sticas dos 
   atributos) e repassa o comando para a gera��o da tabela. 
   Par�metros: cmd � a string que cont�m o comando(apenas a parte dos par�metros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaCreate(char *cmd);

/* Descri��o: analisa um comando INSERT sintaticamente, decodifica os 
   par�metros e os passa para a inser��o da tupla.
   Par�metros: cmd � a string que cont�m o comando(apenas a parte dos par�metros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaInsert(char *cmd);

/* Descri��o: analisa um comando DELETE sintaticamente, decodifica os 
   par�metros e os passa para a remo��o da(s) tupla(s).
   Par�metros: cmd � a string que cont�m o comando(apenas a parte dos par�metros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaDelete(char *cmd);

/* Descri��o: analisa um comando UPDATE sintaticamente, decodifica os 
   par�metros e os passa para a atualiza��o da(s) tupla(s).
   Par�metros: cmd � a string que cont�m o comando(apenas a parte dos par�metros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaUpdate(char *cmd);



/* Descri��o: verifica uma condicao sintaticamente.
   Par�metros: gmcondicao � a string que cont�m a selecao. c � a condicao que ser� retornada
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int verificarCondicao(char *gmcondicao, Condicao *c);

/* Descri��o: analisa um comando SELECT sintaticamente, decodifica o nome 
   da tabela e a condi��o de sele��o. Caso haja uma condi��o de join, o comando
   � repassado para decodifica��o dessa condi��o. Caso contr�rio, os par�metros
   s�o passados para gera��o das opera��es alg�bricas correspondentes.
   Par�metros: cmd � a string que cont�m o comando(apenas a parte dos par�metros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaSelect(char *cmd);

/* Descri��o: analisa a condi��o de join de um comando SELECT e passa os par�metros
   para gera��o das opera��es alg�bricas correspondentes.
   Par�metros: cmd � a string que cont�m o comando(apenas a parte dos par�metros).
               listaAtrib � a string que cont�m a lista de atributos da proje��o.
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int analisaJoin(char *listaAtrib, char *cmd);

/* Descri��o: decodifica um bloco de par�metros de um CREATE TABLE (nome,
   tipo, chv e/ou ord) em uma vari�vel do tipo Atributo.
   Par�metros: a � um ponteiro para uma lista de atributos. s � a string que 
   cont�m o comando(apenas a parte dos par�metros).
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int decAtributo(Atributo *a, char *s);

/* Descri��o: L� um arquivo de comandos sql e os interpreta.
   Par�metros: arq � um ponteiro para o arquivo com os comandos sql.
   Valor de retorno: retorna zero em caso de sucesso. Retorna um valor positivo
   em caso de erro (vide ErrMsgs.c).*/
int interpreta(FILE *arq);
