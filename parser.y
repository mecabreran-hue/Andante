%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;
extern FILE *yyin;

int yylex(void);
void yyerror(const char *s);

int errores = 0;

/* ============================================================
   DEFINICION DE NODOS DEL AST
   ============================================================ */

typedef enum {
    NODO_PROGRAMA,
    NODO_DECLARACION,
    NODO_ASIGNACION,
    NODO_ENTRADA,
    NODO_SALIDA,
    NODO_CONDICIONAL,
    NODO_CICLO,
    NODO_BLOQUE,
    NODO_BINARIA,
    NODO_UNARIA,
    NODO_ID,
    NODO_NUM_ENTERO,
    NODO_NUM_DECIMAL,
    NODO_CADENA,
    NODO_BOOL
} TipoNodo;

typedef struct Nodo {
    TipoNodo tipo;
    char *valor;
    char *tipo_dato;
    int linea;
    struct Nodo *izq;
    struct Nodo *der;
    struct Nodo *hijos[10];
    int num_hijos;
} Nodo;

Nodo *raiz = NULL;

Nodo *crear_nodo(TipoNodo tipo, char *valor, int linea) {
    Nodo *n = (Nodo *)malloc(sizeof(Nodo));
    n->tipo = tipo;
    n->valor = valor ? strdup(valor) : NULL;
    n->tipo_dato = NULL;
    n->linea = linea;
    n->izq = NULL;
    n->der = NULL;
    n->num_hijos = 0;
    return n;
}

void agregar_hijo(Nodo *padre, Nodo *hijo) {
    if (padre && hijo && padre->num_hijos < 10) {
        padre->hijos[padre->num_hijos++] = hijo;
    }
}

/* ============================================================
   TABLA DE SIMBOLOS
   ============================================================ */

typedef struct Simbolo {
    char *nombre;
    char *tipo;
    int inicializada;
    int linea;
    struct Simbolo *sig;
} Simbolo;

Simbolo *tabla_simbolos = NULL;

void insertar_simbolo(char *nombre, char *tipo, int inicializada, int linea) {
    Simbolo *actual = tabla_simbolos;
    while (actual != NULL) {
        if (strcmp(actual->nombre, nombre) == 0) {
            printf("Advertencia: '%s' ya declarada en linea %d\n", nombre, actual->linea);
            return;
        }
        actual = actual->sig;
    }
    
    Simbolo *nuevo = (Simbolo *)malloc(sizeof(Simbolo));
    nuevo->nombre = strdup(nombre);
    nuevo->tipo = strdup(tipo);
    nuevo->inicializada = inicializada;
    nuevo->linea = linea;
    nuevo->sig = tabla_simbolos;
    tabla_simbolos = nuevo;
}

Simbolo *buscar_simbolo(char *nombre) {
    Simbolo *actual = tabla_simbolos;
    while (actual != NULL) {
        if (strcmp(actual->nombre, nombre) == 0) return actual;
        actual = actual->sig;
    }
    return NULL;
}

void imprimir_tabla_simbolos() {
    printf("\n========== TABLA DE SIMBOLOS ==========\n");
    printf("%-20s %-10s %-15s %-6s\n", "Nombre", "Tipo", "Inicializada", "Linea");
    printf("------------------------------------------------------\n");
    Simbolo *actual = tabla_simbolos;
    while (actual != NULL) {
        printf("%-20s %-10s %-15s %-6d\n",
               actual->nombre,
               actual->tipo,
               actual->inicializada ? "si" : "no",
               actual->linea);
        actual = actual->sig;
    }
    printf("=======================================\n");
}

/* ============================================================
   IMPRIMIR AST
   ============================================================ */

const char *nombre_nodo(TipoNodo tipo) {
    switch(tipo) {
        case NODO_PROGRAMA: return "Programa";
        case NODO_DECLARACION: return "Declaracion";
        case NODO_ASIGNACION: return "Asignacion";
        case NODO_ENTRADA: return "Entrada";
        case NODO_SALIDA: return "Salida";
        case NODO_CONDICIONAL: return "Condicional";
        case NODO_CICLO: return "Ciclo";
        case NODO_BLOQUE: return "Bloque";
        case NODO_BINARIA: return "Operacion";
        case NODO_UNARIA: return "Unaria";
        case NODO_ID: return "Identificador";
        case NODO_NUM_ENTERO: return "Numero";
        case NODO_NUM_DECIMAL: return "Decimal";
        case NODO_CADENA: return "Cadena";
        case NODO_BOOL: return "Booleano";
        default: return "Desconocido";
    }
}

