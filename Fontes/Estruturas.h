/********************************* Estruturas.h *********************************/

/* Estruturas de dados e suas opera��es. */

#define INTEGER 0
#define STRING 1
#define NAO 0
#define SIM 1
#define CASCADE 1
#define RESTRICT 0


/***** Estruturas de dados *****/


/* Condicao representa uma condi��o (de sele��o ou jun��o)
   e tem 3 campos, mas s� 2 s�o usados de cada vez:
            - se atr2 = "", condi��o � 'atr1 = val' (condi��o de sele��o).
            - se valor = "", condi��o � 'atr1 = atr2' (condi��o de jun��o). */
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

/* Atributo � definido por nome, tipo (0 => inteiro, 1 => string),
   Key (0 => n�o � chave, 1 => �) e Ord (0 => n�o ordenar, 1 => ordenar) */
typedef struct Atributo
		{
			char nome[20];	        /*Nome do atributo*/
			unsigned int tipo :1 ;	/*Tipo do atributo*/
			unsigned int key :1 ;	/*Indica se o atributo � chave*/
			unsigned int fkey :1 ;	/*Indica se o atributo � chave estrangeira*/
			char f_referencia[20];   /*Indica a tabela referenciada pela chave estrangeira*/
            Referenciada *f_referenciada;   /*Indica as tabelas que referenciam essa tabela*/
            unsigned int tipo_ref :1 ;	/*Indica se � cascade ou restrict. Default restrict*/
            unsigned int conta_ref ;	/*Indica quantas referencias existem a essa tabela*/
			unsigned int ord :1 ;	/*Indica se a rela��o � ordenada*/
		}Atributo;                  /*pelo atributo*/
		
/* Cabe�alho de uma tabela (lista duplamente encadeada circular de atributos). */
typedef struct Cabecalho
        {
            Atributo atributo;             /*Atributo*/
            struct Cabecalho *next;     /*Pr�ximo*/
            struct Cabecalho *prev;     /*Anterior*/
        }Cabecalho;

/* Lista duplamente encadeada circular de valores. */
typedef struct Tupla
        {
            char val[100];            /*Valor*/
            struct Tupla *next;     /*Pr�ximo*/
            struct Tupla *prev;     /*Anterior*/
        }Tupla;
        
/* Lista duplamente encadeada circular de tuplas
   (cada tupla � uma lista de valores). */
typedef struct Relacao
        {
            Tupla *tupla;             /*Tupla*/
            struct Relacao *next;     /*Pr�ximo*/
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
                                   0, se n�o houver.*/
            int ord;             /*No. da coluna por qual a rela��o � ordenada
                                   0, se n�o houver.*/
            int fchv;             /*No. da coluna do atributo chave estrangeira
                                   0, se n�o houver.*/
        }Tabela;


/***** Fun��es para manipula��o das listas de atributos (cabe�alhos)*****/


/* Descri��o: inicializa um Cabecalho.
   Par�metros: <c> � o endere�o de um ponteiro para um Cabecalho. */
void initCabecalho (Cabecalho **c);

/* Descri��o: insere um atributo em um cabe�alho.
   Par�metros: <c> � o endere�o de um ponteiro para um Cabecalho;
               <a> � o atributo a ser inserido. */
void insertCabecalho (Cabecalho **c, Atributo a);

/* Descri��o: retorna o n�mero de atributos em uma lista.
   Par�metros: <c> � um ponteiro para uma lista de atributos. */
int numAtr (Cabecalho *c);

/* Descri��o: armazena na vari�vel apontada por <atr> o n-�simo
              atributo de uma lista.
   Par�metros: <c> � um ponteiro para uma lista de atributos;
               <n> � a posi��o do atributo a ser recuperado;
               <atr> � um ponteiro para uma vari�vel do tipo Atributo. */
void natr (Cabecalho *c, int n, Atributo *atr);

