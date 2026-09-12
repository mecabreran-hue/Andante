%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;
extern FILE *yyin;

int yylex(void);
void yyerror(const char *s);

int errores = 0;
%}

%union {
    int num;
    double dec;
    char *str;
}

%token <num> TOK_NUM_ENTERO
%token <dec> TOK_NUM_DECIMAL
%token <str> TOK_ID
%token <str> TOK_CADENA

%token TOK_CAMINAR TOK_LEER TOK_AVANZAR TOK_VIRAR TOK_SINO
%token TOK_HISTORIA TOK_INICIO TOK_FINAL TOK_DETENER
%token TOK_NUMERO TOK_DECIMAL TOK_TEXTO
%token TOK_VERDADERO TOK_FALSO

%token TOK_SUMA TOK_RESTA TOK_MULTIPLICACION TOK_DIVISION
%token TOK_IGUAL TOK_DIFERENTE TOK_MENOR TOK_MAYOR
%token TOK_MENORIGUAL TOK_MAYORIGUAL
%token TOK_AND TOK_OR TOK_NOT
%token TOK_ASIGNACION

%token TOK_PUNTOCOMA TOK_COMA
%token TOK_PARENIZQ TOK_PARENDER
%token TOK_LLAVEIZQ TOK_LLAVEDER
%token TOK_CORCHETEIZQ TOK_CORCHETEDER

%nonassoc TOK_SINO
%nonassoc TOK_VIRAR_SIN_SINO

%left TOK_OR
%left TOK_AND
%left TOK_IGUAL TOK_DIFERENTE
%left TOK_MENOR TOK_MAYOR TOK_MENORIGUAL TOK_MAYORIGUAL
%left TOK_SUMA TOK_RESTA
%left TOK_MULTIPLICACION TOK_DIVISION
%right TOK_NOT
%right TOK_ASIGNACION

%start programa

%%

programa
    : lista_instrucciones
    ;

lista_instrucciones
    : /* vacio */
    | lista_instrucciones instruccion
    ;

instruccion
    : declaracion
    | asignacion
    | entrada
    | salida
    | condicional
    | ciclo
    | bloque
    | error TOK_PUNTOCOMA { yyerrok; }
    ;

declaracion
    : tipo lista_ids TOK_PUNTOCOMA
    | tipo lista_ids_init TOK_PUNTOCOMA
    ;

lista_ids_init
    : TOK_ID TOK_ASIGNACION expresion
    | lista_ids_init TOK_COMA TOK_ID TOK_ASIGNACION expresion
    ;

tipo
    : TOK_NUMERO
    | TOK_DECIMAL
    | TOK_TEXTO
    ;

lista_ids
    : TOK_ID
    | lista_ids TOK_COMA TOK_ID
    ;

asignacion
    : TOK_AVANZAR TOK_ID TOK_ASIGNACION expresion TOK_PUNTOCOMA
    ;

entrada
    : TOK_LEER lista_ids TOK_PUNTOCOMA
    ;

salida
    : TOK_CAMINAR expresion TOK_PUNTOCOMA
    ;

condicional
    : TOK_VIRAR TOK_PARENIZQ expresion TOK_PARENDER bloque %prec TOK_VIRAR_SIN_SINO
    | TOK_VIRAR TOK_PARENIZQ expresion TOK_PARENDER bloque TOK_SINO bloque
    ;

ciclo
    : TOK_HISTORIA TOK_PARENIZQ expresion TOK_PARENDER bloque
    ;

bloque
    : TOK_INICIO lista_instrucciones TOK_FINAL
    ;

expresion
    : expresion TOK_SUMA expresion
    | expresion TOK_RESTA expresion
    | expresion TOK_MULTIPLICACION expresion
    | expresion TOK_DIVISION expresion
    | expresion TOK_IGUAL expresion
    | expresion TOK_DIFERENTE expresion
    | expresion TOK_MENOR expresion
    | expresion TOK_MAYOR expresion
    | expresion TOK_MENORIGUAL expresion
    | expresion TOK_MAYORIGUAL expresion
    | expresion TOK_AND expresion
    | expresion TOK_OR expresion
    | TOK_NOT expresion
    | TOK_PARENIZQ expresion TOK_PARENDER
    | TOK_ID
    | TOK_NUM_ENTERO
    | TOK_NUM_DECIMAL
    | TOK_CADENA
    | TOK_VERDADERO
    | TOK_FALSO
    ;

%%

void yyerror(const char *s) {
    errores++;
    printf("Error Sintactico en linea %d: %s\n", yylineno, s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            perror("Error al abrir archivo");
            return 1;
        }
        yyin = fp;
    }
    
    printf("=== Analizando programa en Andante ===\n\n");
    
    yyparse();
    
    if (errores == 0) {
        printf("\n=== Analisis completado sin errores ===\n");
    } else {
        printf("\n=== Analisis completado con %d errores ===\n", errores);
    }
    
    return 0;
}