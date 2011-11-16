/********************************* Entrada.c *********************************/

#include <stdio.h>
#include <string.h>
#include "Entrada.h"

int strcasecmp(const char *s1, const char *s2)
{
    char c1, c2;

    /* Variáveis: c1 -> armazena cada caracter de s1, em caixa baixa.
    			  c2 -> armazena cada caracter de s2, em caixa baixa. */

    while (1)
    {
        c1 = tolower( (unsigned char) *s1++ );	/*Os caracteres são passados */
        c2 = tolower( (unsigned char) *s2++ );	/*para caixa baixa.*/
        if (c1 == 0 || c1 != c2) return c1 - c2; /*Se s1 termina, ou se */
    }										/*c1 != c2, a função termina.*/
}

int primeiroCh(char *s)
{
    int i = 0;

    /* Variáveis: i -> contador. Guarda a posição do próximo caractere válido. */

    while (s[i] == ' ') i++;
    if ((s[i] == 0) || (s[i] == 10)) i = -1; /*Se o 1° caractere for nulo ou de*/
    return(i);						/*quebra de linha, a função retorna -1.*/
}

void retiraQuebras(char *s)
{
    char *ch;

    /* Variáveis: ch -> aponta para quebras de linha em s. */

    ch = strchr(s, 10);	/*Procuramos pela próxima quebra de linha em s.*/
    while ((ch != NULL)  && (ch < &s[strlen(s)]))
    {
        *ch = 32;		/*Se existir, a substituimos por ' '.*/
        ch = strchr(ch, 10);
    }
}

void suprimeEspacos(char *s)
{
    int i, j, k = 0;
    char *temp;

    /* Variáveis: i -> contador. Posição na string s.
    			  j -> deslocamento em s.
    			  k -> contador. Posição em temp.
    			  temp -> armazena a string a ser retornada. */

    temp = (char *) malloc(sizeof(char[255]));
    for (i = primeiroCh(s); i < strlen(s); i++)
    {
        if (s[i] == ' ')	/*Se encontramos um espaço, ignoramos os próximos.*/
        {
            if ((j = primeiroCh(&s[i])) == -1) break;
            temp[k++] = s[i];
            i += j - 1;
            continue;
        }
        if (s[i] != 9) temp[k++] = s[i];
        else temp[k++] = ' ';	/*Se encontramos uma tabulação,*/
    }							/*colocamos um espaço no lugar.*/
    temp[k] = 0;
    strcpy(s, temp);
    free(temp);
}

void normaliza(char *s)
{
    retiraQuebras(s);	/*Retiramos as quebras de linha.*/
    suprimeEspacos(s);	/*E suprimimos os espaços.*/
    suprimeEspacos(s);	/*(2 vezes, para evitar anomalias como tabulações)*/
}

int checaDuploComando(char *cmd)
{
    int i;
    char *ptr;
    char *temp;

    /* Variáveis: i -> contador.
    			  ptr -> ponteiro para percorrer cmd.
    			  temp -> armazena o valor de cmd, em caixa alta.*/

    temp = (char *) malloc(strlen(cmd));
    for (i = 0; cmd[i] != 0; i++)
        temp[i] = toupper(cmd[i]);	/*Passamos cmd para maiúsculas.*/
    temp[--i] = 0;					/*Buscamos duas expressões reservadas.*/
    if (((ptr = strstr(temp, "CREATE TABLE")) != NULL)
            || ((ptr = strstr(temp, "INSERT")) != NULL)
            || ((ptr = strstr(temp, "SELECT")) != NULL)
            || ((ptr = strstr(temp, "UPDATE")) != NULL)
            || ((ptr = strstr(temp, "DELETE")) != NULL))
    {
        ptr++;	/*Buscamos a segunda, a partir do fim da primeira.*/
        if (strstr(ptr, "CREATE TABLE") || strstr(ptr, "INSERT")
                || strstr(ptr, "SELECT") || strstr(ptr, "UPDATE") || strstr(ptr, "DELETE"))
        {
            free(temp);
            return(PNTVRG_FALTANDO);
        }
    }
    free(temp);	/*Caso não se encontre, só existe um comando.*/
    return(0);
}

int proxComando(char *buffer, FILE *arq)
{
    char linha[255], *fim = NULL;
    int i;

    /* Variáveis: linha -> armazena cada linha lida do arquivo.
    			  fim -> ponteiro usado para encontrar o delimitador (';').
    			  i -> usada para acessar posições específicas de linha.*/

    buffer[0] = 0;	/*Inicializamos as variáveis.*/
    linha[0] = 0;

    while ((fim == NULL) && (!feof(arq)))	/*Enquanto não encontra-se ';',*/
    {
        /*lemos mais 1 linha do arquivo.*/
        fgets(linha, 255, arq);
        normaliza(linha);
        i = strlen(linha);
        linha[i] = 32;
        linha[i+1] = 0;
        strcat(buffer, linha);					/*E concatenamos com Buffer.*/
        fim = strchr(buffer, ';');
    }
    if (fim == NULL) return(1);		/*Se o ';' não foi encontrado, erro.*/
    strncpy(linha, buffer, fim - buffer + 1);
    linha[fim - buffer + 1] = 0;		/*O comando é normalizado.*/
    normaliza(linha);
    strcpy(buffer, linha);				/*É preciso checar se não existem duas */
    return(checaDuploComando(buffer));	/*palavras-chave.*/
}					/*Se existirem, falta o ';' no primeiro comando.*/

