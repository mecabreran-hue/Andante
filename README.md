Andante (.and)
Lenguaje de programación educativo desarrollado para el curso de Compiladores.

Descripción
Andante es un lenguaje cuyo nombre proviene del término musical italiano que significa "caminando". Cada programa es un viaje, cada variable es un destino, y cada instrucción es un paso en el camino.

La inspiración nació de combinar dos pasiones: los libros y los carros. El lenguaje está diseñado para que el código se lea como una narrativa, con palabras clave en español que evocan movimiento.

Integrantes

Moisés Emanuel Cabrera Noriega - 2400019

Lourdes Mercedes Alvarado Rodriguez - 2400438

Extensión

Los archivos de Andante usan la extensión .and.

Palabras reservadas

Palabra	Función

caminar	Mostrar en pantalla

leer	Leer entrada del usuario

avanzar	Asignar valor a variable

virar	Condicional

sino	Alternativa

historia	Ciclo

inicio	Abrir bloque

final	Cerrar bloque

detener	Romper ciclo

numero	Tipo entero

decimal	Tipo flotante

texto	Tipo cadena

verdadero / falso	Valores booleanos

Ejemplo de programa

// Viaje al conocimiento

caminar "Bienvenido a Andante";

numero paginas = 100;
numero leidas = 0;
texto libro = "El Principito";

leer paginas;

historia (leidas < paginas) inicio
    avanzar leidas = leidas + 10;
    caminar "Leidas " + leidas + " paginas";
    
    virar (leidas == 50) inicio
        caminar "Mitad del libro";
    final
final

caminar "Termine de leer " + libro;

Compilación

Requiere Flex, Bison y GCC.
bison -d parser.y
flex lexer.l
gcc parser.tab.c lex.yy.c -o andante

Ejecución

./andante archivo.and

Ejemplo:

./andante entrada_valida_1.and

Estructura del proyecto

Andante Proyecto/
├── lexer.l              Analizador léxico (Flex)

├── parser.y             Analizador sintáctico (Bison)

├── README.md            Este archivo

├── .gitignore           Archivos ignorados por Git

├── .gitattributes       Configuración de saltos de línea

├── entrada_valida_1.and Ejemplo válido 1

├── valido_2.and         Ejemplo válido 2

├── valido_3.and         Ejemplo válido 3

├── valido_4.and         Ejemplo válido 4

├── valido_5.and         Ejemplo válido 5

├── valido_6.and         Ejemplo válido 6

├── entrada_errores_1.and Ejemplo inválido 1

├── invalido_2.and       Ejemplo inválido 2

├── invalido_3.and       Ejemplo inválido 3

├── invalido_4.and       Ejemplo inválido 4

├── invalido_5.and       Ejemplo inválido 5

└── invalido_6.and       Ejemplo inválido 6


Capacidades cubiertas

Declaración de variables

Asignación de valores

Expresiones aritméticas

Precedencia de operaciones

Comparaciones

Entrada de datos

Salida de datos

Condicional con alternativa

Ciclo while

Comentarios de una y varias líneas

Estado del proyecto

Analizador léxico funcional con Flex

Analizador sintáctico funcional con Bison

Integración Flex + Bison

AST inicial (árbol de sintaxis abstracta)

Tabla de símbolos básica

6 ejemplos válidos y 6 inválidos

Detección de errores con línea exacta

Próximos pasos

Validaciones semánticas

Representación intermedia


Repositorio

https://github.com/mecabreran-hue/Andante
