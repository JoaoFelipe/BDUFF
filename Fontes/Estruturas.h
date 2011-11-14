/********************************* Estruturas.h *********************************/

/* Estruturas de dados e suas operações. */

#define INTEGER 0
#define STRING 1
#define NAO 0
#define SIM 1
#define CASCADE 1
#define RESTRICT 0


/***** Estruturas de dados *****/


/* Condicao representa uma condição (de seleção ou junção)
   e tem 3 campos, mas só 2 são usados de cada vez:
            - se atr2 = "", condição é 'atr1 = val' (condição de seleção).
            - se valor = "", condição é 'atr1 = atr2' (condição de junção). */
typedef struct Condicao	
		{
			char atr1[20];	/*Nome do primeiro atributo*/
			char atr2[20];	/*Nome do segundo atributo*/
			char val[30];	/*Constante a ser comparada*/
		}Condicao;

typedef struct Referenciada
        {
            char nome[20];
            struct Referenciada *next;        
            struct Referenciada *prev;
        }Referenciada;

/* Atributo é definido por nome, tipo (0 => inteiro, 1 => string),
   Key (0 => não é chave, 1 => é) e Ord (0 => não ordenar, 1 => ordenar) */
typedef struct Atributo
		{
			char nome[20];	        /*Nome do atributo*/
			unsigned int tipo :1 ;	/*Tipo do atributo*/
			unsigned int key :1 ;	/*Indica se o atributo é chave*/
			unsigned int fkey :1 ;	/*Indica se o atributo é chave estrangeira*/
			char f_referencia[20];   /*Indica a tabela referenciada pela chave estrangeira*/
            Referenciada *f_referenciada;   /*Indica as tabelas que referenciam essa tabela*/
            unsigned int tipo_ref :1 ;	/*Indica se é cascade ou restrict. Default restrict*/
            unsigned int conta_ref ;	/*Indica quantas referencias existem a essa tabela*/
			unsigned int ord :1 ;	/*Indica se a relação é ordenada*/
		}Atributo;                  /*pelo atributo*/
		
/* Cabeçalho de uma tabela (lista duplamente encadeada circular de atributos). */
typedef struct Cabecalho
        {
            Atributo atributo;             /*Atributo*/
            struct Cabecalho *next;     /*Próximo*/
            struct Cabecalho *prev;     /*Anterior*/
        }Cabecalho;

/* Lista duplamente encadeada circular de valores. */
typedef struct Tupla
        {
            char val[100];            /*Valor*/
            struct Tupla *next;     /*Próximo*/
            struct Tupla *prev;     /*Anterior*/
        }Tupla;
        
/* Lista duplamente encadeada circular de tuplas
   (cada tupla é uma lista de valores). */
typedef struct Relacao
        {
            Tupla *tupla;             /*Tupla*/
            struct Relacao *next;     /*Próximo*/
            struct Relacao *prev;     /*Anterior*/
        }Relacao;
        
/* Tabela. */
typedef struct Tabela
        {
            Cabecalho *cabecalho;
            Relacao *relacao;
            int n;             /*Grau*/
            int m;             /*Cardinalidade*/
            int chv;             /*No. da coluna do aributo-chave
                                   0, se não houver.*/
            int ord;             /*No. da coluna por qual a relação é ordenada
                                   0, se não houver.*/
            int fchv;             /*No. da coluna do atributo chave estrangeira
                                   0, se não houver.*/
        }Tabela;


/***** Funções para manipulação das listas de atributos (cabeçalhos)*****/


/* Descrição: inicializa um Cabecalho.
   Parâmetros: <c> é o endereço de um ponteiro para um Cabecalho. */
void initCabecalho (Cabecalho **c);

/* Descrição: insere um atributo em um cabeçalho.
   Parâmetros: <c> é o endereço de um ponteiro para um Cabecalho;
               <a> é o atributo a ser inserido. */
void insertCabecalho (Cabecalho **c, Atributo a);

/* Descrição: retorna o número de atributos em uma lista.
   Parâmetros: <c> é um ponteiro para uma lista de atributos. */
int numAtr (Cabecalho *c);

/* Descrição: armazena na variável apontada por <atr> o n-ésimo
              atributo de uma lista.
   Parâmetros: <c> é um ponteiro para uma lista de atributos;
               <n> é a posição do atributo a ser recuperado;
               <atr> é um ponteiro para uma variável do tipo Atributo. */