int proxDelimitador(char *cmd, int *tipo)
{
    int i;

    /* Variáveis: i -> usada para varrer cmd.*/

    for (i = 0; (cmd[i] != '(') && (cmd[i] != ')') && (cmd[i] != ',')
            && (cmd[i] != 0); i++);
    switch (cmd[i])
    {
    case 0:
        return(-1);               /*Se a string termina, não há bloco.*/
    case '(':
        *tipo = 0;
        break;
    case ')':
        *tipo = 1;
        break;
    case ',':
        *tipo = 2;
        break;
    default:
        *tipo = 3;
    }
    return (i);
}

int proxDelimitadorDel(char *cmd, int *tipo)
{
    int i;

    /* Variáveis: i -> usada para varrer cmd.*/

    for (i = 0; (cmd[i] != ' ') && (cmd[i] != 0); i++);
    switch (cmd[i])
    {
    case 0:
        return(-1);
    default:
        *tipo = 0;;
    }
    return (i);
}

int proxDelimitadorWhere(char *cmd, int *tipo)
{
    int i;

    /* Variáveis: i -> usada para varrer cmd.*/

    for (i = 0; (cmd[i] != ' ') && (cmd[i] != '=') && (cmd[i] != 0); i++);
    switch (cmd[i])
    {
    case 0:
        *tipo = 1;
        break;               /*Se a string termina, não há bloco.*/
    case '=':
        *tipo = 0;
        break;
    case ' ':
        *tipo = 1;
        break;
    default:
        *tipo = 3;
    }
    return (i);
}


int proxPalavra(char *s)
{
    int i;

    /* Variáveis: i -> usada para varrer s.*/

    /*Atualização 2010.2 Uma palavra também é determinada por parenteses, abre e fecha*/
    /*for (i = 0; (s[i] != 0) && (s[i] != '(') && (s[i] != ' '); i++); /*Uma palavra é delimitada*/
    /*return (i);										/*por ' ' ou ''.*/

    i=0;
    while(s[i] != 0 && s[i] != '(' && s[i] != ' ')/* && s[i] != ',')*/
        i++;
    return i;
}

int analisaComando(char *c)
{
    int i;
    char cmd[100];
    char param[300];
    char temp[70];

    /* Variáveis: i -> usada para marcar posições específicas em c.
    			  cmd -> armazena a o comando.
    			  param -> armazena os parâmetros do comando (todo o resto).
                  temp -> armazena a 1a. palavra-chave do comando. */

    i = proxPalavra(c);
    strncpy(temp, c, i);
    temp[i] = 0;
    if (!strcasecmp(temp, "create") || !strcasecmp(temp, "insert") ||
            !strcasecmp(temp, "delete"))
        i += proxPalavra(&c[i + 1]) + 1;
    if (c[i] == 0) return(CMD_DESCONHECIDO);
    strncpy(cmd, c, i);
    cmd[i] = 0;
    normaliza(cmd);
    strncpy(param, &c[i+1], strlen(c) - i - 1);
    param[strlen(c) - i - 2] = 0;	/*Tentamos descobrir qual é o comando.*/
    if (!strcasecmp(cmd, "create table")) return(analisaCreate(param));
    else if (!strcasecmp(cmd, "insert into")) return(analisaInsert(param));
    else if (!strcasecmp(cmd, "delete from")) return(analisaDelete(param));
    else if (!strcasecmp(cmd, "update")) return(analisaUpdate(param));
    else if (!strcasecmp(cmd, "select")) return(analisaSelect(param));
    else return(CMD_DESCONHECIDO);	/*Caso não seja um deles, erro.*/
}