/* Descri��o: recupera a posi��o do 1� atributo chave em uma lista.
   Par�metros: <c> � um ponteiro para uma lista de atributos.
   Valor de retorno: caso encontre um atributo chave, retorna sua posi��o.
                     Caso n�o encontre, retorna 0.
   Observa��es: em uma lista de atributos deve haver apenas 1 atributo chave. */
int poschv (Cabecalho *c);

/* Descri��o: recupera a posi��o do 1� atributo de ordena��o em uma lista.
   Par�metros: <c> � um ponteiro para uma lista de atributos.
   Valor de retorno: caso encontre um atributo de ordena��o, retorna
                     sua posi��o. Caso n�o encontre, retorna 0.
   Observa��es: em uma lista de atributos deve haver no m�ximo 1
                atributo de ordena��o. */  
int posord (Cabecalho *c);

/* Descri��o: recupera a posi��o de um atributo em uma lista.
   Par�metros: <atr> � o nome do atributo cuja posi��o se quer encontrar.
               <c> � um ponteiro para uma lista de atributos.
   Valor de retorno: caso encontre o atributo de nome <atr>, retorna
                     sua posi��o. Caso n�o encontre, retorna 0. */  
                     
int posfkey (Cabecalho *c);

int posatr (char *atr, Cabecalho *c);

/* Descri��o: libera a mem�ria de uma lista.
   Par�metros: <c> � o endere�o de um ponteiro para uma lista de atributos. */
void fmemCabecalho (Cabecalho **c);


/***** Fun��es para manipula��o das listas de valores (tuplas)*****/


/* Descri��o: inicializa uma lista.
   Par�metros: <t> � o endere�o de um ponteiro para uma lista de valores. */
void initTupla (Tupla **t);

/* Descri��o: insere um valor em uma lista.
   Par�metros: <t> � o endere�o de um ponteiro para uma lista de valores;
               <val> � o valor a ser inserido na lista. */
void insertTupla (Tupla **t, char *val);

/* Descri��o: retorna o n�mero de valores em uma lista.
   Par�metros: <t> � um ponteiro para uma lista de valores. */
int numVal (Tupla *t);

/* Descri��o: armazena na vari�vel apontada por <val> o n-�simo
              valor de uma lista.
   Par�metros: <t> � um ponteiro para uma lista de valores;
               <n> � a posi��o do valor a ser recuperado;
               <val> � uma vari�vel do tipo char*. */
void nval (Tupla *t, int n, char *val);

/* Descri��o: libera a mem�ria de uma lista.
   Par�metros: <t> � o endere�o de um ponteiro para uma lista de valores. */
void fmemTupla (Tupla **t);


/***** Fun��es para manipula��o das listas de tuplas (rela��es)*****/


/* Descri��o: inicializa uma lista.
   Par�metros: <r> � o endere�o de um ponteiro para uma lista de tuplas. */
void initRelacao (Relacao **r);

/* Descri��o: insere uma tupla em uma lista.
   Par�metros: <r> � o endere�o de um ponteiro para uma lista de tuplas;
               <tup> � a tupla a ser inserida na lista. */
void insertRelacao (Relacao **r, Tupla *tup);

/* Descri��o: insere uma tupla em uma lista ordenada crescentemente
              pelo n-�simo atributo (inteiro).
   Par�metros: <r> � o endere�o de um ponteiro para uma lista de tuplas;
               <tup> � a tupla a ser inserida na lista;
               <n> � a posi��o do atributo pelo qual a lista � ordenada. */
void iordinsertRelacao (Relacao **r, Tupla *tup, int n);

/* Descri��o: insere uma tupla em uma lista ordenada crescentemente
              pelo n-�simo atributo (string).
   Par�metros: <r> � o endere�o de um ponteiro para uma lista de tuplas;
               <tup> � a tupla a ser inserida na lista;
               <n> � a posi��o do atributo pelo qual a lista � ordenada. */
void sordinsertRelacao (Relacao **r, Tupla *tup, int n);

/* Descri��o: libera a mem�ria de uma lista.
   Par�metros: <r> � o endere�o de um ponteiro para uma lista de tuplas. */
void fmemRelacao (Relacao **r);
void fmemAtributo (Atributo *att);