void natr (Cabecalho *c, int n, Atributo *atr);

/* Descrição: recupera a posição do 1º atributo chave em uma lista.
   Parâmetros: <c> é um ponteiro para uma lista de atributos.
   Valor de retorno: caso encontre um atributo chave, retorna sua posição.
                     Caso não encontre, retorna 0.
   Observações: em uma lista de atributos deve haver apenas 1 atributo chave. */
int poschv (Cabecalho *c);

/* Descrição: recupera a posição do 1º atributo de ordenação em uma lista.
   Parâmetros: <c> é um ponteiro para uma lista de atributos.
   Valor de retorno: caso encontre um atributo de ordenação, retorna
                     sua posição. Caso não encontre, retorna 0.
   Observações: em uma lista de atributos deve haver no máximo 1
                atributo de ordenação. */  
int posord (Cabecalho *c);

/* Descrição: recupera a posição de um atributo em uma lista.
   Parâmetros: <atr> é o nome do atributo cuja posição se quer encontrar.
               <c> é um ponteiro para uma lista de atributos.
   Valor de retorno: caso encontre o atributo de nome <atr>, retorna
                     sua posição. Caso não encontre, retorna 0. */  
                     
int posfkey (Cabecalho *c);

int posatr (char *atr, Cabecalho *c);

/* Descrição: libera a memória de uma lista.
   Parâmetros: <c> é o endereço de um ponteiro para uma lista de atributos. */
void fmemCabecalho (Cabecalho **c);


/***** Funções para manipulação das listas de valores (tuplas)*****/


/* Descrição: inicializa uma lista.
   Parâmetros: <t> é o endereço de um ponteiro para uma lista de valores. */
void initTupla (Tupla **t);

/* Descrição: insere um valor em uma lista.
   Parâmetros: <t> é o endereço de um ponteiro para uma lista de valores;
               <val> é o valor a ser inserido na lista. */
void insertTupla (Tupla **t, char *val);

/* Descrição: retorna o número de valores em uma lista.
   Parâmetros: <t> é um ponteiro para uma lista de valores. */
int numVal (Tupla *t);

/* Descrição: armazena na variável apontada por <val> o n-ésimo
              valor de uma lista.
   Parâmetros: <t> é um ponteiro para uma lista de valores;
               <n> é a posição do valor a ser recuperado;
               <val> é uma variável do tipo char*. */
void nval (Tupla *t, int n, char *val);

/* Descrição: libera a memória de uma lista.
   Parâmetros: <t> é o endereço de um ponteiro para uma lista de valores. */
void fmemTupla (Tupla **t);


/***** Funções para manipulação das listas de tuplas (relações)*****/


/* Descrição: inicializa uma lista.
   Parâmetros: <r> é o endereço de um ponteiro para uma lista de tuplas. */
void initRelacao (Relacao **r);

/* Descrição: insere uma tupla em uma lista.
   Parâmetros: <r> é o endereço de um ponteiro para uma lista de tuplas;
               <tup> é a tupla a ser inserida na lista. */
void insertRelacao (Relacao **r, Tupla *tup);

/* Descrição: insere uma tupla em uma lista ordenada crescentemente
              pelo n-ésimo atributo (inteiro).
   Parâmetros: <r> é o endereço de um ponteiro para uma lista de tuplas;
               <tup> é a tupla a ser inserida na lista;
               <n> é a posição do atributo pelo qual a lista é ordenada. */
void iordinsertRelacao (Relacao **r, Tupla *tup, int n);

/* Descrição: insere uma tupla em uma lista ordenada crescentemente
              pelo n-ésimo atributo (string).
   Parâmetros: <r> é o endereço de um ponteiro para uma lista de tuplas;
               <tup> é a tupla a ser inserida na lista;
               <n> é a posição do atributo pelo qual a lista é ordenada. */
void sordinsertRelacao (Relacao **r, Tupla *tup, int n);

/* Descrição: libera a memória de uma lista.
   Parâmetros: <r> é o endereço de um ponteiro para uma lista de tuplas. */
void fmemRelacao (Relacao **r);
void fmemAtributo (Atributo *att);
