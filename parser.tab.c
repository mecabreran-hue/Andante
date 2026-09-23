/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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


#line 239 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOK_NUM_ENTERO = 3,             /* TOK_NUM_ENTERO  */
  YYSYMBOL_TOK_NUM_DECIMAL = 4,            /* TOK_NUM_DECIMAL  */
  YYSYMBOL_TOK_ID = 5,                     /* TOK_ID  */
  YYSYMBOL_TOK_CADENA = 6,                 /* TOK_CADENA  */
  YYSYMBOL_TOK_CAMINAR = 7,                /* TOK_CAMINAR  */
  YYSYMBOL_TOK_LEER = 8,                   /* TOK_LEER  */
  YYSYMBOL_TOK_AVANZAR = 9,                /* TOK_AVANZAR  */
  YYSYMBOL_TOK_VIRAR = 10,                 /* TOK_VIRAR  */
  YYSYMBOL_TOK_SINO = 11,                  /* TOK_SINO  */
  YYSYMBOL_TOK_HISTORIA = 12,              /* TOK_HISTORIA  */
  YYSYMBOL_TOK_INICIO = 13,                /* TOK_INICIO  */
  YYSYMBOL_TOK_FINAL = 14,                 /* TOK_FINAL  */
  YYSYMBOL_TOK_DETENER = 15,               /* TOK_DETENER  */
  YYSYMBOL_TOK_NUMERO = 16,                /* TOK_NUMERO  */
  YYSYMBOL_TOK_DECIMAL = 17,               /* TOK_DECIMAL  */
  YYSYMBOL_TOK_TEXTO = 18,                 /* TOK_TEXTO  */
  YYSYMBOL_TOK_VERDADERO = 19,             /* TOK_VERDADERO  */
  YYSYMBOL_TOK_FALSO = 20,                 /* TOK_FALSO  */
  YYSYMBOL_TOK_SUMA = 21,                  /* TOK_SUMA  */
  YYSYMBOL_TOK_RESTA = 22,                 /* TOK_RESTA  */
  YYSYMBOL_TOK_MULTIPLICACION = 23,        /* TOK_MULTIPLICACION  */
  YYSYMBOL_TOK_DIVISION = 24,              /* TOK_DIVISION  */
  YYSYMBOL_TOK_IGUAL = 25,                 /* TOK_IGUAL  */
  YYSYMBOL_TOK_DIFERENTE = 26,             /* TOK_DIFERENTE  */
  YYSYMBOL_TOK_MENOR = 27,                 /* TOK_MENOR  */
  YYSYMBOL_TOK_MAYOR = 28,                 /* TOK_MAYOR  */
  YYSYMBOL_TOK_MENORIGUAL = 29,            /* TOK_MENORIGUAL  */
  YYSYMBOL_TOK_MAYORIGUAL = 30,            /* TOK_MAYORIGUAL  */
  YYSYMBOL_TOK_AND = 31,                   /* TOK_AND  */
  YYSYMBOL_TOK_OR = 32,                    /* TOK_OR  */
  YYSYMBOL_TOK_NOT = 33,                   /* TOK_NOT  */
  YYSYMBOL_TOK_ASIGNACION = 34,            /* TOK_ASIGNACION  */
  YYSYMBOL_TOK_PUNTOCOMA = 35,             /* TOK_PUNTOCOMA  */
  YYSYMBOL_TOK_COMA = 36,                  /* TOK_COMA  */
  YYSYMBOL_TOK_PARENIZQ = 37,              /* TOK_PARENIZQ  */
  YYSYMBOL_TOK_PARENDER = 38,              /* TOK_PARENDER  */
  YYSYMBOL_TOK_LLAVEIZQ = 39,              /* TOK_LLAVEIZQ  */
  YYSYMBOL_TOK_LLAVEDER = 40,              /* TOK_LLAVEDER  */
  YYSYMBOL_TOK_CORCHETEIZQ = 41,           /* TOK_CORCHETEIZQ  */
  YYSYMBOL_TOK_CORCHETEDER = 42,           /* TOK_CORCHETEDER  */
  YYSYMBOL_TOK_VIRAR_SIN_SINO = 43,        /* TOK_VIRAR_SIN_SINO  */
  YYSYMBOL_YYACCEPT = 44,                  /* $accept  */
  YYSYMBOL_programa = 45,                  /* programa  */
  YYSYMBOL_lista_instrucciones = 46,       /* lista_instrucciones  */
  YYSYMBOL_instruccion = 47,               /* instruccion  */
  YYSYMBOL_declaracion = 48,               /* declaracion  */
  YYSYMBOL_tipo = 49,                      /* tipo  */
  YYSYMBOL_lista_ids = 50,                 /* lista_ids  */
  YYSYMBOL_lista_ids_init = 51,            /* lista_ids_init  */
  YYSYMBOL_asignacion = 52,                /* asignacion  */
  YYSYMBOL_entrada = 53,                   /* entrada  */
  YYSYMBOL_salida = 54,                    /* salida  */
  YYSYMBOL_condicional = 55,               /* condicional  */
  YYSYMBOL_ciclo = 56,                     /* ciclo  */
  YYSYMBOL_bloque = 57,                    /* bloque  */
  YYSYMBOL_expresion = 58                  /* expresion  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   198

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  48
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  93

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   298


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   218,   218,   228,   229,   241,   242,   243,   244,   245,
     246,   247,   248,   252,   264,   278,   279,   280,   284,   288,
     300,   305,   318,   329,   337,   345,   351,   361,   370,   377,
     382,   387,   392,   397,   402,   407,   412,   417,   422,   427,
     432,   437,   442,   446,   450,   456,   462,   466,   470
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOK_NUM_ENTERO",
  "TOK_NUM_DECIMAL", "TOK_ID", "TOK_CADENA", "TOK_CAMINAR", "TOK_LEER",
  "TOK_AVANZAR", "TOK_VIRAR", "TOK_SINO", "TOK_HISTORIA", "TOK_INICIO",
  "TOK_FINAL", "TOK_DETENER", "TOK_NUMERO", "TOK_DECIMAL", "TOK_TEXTO",
  "TOK_VERDADERO", "TOK_FALSO", "TOK_SUMA", "TOK_RESTA",
  "TOK_MULTIPLICACION", "TOK_DIVISION", "TOK_IGUAL", "TOK_DIFERENTE",
  "TOK_MENOR", "TOK_MAYOR", "TOK_MENORIGUAL", "TOK_MAYORIGUAL", "TOK_AND",
  "TOK_OR", "TOK_NOT", "TOK_ASIGNACION", "TOK_PUNTOCOMA", "TOK_COMA",
  "TOK_PARENIZQ", "TOK_PARENDER", "TOK_LLAVEIZQ", "TOK_LLAVEDER",
  "TOK_CORCHETEIZQ", "TOK_CORCHETEDER", "TOK_VIRAR_SIN_SINO", "$accept",
  "programa", "lista_instrucciones", "instruccion", "declaracion", "tipo",
  "lista_ids", "lista_ids_init", "asignacion", "entrada", "salida",
  "condicional", "ciclo", "bloque", "expresion", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-49)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -49,    28,    38,   -49,    -3,     7,    36,    44,     6,    20,
     -49,   -49,   -49,   -49,   -49,   -49,    53,   -49,   -49,   -49,
     -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,
       7,     7,   105,   -49,   -27,    40,     7,     7,   107,    41,
      -5,     1,   -49,    39,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,   -49,   -49,    67,     7,
      57,    75,   -49,     7,   -49,   -49,    68,   -49,    29,    29,
     -49,   -49,   168,   168,   -19,   -19,   -19,   -19,   158,   147,
     -49,   120,    63,    63,   135,    56,   -49,    80,   -49,     7,
      63,   135,   -49
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     1,     0,     0,     0,     0,     0,     0,
       3,    15,    16,    17,     4,     5,     0,     6,     7,     8,
       9,    10,    11,    12,    44,    45,    43,    46,    47,    48,
       0,     0,     0,    18,     0,     0,     0,     0,     0,    18,
       0,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,    23,     0,     0,
       0,     0,    28,     0,    13,    14,     0,    42,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      19,     0,     0,     0,    20,     0,    22,    25,    27,     0,
       0,    21,    26
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -49,   -49,    82,   -49,   -49,   -49,    77,   -49,   -49,   -49,
     -49,   -49,   -49,   -48,   -30
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,    14,    15,    16,    34,    41,    17,    18,
      19,    20,    21,    22,    32
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      42,    43,    44,    45,    46,    47,    60,    61,    57,    58,
      24,    25,    26,    27,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    28,    29,     3,    81,
      64,    58,    23,    84,    87,    88,    65,    66,    -2,     4,
      30,    33,    92,    36,    31,     5,     6,     7,     8,    35,
       9,    10,    46,    47,    11,    12,    13,    37,    39,    91,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    80,    85,    59,    63,    10,    67,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      89,    90,    38,    40,     0,    82,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,     4,     0,
       0,     0,     0,    83,     5,     6,     7,     8,     0,     9,
      10,    62,     0,    11,    12,    13,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,     0,     0,
      56,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,     0,     0,    86,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    44,
      45,    46,    47,     0,     0,    50,    51,    52,    53
};

static const yytype_int8 yycheck[] =
{
      30,    31,    21,    22,    23,    24,    36,    37,    35,    36,
       3,     4,     5,     6,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    19,    20,     0,    59,
      35,    36,    35,    63,    82,    83,    35,    36,     0,     1,
      33,     5,    90,    37,    37,     7,     8,     9,    10,     5,
      12,    13,    23,    24,    16,    17,    18,    37,     5,    89,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,     5,     5,    34,    34,    13,    38,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      34,    11,    10,    16,    -1,    38,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,     1,    -1,
      -1,    -1,    -1,    38,     7,     8,     9,    10,    -1,    12,
      13,    14,    -1,    16,    17,    18,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      35,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    35,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    21,
      22,    23,    24,    -1,    -1,    27,    28,    29,    30
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    45,    46,     0,     1,     7,     8,     9,    10,    12,
      13,    16,    17,    18,    47,    48,    49,    52,    53,    54,
      55,    56,    57,    35,     3,     4,     5,     6,    19,    20,
      33,    37,    58,     5,    50,     5,    37,    37,    46,     5,
      50,    51,    58,    58,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    35,    35,    36,    34,
      58,    58,    14,    34,    35,    35,    36,    38,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
       5,    58,    38,    38,    58,     5,    35,    57,    57,    34,
      11,    58,    57
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    44,    45,    46,    46,    47,    47,    47,    47,    47,
      47,    47,    47,    48,    48,    49,    49,    49,    50,    50,
      51,    51,    52,    53,    54,    55,    55,    56,    57,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     3,     3,     1,     1,     1,     1,     3,
       3,     5,     5,     3,     3,     5,     7,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     3,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* programa: lista_instrucciones  */
