/******************************** Principal.c ********************************/

/* Arquivo principal. */

#include <stdlib.h>
#include "Entrada.h"

/* Função main: o programa deve receber como parâmetro o nome de um arquivo
				contendo comandos sql. Após verificar o formato da chamada ao
				programa, a função encaminha o nome do arquivo ao interpretador
				que lerá e executará os comandos. */


int main(int argc, char *argv[]){
    FILE *arquivo;

/*    argc = 2; /*DELETAR
    arquivo = fopen("cmd.sql", "r");*/


	if (argc != 2){
		printf("Use: BDUFF <nomedoarquivo>\n\n");
		system("pause");
		exit(1);
	}

	if ((arquivo = fopen(argv[1], "r")) == NULL){
		printf("\n Erro: Nao foi possivel abrir o arquivo!\n\n");
		system("pause");
	    exit(1);
	}
  

    if(arquivo != NULL)
        interpreta(arquivo);	/*Chamada à função de interpretação dos comandos*/
    return(0);
}
