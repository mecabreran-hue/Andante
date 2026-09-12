%{
/* ======================================================================
   ANDANTE (.and) - Analizador Lexico
   [Nombre Universidad]
   Integrantes: [Tu nombre], [Nombre companera]
   ====================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

int linea = 1;
%}

%option noyywrap yylineno

DIGITO          [0-9]
LETRA           [a-zA-Z_]
ID              {LETRA}({LETRA}|{DIGITO})*
NUM_ENTERO      {DIGITO}+
NUM_DECIMAL     {DIGITO}+"."{DIGITO}+
CADENA          \"([^\"\n])*\"
ESPACIO         [ \t]+

%x COMENTARIO_BLOQUE

%%

"//".*                  { /* Ignorar comentario de linea */ }

"/*"                    { BEGIN(COMENTARIO_BLOQUE); }
<COMENTARIO_BLOQUE>"*/" { BEGIN(INITIAL); }
<COMENTARIO_BLOQUE>\n   { linea++; }
<COMENTARIO_BLOQUE>.    { /* Ignorar */ }

"caminar"   { return TOK_CAMINAR; }
"leer"      { return TOK_LEER; }
"avanzar"   { return TOK_AVANZAR; }
"virar"     { return TOK_VIRAR; }
"sino"      { return TOK_SINO; }
"historia"  { return TOK_HISTORIA; }
"inicio"    { return TOK_INICIO; }
"final"     { return TOK_FINAL; }
"detener"   { return TOK_DETENER; }
"numero"    { return TOK_NUMERO; }
"decimal"   { return TOK_DECIMAL; }
"texto"     { return TOK_TEXTO; }
"verdadero" { return TOK_VERDADERO; }
"falso"     { return TOK_FALSO; }

{ID}        { yylval.str = strdup(yytext); return TOK_ID; }

{NUM_ENTERO}    { yylval.num = atoi(yytext); return TOK_NUM_ENTERO; }
{NUM_DECIMAL}   { yylval.dec = atof(yytext); return TOK_NUM_DECIMAL; }
{CADENA}        { yylval.str = strdup(yytext); return TOK_CADENA; }

"+"         { return TOK_SUMA; }
"-"         { return TOK_RESTA; }
"*"         { return TOK_MULTIPLICACION; }
"/"         { return TOK_DIVISION; }

"=="        { return TOK_IGUAL; }
"!="        { return TOK_DIFERENTE; }
"<="        { return TOK_MENORIGUAL; }
">="        { return TOK_MAYORIGUAL; }
"<"         { return TOK_MENOR; }
">"         { return TOK_MAYOR; }

"&&"        { return TOK_AND; }
"||"        { return TOK_OR; }
"!"         { return TOK_NOT; }

"="         { return TOK_ASIGNACION; }

";"         { return TOK_PUNTOCOMA; }
","         { return TOK_COMA; }
"("         { return TOK_PARENIZQ; }
")"         { return TOK_PARENDER; }
"{"         { return TOK_LLAVEIZQ; }
"}"         { return TOK_LLAVEDER; }
"["         { return TOK_CORCHETEIZQ; }
"]"         { return TOK_CORCHETEDER; }

{ESPACIO}   { /* Ignorar espacios */ }
\n          { linea++; }

. {
    printf("Error Lexico en linea %d: Caracter no reconocido '%s'\n",
           linea, yytext);
}

%%