#line 219 "parser.y"
        {
            raiz = crear_nodo(NODO_PROGRAMA, NULL, 0);
            agregar_hijo(raiz, (yyvsp[0].nodo));
            (yyval.nodo) = raiz;
        }
#line 1368 "parser.tab.c"
    break;

  case 3: /* lista_instrucciones: %empty  */
#line 228 "parser.y"
        { (yyval.nodo) = NULL; }
#line 1374 "parser.tab.c"
    break;

  case 4: /* lista_instrucciones: lista_instrucciones instruccion  */
#line 230 "parser.y"
        {
            if ((yyvsp[-1].nodo) == NULL) {
                (yyval.nodo) = crear_nodo(NODO_BLOQUE, NULL, 0);
            } else {
                (yyval.nodo) = (yyvsp[-1].nodo);
            }
            if ((yyvsp[0].nodo) != NULL) agregar_hijo((yyval.nodo), (yyvsp[0].nodo));
        }
#line 1387 "parser.tab.c"
    break;

  case 5: /* instruccion: declaracion  */
#line 241 "parser.y"
                     { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1393 "parser.tab.c"
    break;

  case 6: /* instruccion: asignacion  */
#line 242 "parser.y"
                     { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1399 "parser.tab.c"
    break;

  case 7: /* instruccion: entrada  */
#line 243 "parser.y"
                     { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1405 "parser.tab.c"
    break;

  case 8: /* instruccion: salida  */
#line 244 "parser.y"
                     { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1411 "parser.tab.c"
    break;

  case 9: /* instruccion: condicional  */
#line 245 "parser.y"
                     { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1417 "parser.tab.c"
    break;

  case 10: /* instruccion: ciclo  */
#line 246 "parser.y"
                     { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1423 "parser.tab.c"
    break;

  case 11: /* instruccion: bloque  */
#line 247 "parser.y"
                     { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1429 "parser.tab.c"
    break;

  case 12: /* instruccion: error TOK_PUNTOCOMA  */
#line 248 "parser.y"
                          { yyerrok; (yyval.nodo) = NULL; }
#line 1435 "parser.tab.c"
    break;

  case 13: /* declaracion: tipo lista_ids TOK_PUNTOCOMA  */
#line 253 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_DECLARACION, (yyvsp[-2].nodo)->valor, yylineno);
            agregar_hijo((yyval.nodo), (yyvsp[-1].nodo));
            
            /* Insertar cada ID en la tabla */
            Nodo *actual = (yyvsp[-1].nodo);
            while (actual != NULL) {
                insertar_simbolo(actual->valor, (yyvsp[-2].nodo)->valor, 0, yylineno);
                actual = actual->der;
            }
        }
#line 1451 "parser.tab.c"
    break;

  case 14: /* declaracion: tipo lista_ids_init TOK_PUNTOCOMA  */
#line 265 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_DECLARACION, (yyvsp[-2].nodo)->valor, yylineno);
            agregar_hijo((yyval.nodo), (yyvsp[-1].nodo));
            
            Nodo *actual = (yyvsp[-1].nodo);
            while (actual != NULL) {
                insertar_simbolo(actual->valor, (yyvsp[-2].nodo)->valor, 1, yylineno);
                actual = actual->der;
            }
        }
#line 1466 "parser.tab.c"
    break;

  case 15: /* tipo: TOK_NUMERO  */
#line 278 "parser.y"
                   { (yyval.nodo) = crear_nodo(NODO_ID, "numero", yylineno); }
#line 1472 "parser.tab.c"
    break;

  case 16: /* tipo: TOK_DECIMAL  */
#line 279 "parser.y"
                   { (yyval.nodo) = crear_nodo(NODO_ID, "decimal", yylineno); }
#line 1478 "parser.tab.c"
    break;

  case 17: /* tipo: TOK_TEXTO  */
#line 280 "parser.y"
                   { (yyval.nodo) = crear_nodo(NODO_ID, "texto", yylineno); }
#line 1484 "parser.tab.c"
    break;

  case 18: /* lista_ids: TOK_ID  */
#line 285 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_ID, (yyvsp[0].str), yylineno);
        }
#line 1492 "parser.tab.c"
    break;

  case 19: /* lista_ids: lista_ids TOK_COMA TOK_ID  */
#line 289 "parser.y"
        {
            (yyval.nodo) = (yyvsp[-2].nodo);
            Nodo *nuevo = crear_nodo(NODO_ID, (yyvsp[0].str), yylineno);
            
            Nodo *ultimo = (yyval.nodo);
            while (ultimo->der != NULL) ultimo = ultimo->der;
            ultimo->der = nuevo;
        }
#line 1505 "parser.tab.c"
    break;

  case 20: /* lista_ids_init: TOK_ID TOK_ASIGNACION expresion  */
#line 301 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_ID, (yyvsp[-2].str), yylineno);
            agregar_hijo((yyval.nodo), (yyvsp[0].nodo));
        }
#line 1514 "parser.tab.c"
    break;

  case 21: /* lista_ids_init: lista_ids_init TOK_COMA TOK_ID TOK_ASIGNACION expresion  */
#line 306 "parser.y"
        {
            (yyval.nodo) = (yyvsp[-4].nodo);
            Nodo *nuevo = crear_nodo(NODO_ID, (yyvsp[-2].str), yylineno);
            agregar_hijo(nuevo, (yyvsp[0].nodo));
            
            Nodo *ultimo = (yyval.nodo);
            while (ultimo->der != NULL) ultimo = ultimo->der;
            ultimo->der = nuevo;
        }
#line 1528 "parser.tab.c"
    break;

  case 22: /* asignacion: TOK_AVANZAR TOK_ID TOK_ASIGNACION expresion TOK_PUNTOCOMA  */
#line 319 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_ASIGNACION, (yyvsp[-3].str), yylineno);
            agregar_hijo((yyval.nodo), (yyvsp[-1].nodo));
            
            Simbolo *s = buscar_simbolo((yyvsp[-3].str));
            if (s != NULL) s->inicializada = 1;
        }
#line 1540 "parser.tab.c"
    break;

  case 23: /* entrada: TOK_LEER lista_ids TOK_PUNTOCOMA  */
#line 330 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_ENTRADA, NULL, yylineno);
            agregar_hijo((yyval.nodo), (yyvsp[-1].nodo));
        }
#line 1549 "parser.tab.c"
    break;

  case 24: /* salida: TOK_CAMINAR expresion TOK_PUNTOCOMA  */
#line 338 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_SALIDA, NULL, yylineno);
            agregar_hijo((yyval.nodo), (yyvsp[-1].nodo));
        }