void imprimir_ast(Nodo *nodo, int nivel) {
    if (nodo == NULL) return;
    
    for (int i = 0; i < nivel; i++) printf("  ");
    
    printf("%s", nombre_nodo(nodo->tipo));
    if (nodo->valor) printf(": %s", nodo->valor);
    if (nodo->tipo_dato) printf(" [%s]", nodo->tipo_dato);
    printf("\n");
    
    for (int i = 0; i < nodo->num_hijos; i++) {
        imprimir_ast(nodo->hijos[i], nivel + 1);
    }
    if (nodo->izq) imprimir_ast(nodo->izq, nivel + 1);
    if (nodo->der) imprimir_ast(nodo->der, nivel + 1);
}

%}

%union {
    int num;
    double dec;
    char *str;
    struct Nodo *nodo;
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

%type <nodo> programa lista_instrucciones instruccion
%type <nodo> declaracion asignacion entrada salida condicional ciclo bloque
%type <nodo> expresion tipo lista_ids lista_ids_init

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
        {
            raiz = crear_nodo(NODO_PROGRAMA, NULL, 0);
            agregar_hijo(raiz, $1);
            $$ = raiz;
        }
    ;

lista_instrucciones
    : /* vacio */
        { $$ = NULL; }
    | lista_instrucciones instruccion
        {
            if ($1 == NULL) {
                $$ = crear_nodo(NODO_BLOQUE, NULL, 0);
            } else {
                $$ = $1;
            }
            if ($2 != NULL) agregar_hijo($$, $2);
        }
    ;

instruccion
    : declaracion    { $$ = $1; }
    | asignacion     { $$ = $1; }
    | entrada        { $$ = $1; }
    | salida         { $$ = $1; }
    | condicional    { $$ = $1; }
    | ciclo          { $$ = $1; }
    | bloque         { $$ = $1; }
    | error TOK_PUNTOCOMA { yyerrok; $$ = NULL; }
    ;

declaracion
    : tipo lista_ids TOK_PUNTOCOMA
        {
            $$ = crear_nodo(NODO_DECLARACION, $1->valor, yylineno);
            agregar_hijo($$, $2);
            
            /* Insertar cada ID en la tabla */
            Nodo *actual = $2;
            while (actual != NULL) {
                insertar_simbolo(actual->valor, $1->valor, 0, yylineno);
                actual = actual->der;
            }
        }
    | tipo lista_ids_init TOK_PUNTOCOMA
        {
            $$ = crear_nodo(NODO_DECLARACION, $1->valor, yylineno);
            agregar_hijo($$, $2);
            
            Nodo *actual = $2;
            while (actual != NULL) {
                insertar_simbolo(actual->valor, $1->valor, 1, yylineno);
                actual = actual->der;
            }
        }
    ;

tipo
    : TOK_NUMERO   { $$ = crear_nodo(NODO_ID, "numero", yylineno); }
    | TOK_DECIMAL  { $$ = crear_nodo(NODO_ID, "decimal", yylineno); }
    | TOK_TEXTO    { $$ = crear_nodo(NODO_ID, "texto", yylineno); }
    ;

lista_ids
    : TOK_ID
        {
            $$ = crear_nodo(NODO_ID, $1, yylineno);
        }
    | lista_ids TOK_COMA TOK_ID
        {
            $$ = $1;
            Nodo *nuevo = crear_nodo(NODO_ID, $3, yylineno);
            
            Nodo *ultimo = $$;
            while (ultimo->der != NULL) ultimo = ultimo->der;
            ultimo->der = nuevo;
        }
    ;

lista_ids_init
    : TOK_ID TOK_ASIGNACION expresion
        {
            $$ = crear_nodo(NODO_ID, $1, yylineno);
            agregar_hijo($$, $3);
        }
    | lista_ids_init TOK_COMA TOK_ID TOK_ASIGNACION expresion
        {
            $$ = $1;
            Nodo *nuevo = crear_nodo(NODO_ID, $3, yylineno);
            agregar_hijo(nuevo, $5);
            
            Nodo *ultimo = $$;
            while (ultimo->der != NULL) ultimo = ultimo->der;
            ultimo->der = nuevo;
        }
    ;

asignacion
    : TOK_AVANZAR TOK_ID TOK_ASIGNACION expresion TOK_PUNTOCOMA
        {
            $$ = crear_nodo(NODO_ASIGNACION, $2, yylineno);
            agregar_hijo($$, $4);
            
            Simbolo *s = buscar_simbolo($2);
            if (s != NULL) s->inicializada = 1;
        }
    ;

entrada
    : TOK_LEER lista_ids TOK_PUNTOCOMA
        {
            $$ = crear_nodo(NODO_ENTRADA, NULL, yylineno);
            agregar_hijo($$, $2);
        }
    ;

salida
    : TOK_CAMINAR expresion TOK_PUNTOCOMA
        {
            $$ = crear_nodo(NODO_SALIDA, NULL, yylineno);
            agregar_hijo($$, $2);
        }
    ;

condicional
    : TOK_VIRAR TOK_PARENIZQ expresion TOK_PARENDER bloque %prec TOK_VIRAR_SIN_SINO
        {
            $$ = crear_nodo(NODO_CONDICIONAL, NULL, yylineno);
            agregar_hijo($$, $3);
            agregar_hijo($$, $5);
        }
    | TOK_VIRAR TOK_PARENIZQ expresion TOK_PARENDER bloque TOK_SINO bloque
        {
            $$ = crear_nodo(NODO_CONDICIONAL, "con_sino", yylineno);
            agregar_hijo($$, $3);
            agregar_hijo($$, $5);
            agregar_hijo($$, $7);
        }
    ;

ciclo
    : TOK_HISTORIA TOK_PARENIZQ expresion TOK_PARENDER bloque
        {
            $$ = crear_nodo(NODO_CICLO, NULL, yylineno);
            agregar_hijo($$, $3);
            agregar_hijo($$, $5);
        }
    ;

bloque
    : TOK_INICIO lista_instrucciones TOK_FINAL
        {
            $$ = $2;
        }
    ;

expresion
    : expresion TOK_SUMA expresion
        {
            $$ = crear_nodo(NODO_BINARIA, "+", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | expresion TOK_RESTA expresion
        {
            $$ = crear_nodo(NODO_BINARIA, "-", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | expresion TOK_MULTIPLICACION expresion
        {
            $$ = crear_nodo(NODO_BINARIA, "*", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | expresion TOK_DIVISION expresion
        {
            $$ = crear_nodo(NODO_BINARIA, "/", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | expresion TOK_IGUAL expresion
        {
            $$ = crear_nodo(NODO_BINARIA, "==", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | expresion TOK_DIFERENTE expresion
        {
            $$ = crear_nodo(NODO_BINARIA, "!=", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | expresion TOK_MENOR expresion
        {
            $$ = crear_nodo(NODO_BINARIA, "<", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | expresion TOK_MAYOR expresion
        {
            $$ = crear_nodo(NODO_BINARIA, ">", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | expresion TOK_MENORIGUAL expresion
        {
            $$ = crear_nodo(NODO_BINARIA, "<=", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | expresion TOK_MAYORIGUAL expresion
        {
            $$ = crear_nodo(NODO_BINARIA, ">=", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | expresion TOK_AND expresion
        {
            $$ = crear_nodo(NODO_BINARIA, "&&", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | expresion TOK_OR expresion
        {
            $$ = crear_nodo(NODO_BINARIA, "||", yylineno);
            $$->izq = $1; $$->der = $3;
        }
    | TOK_NOT expresion
        {
            $$ = crear_nodo(NODO_UNARIA, "!", yylineno);
            $$->izq = $2;
        }
    | TOK_PARENIZQ expresion TOK_PARENDER
        {
            $$ = $2;
        }
    | TOK_ID
        {
            $$ = crear_nodo(NODO_ID, $1, yylineno);
        }
    | TOK_NUM_ENTERO
        {
            char buf[32];
            sprintf(buf, "%d", $1);
            $$ = crear_nodo(NODO_NUM_ENTERO, buf, yylineno);
        }
    | TOK_NUM_DECIMAL
        {
            char buf[32];
            sprintf(buf, "%.2f", $1);
            $$ = crear_nodo(NODO_NUM_DECIMAL, buf, yylineno);
        }
    | TOK_CADENA
        {
            $$ = crear_nodo(NODO_CADENA, $1, yylineno);
        }
    | TOK_VERDADERO
        {
            $$ = crear_nodo(NODO_BOOL, "verdadero", yylineno);
        }
    | TOK_FALSO
        {
            $$ = crear_nodo(NODO_BOOL, "falso", yylineno);
        }
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
        printf("\n========== ARBOL DE SINTAXIS ABSTRACTA ==========\n");
        imprimir_ast(raiz, 0);
        imprimir_tabla_simbolos();
    } else {
        printf("\n=== Analisis completado con %d errores ===\n", errores);
    }
    
    return 0;
}