int analisaCreate(char *cmd)
{
    int i, j, tipo;
    char nome[30], temp[100];
    Cabecalho *c;
    Atributo a;

    /* Variáveis: i -> usada para acessar posições específicas em cmd.
                  j -> usada para acessar posições específicas em cmd.
                  tipo -> armazena o tipo de bloco.
                  nome -> armazena o nome da tabela a ser criada.
                  temp -> armazena cada atributo (com suas definições).
                  c -> cabeçalho da tabela.
                  a -> armazena cada atributo a ser inserido na lista.*/

    initCabecalho(&c);
    i = proxDelimitador(cmd, &tipo);	/*Se não encontrarmos '(', erro de sintaxe.*/
    if (tipo != 0)
    {
        fmemCabecalho(&c);
        return(PAR_ESQ_FALTANDO);
    }

    strncpy(nome, cmd, i);	/*Copiamos o nome da tabela.*/
    nome[i] = 0;
    suprimeEspacos(nome);

    j = i + 1;
    i = proxDelimitador(&cmd[j], &tipo);
    while (i != -1)				/*Enquanto há atributos,*/
    {
        strncpy(temp , &cmd[j], i);	/*copiamos de cmd.*/
        temp[i] = 0;
        j += i + 1;
        normaliza(temp);
        if (i = decAtributo(&a, temp))
        {
            fmemCabecalho(&c);
            return(i);
        }
        insertCabecalho(&c, a);	/*E inserimos na lista.*/
        i = proxDelimitador(&cmd[j], &tipo);
    }
    createTable(nome, c);	/*A função específica é chamada.*/
    return(0);
}

int analisaInsert(char *cmd)
{
    int i, j, tipo;
    char nome[30], temp[100];
    Tupla *t;

    /* Variáveis: i -> usada para acessar posições específicas em cmd.
    			  j -> usada para acessar posições específicas em cmd.
    			  tipo -> armazena o tipo do bloco.
    			  nome -> armazena o nome da tabela.
    			  temp -> partes do comando a serem interpretadas.
    			  t -> ponterio para uma lista de valores a serem inseridos.*/

    initTupla(&t);
    i = proxPalavra(cmd);
    strncpy(nome, cmd, i);	/*O primeiro identificador é o nome da tabela.*/
    nome[i] = 0;
    j = i + 1;
    i = proxDelimitador(&cmd[j], &tipo);	/*Os valores têm que vir entre parêntesis.*/
    if (tipo != 0)
    {
        fmemTupla(&t);
        return(PAR_ESQ_FALTANDO);
    }
    strncpy(temp, &cmd[j], i);
    temp[i] = 0;
    normaliza(temp);
    if (strcasecmp(temp, "values"))	/*Em seguida temos a palavra "values".*/
    {
        fmemTupla(&t);
        return(VALUES_FALTANDO);
    }
    j += i + 1;
    i = proxDelimitador(&cmd[j], &tipo);	/*Os valores são lidos*/
    while (i != -1)
    {
        strncpy(temp , &cmd[j], i);
        temp[i] = 0;
        j += i + 1;
        normaliza(temp);
        insertTupla(&t, temp);			/*e colocados na lista.*/
        i = proxDelimitador(&cmd[j], &tipo);
    }
    insert(nome, t);
    return(0);
}

int verificarCondicao(char *gmcondicao, Condicao *c) {
    char *ch, *ch2;

    /* Variáveis: ch -> usada para ler o lado direito(atributo) da condicao 
                  ch2 -> usada para ler o lado esquerdo(valor) da condicao
    */

  
    if ((ch = strchr(gmcondicao, '=')) != NULL)             /*Procuramos por '=' e colocamos a posicao em ch*/
    {
        
        if ((ch2 = strchr(gmcondicao, '>')) != NULL)        /*Procuramos por '>' e colocamos a posicao em ch2*/
        {           
            c->opr[0] = '%';
        } else if ((ch2 = strchr(gmcondicao, '<')) != NULL) /*Procuramos por '<' e colocamos a posicao em ch2*/ 
        {
            c->opr[0] = '@';
        } else {                                            /*se não encontrar, colocamos a posicao do '=' em ch2*/
            ch2 = ch;
            c->opr[0] = '=';
        }
    } else if ((ch = strchr(gmcondicao, '>')) != NULL) {    /*Procuramos por '>' e colocamos a posicao em ch*/
        if ((ch2 = strchr(gmcondicao, '<')) != NULL)        /*Procuramos por '<' e colocamos a posicao em ch2*/  
        {   
            c->opr[0] = '#';
        } else {                                            /*se não encontrar, colocamos a posicao do '>' em ch2*/  
            ch2 = ch;
            c->opr[0] = '>';
        } 
    } else if ((ch = strchr(gmcondicao, '<')) != NULL) {    /*Procuramos por '<' e colocamos a posicao em ch e ch2*/
        ch2 = ch;
        c->opr[0] = '<';
    } else {
        return(COMPARACAO_INVLD);
    }
    
    strncpy(c->val, &ch[1], strlen(gmcondicao) - ((int) (ch - gmcondicao))); /* Pega o valor da comparacao */
    c->val[strlen(gmcondicao) - ((int) (ch - gmcondicao))] = '\0';           /* a direira do ch */
       
    strncpy(c->atr1, gmcondicao, (int) (ch2 - gmcondicao));   /* Pega o nome do atributo */
    c->atr1[ch2 - gmcondicao] = '\0';                         /* a esquerda do ch2 */ 
    normaliza(c->atr1);
    c->opr[1] = '\0';
    return 0;
}