#line 1558 "parser.tab.c"
    break;

  case 25: /* condicional: TOK_VIRAR TOK_PARENIZQ expresion TOK_PARENDER bloque  */
#line 346 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_CONDICIONAL, NULL, yylineno);
            agregar_hijo((yyval.nodo), (yyvsp[-2].nodo));
            agregar_hijo((yyval.nodo), (yyvsp[0].nodo));
        }
#line 1568 "parser.tab.c"
    break;

  case 26: /* condicional: TOK_VIRAR TOK_PARENIZQ expresion TOK_PARENDER bloque TOK_SINO bloque  */
#line 352 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_CONDICIONAL, "con_sino", yylineno);
            agregar_hijo((yyval.nodo), (yyvsp[-4].nodo));
            agregar_hijo((yyval.nodo), (yyvsp[-2].nodo));
            agregar_hijo((yyval.nodo), (yyvsp[0].nodo));
        }
#line 1579 "parser.tab.c"
    break;

  case 27: /* ciclo: TOK_HISTORIA TOK_PARENIZQ expresion TOK_PARENDER bloque  */
#line 362 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_CICLO, NULL, yylineno);
            agregar_hijo((yyval.nodo), (yyvsp[-2].nodo));
            agregar_hijo((yyval.nodo), (yyvsp[0].nodo));
        }
