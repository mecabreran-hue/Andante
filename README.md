# Andante (.and)
Lenguaje de programacion educativo.

## Compilacion

    bison -d parser.y
    flex lexer.l
    gcc parser.tab.c lex.yy.c -o andante

## Ejecucion

    ./andante archivo.and