int analisaSelect(char *cmd)
{
    int i, j,k, tipo;
    int isDistinct=0, isOrderBy=0, isCount=0, isSum=0, isAvg=0, isMin=0, isMax=0;
    char nome[30],ParentesesErro[10], temp[100], *ch, SumAndAvg[10], listaAtrib[200],distinct[10],count[10], orderby[20], gmcondicao[20], orderbyAtributos[100];
    Condicao c;

    /* Variáveis: i -> usada para acessar posições específicas em cmd.
    			  j -> usada para acessar posições específicas em cmd.
    			  tipo -> armazena o tipo de bloco.
    			  nome -> armazena o nome da tabela a ser consultada.
    			  temp -> partes do comando a serem interpretadas.
    			  ch -> usada para ler a condição.
    			  c -> condição de Seleção.*/

    i = proxPalavra(cmd);
    strncpy(distinct, cmd, i);
    distinct[i] = 0;

    if(strcasecmp(distinct,"distinct")==0)
    {

        isDistinct = 1;
        j = i+1;
        i = proxPalavra(&cmd[j]);
        strncpy(listaAtrib, &cmd[j], i);
    }
    else
    {
        strncpy(listaAtrib, cmd, i);	/*O primeiro identificador é a lista de atributos.*/

        /*Tratamento do COUNT,SUM,MIN,MAX,AVG 2010.2*/
        listaAtrib[i] = 0;
        if(strcasecmp(listaAtrib,"avg")==0)
        {
            isAvg = 1;

            i = proxPalavra(cmd); /*Pega o tamanho de Sum*/
            j = i+1;

            i = proxPalavra(&cmd[j]); /*Pega o argumento de Min*/

            strncpy(listaAtrib,&cmd[j],i); /*Pega argumento*/
            listaAtrib[i]=0;
            if(strcasecmp(listaAtrib,"distinct")!=0)
                listaAtrib[i-1] = 0; /*Retira parenteses*/
            else
                listaAtrib[i] = 0; /*Senão mantém a cláusula DISTINCT*/

            /*Dentro de Sum poderá conter a cláusula Distinct 2010.2*/
            if(strcasecmp(listaAtrib,"distinct")==0)
            {
                isDistinct = 1;
                j = i + j + 1; /*Pula distinct e espaço em branco para alcançar argumentos*/
                i = proxPalavra(&cmd[j]);
                strncpy(listaAtrib, &cmd[j], i);
                listaAtrib[i-1]=0;
            }

            /*Pega próximo parenteses*/
            j += i - 1;
            strncpy(ParentesesErro, &cmd[j], 1);
            ParentesesErro[1] = 0;

            /*Pula para próxima palavra, segundo a sintaxe estabelecida*/
            i = j + 1;

            if(strcasecmp(ParentesesErro,")")!=0) /*Se o Count for para todos, apenas aceita asterístico como parâmetro*/
                return(AVG_ERRADO);

        }
        else if(strcasecmp(listaAtrib,"sum")==0)
        {
            isSum = 1;

            i = proxPalavra(cmd); /*Pega o tamanho de Sum*/
            j = i+1;

            i = proxPalavra(&cmd[j]); /*ega o argumento de Min*/

            strncpy(listaAtrib,&cmd[j],i); /*Pega argumento*/
            listaAtrib[i]=0;
            if(strcasecmp(listaAtrib,"distinct")!=0)
                listaAtrib[i-1] = 0; /*Retira parenteses*/
            else
                listaAtrib[i] = 0; /*Senão mantém a cláusula DISTINCT*/

            /*Dentro de Sum poderá conter a cláusula Distinct 2010.2*/
            if(strcasecmp(listaAtrib,"distinct")==0)
            {
                isDistinct = 1;
                j = i + j + 1; /*Pula distinct e espaço em branco para alcançar argumentos*/
                i = proxPalavra(&cmd[j]);
                strncpy(listaAtrib, &cmd[j], i);
                listaAtrib[i-1]=0;
            }

            /*Pega próximo parenteses*/
            j += i - 1;
            strncpy(ParentesesErro, &cmd[j], 1);
            ParentesesErro[1] = 0;

            /*Pula para próxima palavra, segundo a sintaxe estabelecida*/
            i = j + 1;

            if(strcasecmp(ParentesesErro,")")!=0) /*Se o Count for para todos, apenas aceita asterístico como parâmetro*/
                return(SUM_ERRADO);
        }
        else if(strcasecmp(listaAtrib,"count")==0)
        {
            isCount = 1;
            i = proxPalavra(cmd);
            j = i+1;

            strncpy(listaAtrib,&cmd[j],1); /*Pega asterístico*/
            listaAtrib[1] = 0;

            /*Pega próximo parenteses*/
            i++;
            j = i + 1;
            strncpy(ParentesesErro, &cmd[j], i);
            ParentesesErro[1] = 0;

            /*Pula para próxima palavra, segundo a sintaxe estabelecida*/
            i += 2;

            if(strcasecmp(listaAtrib,"*")!=0 || strcasecmp(ParentesesErro,")")!=0) /*Se o Count for para todos, apenas aceita asterístico como parâmetro*/
                return(COUNT_ERRADO);
        }
        else
        {
            if(strcasecmp(listaAtrib,"min")==0)
            {
                isMin = 1; /*A cláusula isMin é ativada*/

                i = proxPalavra(cmd); /*Pega o tamanho de Min*/
                j = i+1;

                i = proxPalavra(&cmd[j]); /*Pega o argumento de Min*/

                strncpy(listaAtrib,&cmd[j],i); /*Pega aargumento*/
                listaAtrib[i-1] = 0; /*Retira Parenteses do argumento*/

                /*Pega próximo parenteses*/
                i+=2;
                j = i + 1;
                strncpy(ParentesesErro, &cmd[j], 1);
                ParentesesErro[1] = 0;

                /*Pula para próxima palavra, segundo a sintaxe estabelecida*/
                i += 2;

                if(strcasecmp(ParentesesErro,")")!=0) /*Se o Count for para todos, apenas aceita asterístico como parâmetro*/
                    return(MIN_ERRADO);
            }
            else if(strcasecmp(listaAtrib,"max")==0)
            {
                isMax = 1;

                i = proxPalavra(cmd); /*Pega o tamanho de Max*/
                j = i+1;

                i = proxPalavra(&cmd[j]); /*Pega o argumento de Max*/

                strncpy(listaAtrib,&cmd[j],i); /*Pega argumento*/
                listaAtrib[i-1] = 0;

                /*Pega próximo parenteses*/
                i+=2;
                j = i + 1;
                strncpy(ParentesesErro, &cmd[j], 1);
                ParentesesErro[1] = 0;

                /*Pula para próxima palavra, segundo a sintaxe estabelecida*/
                i += 2;

                if(strcasecmp(ParentesesErro,")")!=0) /*Se o Count for para todos, apenas aceita asterístico como parâmetro*/
                    return(MAX_ERRADO);
            }
        }


        /*Fim do tratamento do COUNT,SUM,MIN,MAX,AVG 2010.2*/

        j=0;
    }
    if(!isDistinct && isSum) listaAtrib[i-1] = 0;
    else listaAtrib[i] = 0;

    j += i + 1;
    i = proxPalavra(&cmd[j]);
    strncpy(temp, &cmd[j], i);

    temp[i] = 0;
    if (strcasecmp(temp, "from")) return (FROM_FALTANDO); /*Se não encontramos a palavra*/
    j += i + 1;								/*reservada "from", erro.*/
    if (cmd[j] == '(') return(analisaJoin(listaAtrib, &cmd[j]));	/*Se há uma condição de junção,*/
    i = proxPalavra(&cmd[j]);				/*chamamos uma função especializada.*/
    strncpy(nome, &cmd[j], i);			/*Lemos o nome da tabela.*/
    nome[i] = 0;
    j += i + 1;

    /* GM: Inicio Order By */
    i = proxPalavra(&cmd[j]);
    strncpy(orderby, &cmd[j], i);
    orderby[i]=0;

    if (strcasecmp(orderby, "order")==0)
    {
        j += i + 1;
        i = proxPalavra(&cmd[j]);
        strncpy(orderby, &cmd[j], i);
        orderby[i]=0;

        if (strcasecmp(orderby, "by")==0)
        {
            /* GM: DIGITOU ORDER BY!! */
            j += i + 1;
            i = proxPalavra(&cmd[j]);

            /* GM: orderbyAtributos = atributo a se ordenado */
            strncpy(orderbyAtributos, &cmd[j], i);

            j += i + 1; /* GM: Não mexer */

            isOrderBy=1;
        }
        else
        {
            /* GM: DIGITOU ORDER, MAS NAO DIGITOU 'BY', MOSRTRA ERRO */
        }
    }
    /* GM: Fim Order By */



    if (cmd[j - 1] == 0)					/*Se o comando acabou, é um select sem*/
    {
        /*condição.*/
        selectSQL(nome, listaAtrib, NULL,isDistinct,isOrderBy,isCount,isSum,isMin,isMax,isAvg);
        return(0);
    }


    i = proxPalavra(&cmd[j]);
    strncpy(temp, &cmd[j], i);
    temp[i] = 0;							/*Caso contrário, devemos achar a*/
    if (strcasecmp(temp, "where")) return (WHERE_FALTANDO);	/*cláusula "where".*/
    j += i + 1;



    /* GM: Separa o ORDER BY do texto */

    strcpy(temp, &cmd[j]); /* GM: temp = TUDO APOS WHERE */

    char *posOB;
    posOB = strstr(temp, "ORDER BY"); /* GM: posicao que o order by se encontra */

    if (posOB != 0)
    {
        strncpy(gmcondicao, &cmd[j], (int)(posOB-temp-1)); /* GM: gmcondicao = Condicao do WHERE */
        j += (posOB-temp);
        strcpy(temp, &cmd[j]); /* GM: temp = ORDER BY xxx*/

        j+= 9;
        i = proxPalavra(&cmd[j]);

        /* GM: orderbyAtributos = atributo a se ordenado */
        strncpy(orderbyAtributos, &cmd[j], i);


        isOrderBy=1;
    }
    /* GM: VE SE TEM order by (minusculo) */
    else
    {
        posOB = strstr(temp, "order by");
        if (posOB != 0)
        {
            strncpy(gmcondicao, &cmd[j], (int)(posOB-temp-1)); /* GM: gmcondicao = Condicao do WHERE */
            j += (posOB-temp);
            strcpy(temp, &cmd[j]); /* GM: temp = ORDER BY xxx*/

            j+= 9;
            i = proxPalavra(&cmd[j]);

            /* GM: orderbyAtributos = atributo a se ordenado */
            strncpy(orderbyAtributos, &cmd[j], i);


            isOrderBy=1;
        }
    }

    /* GM: Se não for order by continua normalmente */
    if (isOrderBy != 1) strcpy(gmcondicao, temp);

    /* GM: Fim da remoção do ORDER BY do texto */


    
    if (verificarCondicao(&gmcondicao, &c))
        return(COMPARACAO_INVLD);
             
    selectSQL(nome, listaAtrib, &c,isDistinct,isOrderBy,isCount,isSum,isMin,isMax,isAvg);/*Se a função chega aqui, temos um select com condição c.*/
    printf("Selecao conluida!");
    return(0);
}

