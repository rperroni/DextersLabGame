#ifndef __ESCAPE_LABORATORIO_H__
#define __ESCAPE_LABORATORIO_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_OBSTACULOS 25
#define MAX_HERRAMIENTAS 25
#define MAX_NIVELES 4
#define MAX_PAREDES 250

typedef struct coordenada {
	int fil;
	int col;
} coordenada_t;

typedef struct personaje {
	char tipo;
	int movimientos;
	coordenada_t posicion;
	bool tiene_llave;
	bool presiono_interruptor;
	bool murio;
} personaje_t;

typedef struct elemento {
	char tipo;
	coordenada_t posicion;
} elemento_t;

typedef struct nivel {
	coordenada_t entrada;
	coordenada_t salida;

	coordenada_t paredes[MAX_PAREDES];
	int tope_paredes;

	elemento_t obstaculos[MAX_OBSTACULOS];
	int tope_obstaculos;

	elemento_t herramientas[MAX_HERRAMIENTAS];
	int tope_herramientas;
} nivel_t;

typedef struct juego {
	nivel_t niveles[MAX_NIVELES];
	int nivel_actual;
	personaje_t personaje;
} juego_t;


/*
 * Inicializará el juego, cargando la informacion de los cuatro niveles
 * y los datos del personaje.
 */
void inicializar_juego(juego_t* juego, char tipo_personaje);

/*
 * Recibe un juego con todas sus estructuras válidas.
 * El juego se dará por ganado si el personaje está en el último nivel
 * y posicionado en la salida.
 * El juego se dará por perdido, si el personaje queda sin movimientos.
 * Devolverá:
 * ->  0 si el estado es jugando.
 * -> -1 si el estado es perdido.
 * ->  1 si el estado es ganado.
 */
int estado_juego(juego_t juego);

/*
 * Recibe el personaje con todas sus estructuras válidas, y la coordenada de la salida del nivel en ejecución.
 * El nivel se dará por ganado cuando el personaje se
 * posicione en la salida habiendo obtenido previamente la llave.
 * Devolverá:
 * ->  0 si el estado es jugando.
 * ->  1 si el estado es ganado.
 */
int estado_nivel(personaje_t personaje, coordenada_t salida);

/*
 * Inicializara un nivel cargando su entrada,
 * salida, obtáculos, herramientas y paredes.
 */
void inicializar_nivel(nivel_t* nivel, int numero_nivel, int cantidad_baldosas_pinches, int cantidad_guardia, bool hay_bomba);


/*
 * Mueve el personaje en la dirección indicada por el usuario
 * y actualiza el juego según los elementos que haya en el camino
 * del personaje.
 * El juego quedará en un estado válido al terminar el movimiento.
 * El movimiento será:
 * -> w: Si el personaje debe moverse para la arriba.
 * -> a: Si el personaje debe moverse para la derecha.
 * -> s: Si el personaje debe moverse para la abajo.
 * -> d: Si el personaje debe moverse para la izquierda.
 * En caso de que en la dirección que se quiere mover haya una pared
 * se contará como un movimiento, los guardias se moverán y
 * el personaje quedará en la misma baldosa.
 */
void mover_personaje(juego_t* juego, char movimiento);

/*
 * Mostrará el juego por pantalla.
 * Se recomienda mostrar todo aquello que le sea de
 * utilidad al jugador, como los movimientos restantes,
 * el nivel, los obstaculos posicionados, las paredes, etc.
 */
void mostrar_juego(juego_t juego);

#endif /* __ESCAPE_LABORATORIO_H__ */