#line 1589 "parser.tab.c"
    break;

  case 28: /* bloque: TOK_INICIO lista_instrucciones TOK_FINAL  */
#line 371 "parser.y"
        {
            (yyval.nodo) = (yyvsp[-1].nodo);
        }
#line 1597 "parser.tab.c"
    break;

  case 29: /* expresion: expresion TOK_SUMA expresion  */
#line 378 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, "+", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1606 "parser.tab.c"
    break;

  case 30: /* expresion: expresion TOK_RESTA expresion  */
#line 383 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, "-", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1615 "parser.tab.c"
    break;

  case 31: /* expresion: expresion TOK_MULTIPLICACION expresion  */
#line 388 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, "*", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1624 "parser.tab.c"
    break;

  case 32: /* expresion: expresion TOK_DIVISION expresion  */
#line 393 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, "/", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1633 "parser.tab.c"
    break;

  case 33: /* expresion: expresion TOK_IGUAL expresion  */
#line 398 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, "==", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1642 "parser.tab.c"
    break;

  case 34: /* expresion: expresion TOK_DIFERENTE expresion  */
#line 403 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, "!=", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1651 "parser.tab.c"
    break;

  case 35: /* expresion: expresion TOK_MENOR expresion  */
#line 408 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, "<", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1660 "parser.tab.c"
    break;

  case 36: /* expresion: expresion TOK_MAYOR expresion  */