int analisaJoin(char *listaAtrib, char *cmd)
{
    int i, j;
    char temp[30], *ch;
    char nome1[30], nome2[30];
    Condicao c, cj;

    /* Variáveis: i -> usada para acessar posições específicas em cmd.
    			  j -> usada para acessar posições específicas em cmd.
    			  temp -> partes do comando a serem interpretadas.
    			  ch -> usada para ler a condição.
    			  nome1 -> armazena o nome da 1ª tabela da condição.
    			  nome2 -> armazena o nome da 2ª tabela da condição.
    			  c -> condição de seleção.
    			  cj -> condição de junção.*/

    i = proxPalavra(&cmd[1]);		/*O primeiro indentificador deve ser o nome da*/
    strncpy(nome1, &cmd[1], i);	/*primeira tabela da junção.*/
    nome1[i] = 0;
    j = i + 2;
    i = proxPalavra(&cmd[j]);
    strncpy(temp, &cmd[j], i);
    temp[i] = 0;
    if (strcasecmp(temp, "join")) return(JOIN_FALTANDO);	/*Deve-se encontrar "join".*/
    j += i + 1;
    i = proxPalavra(&cmd[j]);
    strncpy(nome2, &cmd[j], i);	/*Em seguida, vem o nome da segunda tabela*/
    nome2[i] = 0;
    j += i + 1;
    i = proxPalavra(&cmd[j]);
    strncpy(temp, &cmd[j], i);
    temp[i] = 0;
    if (strcasecmp(temp, "on")) return(ON_FALTANDO);	/*e a palavra "on".*/
    j += i + 1;
    i = proxPalavra(&cmd[j]);
    strncpy(temp, &cmd[j], i);
    temp[i] = 0;
    if ((ch = strchr(temp, '=')) == NULL)	/*Chega-se à condição de junção.*/
    {
        if (cmd[j + i + 1] != '=') return(COMPARACAO_INVLD);
        strcpy(cj.atr1, temp);
        j += i + 3;
        i = proxPalavra(&cmd[j]);
        if (cmd[i + j - 1] == ')') i--;
        strncpy(c.atr2, &cmd[j], i);
        cj.atr2[i] = 0;
    }
    else
    {
        if ((ch = strchr(temp, '=')) == NULL) return(COMPARACAO_INVLD);
        strncpy(cj.atr1, temp, (int) (ch - temp));
        cj.atr1[(int) (ch - temp)] = 0;
        if (temp[strlen(temp) - 1] == ')') temp[strlen(temp) - 1] = 0;
        strncpy(cj.atr2, &ch[1], strlen(temp) - ((int) (ch - temp)));
        cj.atr2[strlen(temp) - ((int) (ch - temp))] = 0;
    }
    j += i + 1;					/*Se o comando terminou, temos um select com*/
    if (cmd[j - 1] == 0)			/*join, sem condição de seleção.*/
    {
        join(nome1, nome2, listaAtrib, cj, NULL);
        return(0);
    }
    i = proxPalavra(&cmd[j]);		/*Senão, é preciso ler a condição.*/
    strncpy(temp, &cmd[j], i);	/*Idêntico à função anterior.*/
    temp[i] = 0;
    if (strcasecmp(temp, "where")) return (WHERE_FALTANDO);
    j += i + 1;
    strcpy(temp, &cmd[j]);
    if (verificarCondicao(&temp, &c))
        return(COMPARACAO_INVLD);
    
    join(nome1, nome2, listaAtrib, cj, &c);
    return(0);
}

