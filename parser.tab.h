/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOK_NUM_ENTERO = 258,          /* TOK_NUM_ENTERO  */
    TOK_NUM_DECIMAL = 259,         /* TOK_NUM_DECIMAL  */
    TOK_ID = 260,                  /* TOK_ID  */
    TOK_CADENA = 261,              /* TOK_CADENA  */
    TOK_CAMINAR = 262,             /* TOK_CAMINAR  */
    TOK_LEER = 263,                /* TOK_LEER  */
    TOK_AVANZAR = 264,             /* TOK_AVANZAR  */
    TOK_VIRAR = 265,               /* TOK_VIRAR  */
    TOK_SINO = 266,                /* TOK_SINO  */
    TOK_HISTORIA = 267,            /* TOK_HISTORIA  */
    TOK_INICIO = 268,              /* TOK_INICIO  */
    TOK_FINAL = 269,               /* TOK_FINAL  */
    TOK_DETENER = 270,             /* TOK_DETENER  */
    TOK_NUMERO = 271,              /* TOK_NUMERO  */
    TOK_DECIMAL = 272,             /* TOK_DECIMAL  */
    TOK_TEXTO = 273,               /* TOK_TEXTO  */
    TOK_VERDADERO = 274,           /* TOK_VERDADERO  */
    TOK_FALSO = 275,               /* TOK_FALSO  */
    TOK_SUMA = 276,                /* TOK_SUMA  */
    TOK_RESTA = 277,               /* TOK_RESTA  */
    TOK_MULTIPLICACION = 278,      /* TOK_MULTIPLICACION  */
    TOK_DIVISION = 279,            /* TOK_DIVISION  */
    TOK_IGUAL = 280,               /* TOK_IGUAL  */
    TOK_DIFERENTE = 281,           /* TOK_DIFERENTE  */
    TOK_MENOR = 282,               /* TOK_MENOR  */
    TOK_MAYOR = 283,               /* TOK_MAYOR  */
    TOK_MENORIGUAL = 284,          /* TOK_MENORIGUAL  */
    TOK_MAYORIGUAL = 285,          /* TOK_MAYORIGUAL  */
    TOK_AND = 286,                 /* TOK_AND  */
    TOK_OR = 287,                  /* TOK_OR  */
    TOK_NOT = 288,                 /* TOK_NOT  */
    TOK_ASIGNACION = 289,          /* TOK_ASIGNACION  */
    TOK_PUNTOCOMA = 290,           /* TOK_PUNTOCOMA  */
    TOK_COMA = 291,                /* TOK_COMA  */
    TOK_PARENIZQ = 292,            /* TOK_PARENIZQ  */
    TOK_PARENDER = 293,            /* TOK_PARENDER  */
    TOK_LLAVEIZQ = 294,            /* TOK_LLAVEIZQ  */
    TOK_LLAVEDER = 295,            /* TOK_LLAVEDER  */
    TOK_CORCHETEIZQ = 296,         /* TOK_CORCHETEIZQ  */
    TOK_CORCHETEDER = 297,         /* TOK_CORCHETEDER  */
    TOK_VIRAR_SIN_SINO = 298       /* TOK_VIRAR_SIN_SINO  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 169 "parser.y"

    int num;
    double dec;
    char *str;
    struct Nodo *nodo;

#line 114 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