#line 413 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, ">", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1669 "parser.tab.c"
    break;

  case 37: /* expresion: expresion TOK_MENORIGUAL expresion  */
#line 418 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, "<=", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1678 "parser.tab.c"
    break;

  case 38: /* expresion: expresion TOK_MAYORIGUAL expresion  */
#line 423 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, ">=", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1687 "parser.tab.c"
    break;

  case 39: /* expresion: expresion TOK_AND expresion  */
#line 428 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, "&&", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1696 "parser.tab.c"
    break;

  case 40: /* expresion: expresion TOK_OR expresion  */
#line 433 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BINARIA, "||", yylineno);
            (yyval.nodo)->izq = (yyvsp[-2].nodo); (yyval.nodo)->der = (yyvsp[0].nodo);
        }
#line 1705 "parser.tab.c"
    break;

  case 41: /* expresion: TOK_NOT expresion  */
#line 438 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_UNARIA, "!", yylineno);
            (yyval.nodo)->izq = (yyvsp[0].nodo);
        }
#line 1714 "parser.tab.c"
    break;

  case 42: /* expresion: TOK_PARENIZQ expresion TOK_PARENDER  */
#line 443 "parser.y"
        {
            (yyval.nodo) = (yyvsp[-1].nodo);
        }
#line 1722 "parser.tab.c"
    break;

  case 43: /* expresion: TOK_ID  */