int analisaDelete(char *cmd)
{
    int i, j, tipo, erro;
    char nome[30], temp[100], atrib[30], valor[256];

    /* Variáveis: i -> usada para acessar posições específicas em cmd.
    			  j -> usada para acessar posições específicas em cmd.
    			  tipo -> armazena o tipo do bloco.
    			  nome -> armazena o nome da tabela.
    			  temp -> partes do comando a serem interpretadas.*/
    i = proxPalavra(cmd);
    strncpy(nome, cmd, i);	/*O primeiro identificador é o nome da tabela.*/
    nome[i] = 0;
    j = i + 1;
    i = proxDelimitadorDel(&cmd[j], &tipo);
    if (i == -1)
        deleteAll(nome);
    else
    {
        strncpy(temp, &cmd[j], i);
        temp[i] = 0;
        normaliza(temp);
        if (strcasecmp(temp, "where"))	/*Em seguida temos a palavra "where" ou ";".*/
        {
            return(WHERE_FALTANDO);
        }
        else
        {
            j += i + 1;
            erro = analisaWhere(cmd, j, atrib, valor);
            if (erro)
            {
                return erro;
            }
            else
            {
                /*printf("nome %s, atributo = %s, valor = %s\n",nome, atrib, valor);*/
                deleteCond(nome, atrib, valor);
            }
        }
    }
    return(0);
}

