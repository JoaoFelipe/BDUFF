CREATE TABLE PROJETO (
NUMERO INTEGER KEY ORD,
NOME_P STRING);

CREATE TABLE EMPREGADO (
CODIGO_E STRING KEY ORD,
NOME_E STRING,
SALARIO INTEGER,
NUM_PROJ INTEGER FK PROJETO);

CREATE TABLE PRODUTO (
CODIGO_PR STRING KEY ORD,
NOME STRING,
COR STRING);