#line 447 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_ID, (yyvsp[0].str), yylineno);
        }
#line 1730 "parser.tab.c"
    break;

  case 44: /* expresion: TOK_NUM_ENTERO  */
#line 451 "parser.y"
        {
            char buf[32];
            sprintf(buf, "%d", (yyvsp[0].num));
            (yyval.nodo) = crear_nodo(NODO_NUM_ENTERO, buf, yylineno);
        }
#line 1740 "parser.tab.c"
    break;

  case 45: /* expresion: TOK_NUM_DECIMAL  */
#line 457 "parser.y"
        {
            char buf[32];
            sprintf(buf, "%.2f", (yyvsp[0].dec));
            (yyval.nodo) = crear_nodo(NODO_NUM_DECIMAL, buf, yylineno);
        }
#line 1750 "parser.tab.c"
    break;

  case 46: /* expresion: TOK_CADENA  */
#line 463 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_CADENA, (yyvsp[0].str), yylineno);
        }
#line 1758 "parser.tab.c"
    break;

  case 47: /* expresion: TOK_VERDADERO  */
#line 467 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BOOL, "verdadero", yylineno);
        }
#line 1766 "parser.tab.c"
    break;

  case 48: /* expresion: TOK_FALSO  */
#line 471 "parser.y"
        {
            (yyval.nodo) = crear_nodo(NODO_BOOL, "falso", yylineno);
        }
#line 1774 "parser.tab.c"
    break;


#line 1778 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 476 "parser.y"


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