int analisaUpdate(char *cmd)
{
    printf("comando = %s\n", cmd);
    int i, j, tipo, erro;
    char nome[30], temp[100], atribSet[30], valorSet[256], atribWhere[30], valorWhere[256];

    /* Variáveis: i -> usada para acessar posições específicas em cmd.
    			  j -> usada para acessar posições específicas em cmd.
    			  tipo -> armazena o tipo do bloco.
    			  nome -> armazena o nome da tabela.
    			  temp -> partes do comando a serem interpretadas.*/
    i = proxPalavra(cmd);
    strncpy(nome, cmd, i);	/*O primeiro identificador é o nome da tabela.*/
    nome[i] = 0;
    j = i + 1;
    i = proxDelimitadorDel(&cmd[j], &tipo);
    if (i == -1)
    {
        return(SET_FALTANDO);
    }
    else
    {
        strncpy(temp, &cmd[j], i);
        temp[i] = 0;
        normaliza(temp);
        if (strcasecmp(temp, "set"))	/*Em seguida temos a palavra "set".*/
        {
            return(SET_FALTANDO);
        }
        else
        {
            j += i + 1;
            erro = analisaSet(cmd, &j, atribSet, valorSet, i);
    printf("comando set analisado com erro = %i", erro);            
            if (erro)
            {
                return erro;
            }
            else
            {
                i = proxDelimitadorDel(&cmd[j], &tipo);
                if (i == -1){
                    printf("inicia atualização de tudo\n");
                    updateAll(nome, atribSet, valorSet);
                }
                else
                {
                    printf("inicia atualização condicional\n");
                    strncpy(temp, &cmd[j], i);
                    printf("temp = %s\n", temp);
                    temp[i] = 0;
                    normaliza(temp);
                    if (strcasecmp(temp, "where"))	/*Em seguida temos a palavra "where".*/
                        return(WHERE_FALTANDO);
                    else
                    {
                        j += i + 1;
                        erro = analisaWhere(cmd, j, atribWhere, valorWhere);
                        if (erro)
                        {
                            return erro;
                        }
                        else
                        {
                            updateCond(nome, atribSet, valorSet, atribWhere, valorWhere);
                        }
                    }
                }
            }
        }
    }
    return(0);
}

int analisaWhere(char *cmd, int j, char *atrib, char *valor)
{
    int i, tipo;
    i = proxDelimitadorWhere(&cmd[j], &tipo);
    strncpy(atrib , &cmd[j], i);
    atrib[i] = 0;
    normaliza(atrib);
    if ((tipo != 0) || (atrib[0] == 0))
        return NOMEATR_FALTANDO;
    else
    {
        j += i + 1;
        i = proxDelimitadorWhere(&cmd[j], &tipo);
        strncpy(valor , &cmd[j], i);
        valor[i] = 0;
        normaliza(valor);
        if ((tipo != 1) || (valor[0] == 0))
            return COMPARACAO_INVLD;
    }
    return 0;
}

int analisaSet(char *cmd, int *j, char *atrib, char *valor, int ind)
{
    int i, tipo;
    i = proxDelimitadorWhere(&cmd[(*j)], &tipo);
    strncpy(atrib , &cmd[(*j)], i);
    atrib[i] = 0;
    normaliza(atrib);
    if ((tipo != 0) || (atrib[0] == 0))
        return NOMEATR_FALTANDO;
    else
    {
        (*j) += i + 1;
        i = proxDelimitadorWhere(&cmd[(*j)], &tipo);
        strncpy(valor , &cmd[(*j)], i);
        valor[i] = 0;
        normaliza(valor);
        (*j) += i + 1;
        if ((tipo != 1) || (valor[0] == 0))
            return COMPARACAO_INVLD;
    }
    return 0;
}

int decAtributo(Atributo *a, char *s)
{
    int i, j = 0;
    char sub[60];

    /* Variáveis: i -> usada para acessar posições específicas em s.
    			  j -> usada para acessar posições específicas em s.
    			  sub -> armazena parte de s para manipulação.*/

    i = proxPalavra(s);
    if ((i == 0) || (j >= strlen(s))) return(NOMEATR_FALTANDO);
    strncpy(a->nome, s, i);
    a->nome[i] = 0;	/*O primeiro identificador deve ser o nome do atributo.*/
    j = proxPalavra(&s[i + 1]);
    if ((j == 0) || (i >= strlen(s))) return(TIPOATR_FALTANDO);
    strncpy(sub, &s[i + 1], j);
    sub[j] = 0;				/*Em seguida, temos o tipo que deve ser "INTEGER"*/
    if (!strcasecmp(sub, "INTEGER")) a->tipo = INTEGER;	/* ou "STRING"*/
    else if (!strcasecmp(sub, "STRING")) a->tipo = STRING;
    else return(TIPO_INVALIDO);
    a->ord = 0;
    a->key = 0;
    a->fkey = 0;
    a->conta_ref = 0;
    a->tipo_ref=RESTRICT;
    while(i!=0)
    {
        memset(sub,'\0',sizeof(char)*60);
        j += i + 1;
        i = proxPalavra(&s[j+1]);
        if ((i == 0) || (j >= strlen(s))) return(0);
        strncpy(sub, &s[j + 1], i);

        sub[i] = 0;		/*Precisamos ler ainda se o atributo é ou não chave,*/
        if (!strcasecmp(sub, "KEY")) a->key = 1;		/*e se a relação está*/
        else if (!strcasecmp(sub, "ORD")) a->ord = 1;	/*ordenada por ele.*/
        else if (!strcasecmp(sub, "FK"))               /*chave estrangeira.*/
        {
            a->fkey = 1;
            memset(sub,'\0',sizeof(char)*60);
            j += i + 1;
            i = proxPalavra(&s[j+1]);
            strncpy(sub, &s[j + 1], i);

            strcpy(a->f_referencia,sub);
            a->tipo_ref = RESTRICT;


        }
        else if (!strcasecmp(sub, "CASC"))
        {
            if (a->fkey) a->tipo_ref=CASCADE;
            else return(IDT_DESCONHECIDO);
        }
        else if (!strcasecmp(sub, "RESTRICT"))
        {
            if (a->fkey) a->tipo_ref=RESTRICT;
            else return(IDT_DESCONHECIDO);
        }
        else if (!strcasecmp(sub, "KEY"))
        {
            if (a->key) return(ESPEC_DUPLICADA);
            else a->key = 1;
        }
        else if (!strcasecmp(sub, "ORD"))
        {
            if (a->ord) return(ESPEC_DUPLICADA);
            else a->ord = 1;
        }
        else
        {
            return(IDT_DESCONHECIDO);
        }
    }
    if (i != 0) return(PARAM_EXCEDENTES);	/*Se ainda houver algo no comando, erro.*/
    return(0);
}

int interpreta(FILE *arq)
{
    char buffer[300];
    int erro, j = 1;

    /* Variáveis: buffer -> armazena um comando do arquivo.
    			  erro -> flag de erro na leitura.
    			  j -> contador de comandos.*/

    while (!feof(arq))	/*Enquanto há comandos no arquivo,*/
    {
        erro = proxComando(buffer, arq);	/*lemos, */
        normaliza(buffer);
        if (primeiroCh(buffer) == -1) continue;	/*verificamos erros, */
        if (erro) erroSintaxe(erro, j);
        if (erro = analisaComando(buffer)) erroSintaxe(erro, j);	/*e executamos.*/
        j++;
    }
    fclose(arq);
    return(0);	/*Se a função chega até aqui, não houve erros.*/
}
