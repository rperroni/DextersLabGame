#include "escape_laboratorio.h"
#include "detector_personajes.h"
#include "utiles.h"
#include <time.h>
#include <stdlib.h>

#define MAX_COL_NIVEL 17
#define MAX_FIL_NIVEL 17

#define TOPE_NIVEL_1_3 12
#define TOPE_NIVEL_2_4 17

#define ARRIBA 'w'
#define ABAJO 's'
#define DERECHA 'd'
#define IZQUIERDA 'a'

#define JUEGO_PERDIDO -1
#define JUEGO_GANADO 1
#define JUEGO_JUGANDO 0

#define NIVEL_GANADO 1
#define NIVEL_JUGANDO 0

#define PARED '.'
#define ENTRADA 'E'
#define SALIDA 'S'
#define PERSONAJE 'X'
#define POS_VACIA ' '

/*	OBSTÁCULOS	*/
#define B_PINCHE 'P'
#define BOMBA 'B'
#define GUARDIA 'G'

/*	HERRAMIENTAS	*/
#define INTERRUPTOR 'I'
#define B_TELETRANSP 'T'
#define MONEDA 'M'
#define LLAVE 'L'

#define PINCHES_NIVEL_1 4
#define GUARDIAS_NIVEL_1 2

#define PINCHES_NIVEL_2 6
#define GUARDIAS_NIVEL_2 3

#define PINCHES_NIVEL_3 6
#define GUARDIAS_NIVEL_3 3

#define PINCHES_NIVEL_4 10
#define GUARDIAS_NIVEL_4 4

#define NIVEL_INICIAL 1
#define MOVIMIENTOS_INICIAL 10

#define MOV_PERDIDOS_POR_PINCHE 2
#define MOV_GANADOS_POR_MONEDA 1

#define MAX_ITERACIONES 1000

/*	PERSONAJES 	*/
#define P_JOHNNY_BRAVO 'J'
#define P_BELLOTA 'S'
#define P_POLLITO 'P'
#define P_BLUE 'B'
#define P_PURO_HUESO 'H'
#define P_CORAJE 'C'

/*	 COLORES 	 */
#define FIN 			"\033[0m"
#define NEGRO 			"\033[0m\033[30m"
#define GRIS 			"\033[1m\033[30m"
#define ROJO_OSCURO 	"\033[0m\033[31m"
#define ROJO_CLARO 		"\033[1m\033[31m"
#define VERDE_OSCURO 	"\033[0m\033[32m"
#define VERDE_CLARO 	"\033[1m\033[32m"
#define AMARILLO	 	"\033[1m\033[33m"
#define MARRON		 	"\033[0m\033[33m"
#define AZUL	 		"\033[1m\033[34m"
#define AZUL_SUAVE	 	"\033[0m\033[34m"
#define MAGENTA_SUAVE 	"\033[0m\033[35m"
#define MAGENTA 		"\033[1m\033[35m"
#define CELESTE_SUAVE 	"\033[0m\033[36m"
#define CELESTE 		"\033[1m\033[36m"
#define BLANCO		 	"\033[1m\033[37m"


void inicializar_juego(juego_t* juego, char tipo_personaje)
{
	inicializar_nivel(&((*juego).niveles[0]), 1, PINCHES_NIVEL_1, GUARDIAS_NIVEL_1, false);
	inicializar_nivel(&((*juego).niveles[1]), 2, PINCHES_NIVEL_2, GUARDIAS_NIVEL_2, false);
	inicializar_nivel(&((*juego).niveles[2]), 3, PINCHES_NIVEL_3, GUARDIAS_NIVEL_3, true);
	inicializar_nivel(&((*juego).niveles[3]), 4, PINCHES_NIVEL_4, GUARDIAS_NIVEL_4, true);
	(*juego).nivel_actual = NIVEL_INICIAL;

	(*juego).personaje.tipo = tipo_personaje;
	(*juego).personaje.movimientos = MOVIMIENTOS_INICIAL;
	(*juego).personaje.posicion = (*juego).niveles[(*juego).nivel_actual-1].entrada;
	(*juego).personaje.tiene_llave = false;
	(*juego).personaje.presiono_interruptor = false;
	(*juego).personaje.murio = false;
}

/*
 * Pre condiciones: el primer parámetro es de tipo coordenada_t para ser modificado, el segundo parámetro es el límite de la matriz correspondiente.
 *
 * Post condiciones: devuelve una coordenada (fila y columna) generada aleatoriamente dentro del límite indicado.
 */
void obtener_coordenada_aleatoria(coordenada_t *coord, int tamanio_nivel)
{
	(*coord).fil = (rand() % (tamanio_nivel-1)) + 1;
	(*coord).col = (rand() % (tamanio_nivel-1)) + 1;
}

/*
 * Pre condiciones: recibe una variable de tipo nivel_t por referencia
 *
 * Post condiciones: devuelve las coordenadas de la variable nivel_t inicializadas
 */
void inicializar_coordenadas(nivel_t *nivel)
{
	int i;
	(*nivel).entrada.fil = -1;
	(*nivel).entrada.col = -1;
	
	(*nivel).salida.fil = -1;
	(*nivel).salida.col = -1;

	for(i = 0; i < (*nivel).tope_obstaculos; i++)
	{
		(*nivel).obstaculos[i].posicion.fil = -1;
		(*nivel).obstaculos[i].posicion.col = -1;
	}

	for(i = 0; i < (*nivel).tope_herramientas; i++)
	{
		(*nivel).herramientas[i].posicion.fil = -1;
		(*nivel).herramientas[i].posicion.col = -1;
	}
}

/*
 * Pre condiciones: recibe dos coordenadas
 *
 * Post condiciones: devuelve true si son la misma coordenada
 */
bool es_misma_coordenada(coordenada_t coord_1, coordenada_t coord_2)
{
	return (coord_1.fil == coord_2.fil && coord_1.col == coord_2.col);
}

/*
 * Pre condiciones: recibe un nivel y una coordenada
 *
 * Post condiciones: devuelve true si en la coordenada hay una pared del nivel, y false si no.
 */
bool hay_pared (nivel_t nivel, coordenada_t coord)
{
	bool hay_una_pared = false;
	for(int i = 0; i < nivel.tope_paredes; i++)
	{
		if(es_misma_coordenada(nivel.paredes[i], coord))
		{
			hay_una_pared = true;
		}
	}
	return hay_una_pared;
}

/*
 * Pre condiciones: recibe un caracter que corresponde a una herramienta o a un obstaculo que se quiera encontrar, una coordenada, un vector de elemento_t de tamaño MAX_OBSTACULOS y su tope
 *
 * Post condiciones: devuelve true si en la coordenada está el elemento buscado, y false si no.
 */
bool esta_elemento_buscado(char elemento_buscado, coordenada_t coord, elemento_t elementos[MAX_OBSTACULOS], int tope_elementos)
{
	bool elemento_encontrado = false;

	for(int i = 0; i < tope_elementos; i++)
	{
		if(es_misma_coordenada(elementos[i].posicion, coord) && 
			elementos[i].tipo == elemento_buscado)
		{
			elemento_encontrado = true;
		}
	}

	return elemento_encontrado;
}

/*
 * Pre condiciones: recibe un nivel y una coordenada
 *
 * Post condiciones: devuelve true si en la coordenada no hay otro elemento en el nivel, y false si lo hay.
 */
bool es_coordenada_valida(nivel_t nivel, coordenada_t coord)
{
	bool coordenada_valida = true;

	if(hay_pared(nivel, coord))
		coordenada_valida = false;

	if(es_misma_coordenada(nivel.entrada, coord))
	{
		coordenada_valida = false;
	}

	if(es_misma_coordenada(nivel.salida, coord))
	{
		coordenada_valida = false;
	}
	
	if( esta_elemento_buscado(B_PINCHE, coord, nivel.obstaculos, nivel.tope_obstaculos) 
		|| esta_elemento_buscado(GUARDIA, coord, nivel.obstaculos, nivel.tope_obstaculos) 
		|| esta_elemento_buscado(BOMBA, coord, nivel.obstaculos, nivel.tope_obstaculos) 
		|| esta_elemento_buscado(LLAVE, coord, nivel.herramientas, nivel.tope_herramientas) 
		|| esta_elemento_buscado(INTERRUPTOR, coord, nivel.herramientas, nivel.tope_herramientas) 
		|| esta_elemento_buscado(MONEDA, coord, nivel.herramientas, nivel.tope_herramientas) 
		|| esta_elemento_buscado(B_TELETRANSP, coord, nivel.herramientas, nivel.tope_herramientas))
	{
		coordenada_valida = false;
	}

	return coordenada_valida;
}

/*
 * Pre condiciones: recibe dos variables de tipo coordenada_t
 *
 * Post condiciones: devuelve true si la 2da coordenada es contigua a la primera (misma fila o misma columna) y false si no lo es.
 */
bool es_coord_contigua(coordenada_t coord_1, coordenada_t coord_2)
{
	bool es_coord_2_contigua = false;
	if(coord_2.fil == coord_1.fil)
	{
		if(coord_2.col == (coord_1.col - 1) || coord_2.col == (coord_1.col + 1))
		{
			es_coord_2_contigua = true;
		}
	}
	else if(coord_2.col == coord_1.col)
	{
		if(coord_2.fil == (coord_1.fil - 1) || coord_2.fil == (coord_1.fil + 1))
		{
			es_coord_2_contigua = true;
		}
	}
	return es_coord_2_contigua;
}


/*
 * Pre condiciones: recibe un nivel, un entero que debe ser 12 o 17 y una coordenada
 *
 * Post condiciones: devuelve una coordenada válida
 */
void obtener_coordenada(nivel_t nivel, int tamanio_nivel, coordenada_t *coord)
{
	obtener_coordenada_aleatoria(coord, tamanio_nivel);
	while(!es_coordenada_valida(nivel, (*coord)))
		{
			obtener_coordenada_aleatoria(coord, tamanio_nivel);
		}
}

/*
 * Pre condiciones: recibe un nivel, un entero que es 12 o 17 y la cantidad de baldosas pinche del nivel
 *
 * Post condiciones: devuelve el nivel con todas las baldosas pinche colocadas
 */
void obtener_baldosas_pinche(nivel_t *nivel, int tamanio_nivel, int cantidad_baldosas_pinches)
{
	int iteraciones = 0;
	int pinches_puestos = 1;
	int i;
	coordenada_t coord;

	(*nivel).obstaculos[0].tipo = B_PINCHE;
	(*nivel).tope_obstaculos = cantidad_baldosas_pinches;

	while(pinches_puestos < cantidad_baldosas_pinches)
	{
		iteraciones = 0;
		pinches_puestos = 1;
		
		obtener_coordenada(*nivel, tamanio_nivel, &coord);

		(*nivel).obstaculos[0].posicion = coord;
		i = 1;
		while(i < cantidad_baldosas_pinches && iteraciones < MAX_ITERACIONES)
		{
			(*nivel).obstaculos[i].tipo = B_PINCHE;
			obtener_coordenada_aleatoria(&coord, tamanio_nivel);
			while( (!es_coordenada_valida(*nivel, coord) || !es_coord_contigua(coord, (*nivel).obstaculos[i-1].posicion))
					&& iteraciones < MAX_ITERACIONES)
			{
				obtener_coordenada_aleatoria(&coord, tamanio_nivel);
				iteraciones++;
			}
			if( (iteraciones < MAX_ITERACIONES) && es_coordenada_valida(*nivel, coord) && es_coord_contigua(coord, (*nivel).obstaculos[i-1].posicion)) 
			{
				(*nivel).obstaculos[i].posicion = coord;
				pinches_puestos++;
			}
			i++;
		}
	}
}

/*
 * Pre condiciones: recibe un entero
 * 
 * Post condiciones: devuelve TOPE_NIVEL_1_3 si el entero es 1 o 3. Devuelve TOPE_NIVEL_2_4 si el entero es 2 o 4
 */
int tamanio_de_nivel(int numero_nivel)
{
	int tamanio_nivel;
	if(numero_nivel == 1 || numero_nivel == 3)
	{
		tamanio_nivel = TOPE_NIVEL_1_3;
	}
	else if(numero_nivel == 2 || numero_nivel == 4)
	{
		tamanio_nivel = TOPE_NIVEL_2_4;
	}
	return tamanio_nivel;
}


void inicializar_nivel(nivel_t* nivel, int numero_nivel, int cantidad_baldosas_pinches, int cantidad_guardia, bool hay_bomba)
{
	(*nivel).tope_herramientas = MAX_HERRAMIENTAS;
	(*nivel).tope_obstaculos = MAX_OBSTACULOS;

	inicializar_coordenadas(nivel);
	
	coordenada_t coord = {0,0};
	int tamanio_nivel;
	int i, j;

	tamanio_nivel = tamanio_de_nivel(numero_nivel);
	(*nivel).tope_paredes = 0;

	obtener_paredes(numero_nivel, (*nivel).paredes, &((*nivel).tope_paredes));

	obtener_baldosas_pinche(nivel, tamanio_nivel, cantidad_baldosas_pinches);

	obtener_coordenada(*nivel, tamanio_nivel, &coord);			
	(*nivel).entrada = coord;

	obtener_coordenada(*nivel, tamanio_nivel, &coord);
	(*nivel).salida = coord;
	
	(*nivel).tope_obstaculos += cantidad_guardia;
	
	for(j = cantidad_baldosas_pinches; j < (*nivel).tope_obstaculos; j++)
	{
		(*nivel).obstaculos[j].tipo = GUARDIA;
		obtener_coordenada(*nivel, tamanio_nivel, &coord);
		(*nivel).obstaculos[j].posicion = coord;
	}

	(*nivel).tope_herramientas = cantidad_baldosas_pinches/2;			

	for(i = 0; i < cantidad_baldosas_pinches/2; i++)
	{
		(*nivel).herramientas[i].tipo = MONEDA;
	}

	(*nivel).tope_herramientas += cantidad_guardia;
	for(j = i; j < (*nivel).tope_herramientas; j++)
	{
		(*nivel).herramientas[j].tipo = B_TELETRANSP;
	}

	(*nivel).tope_herramientas +=1;
	(*nivel).herramientas[(*nivel).tope_herramientas-1].tipo = LLAVE;

	if(hay_bomba)
	{
		(*nivel).tope_obstaculos += 1;
		(*nivel).obstaculos[(*nivel).tope_obstaculos-1].tipo = BOMBA;
		obtener_coordenada(*nivel, tamanio_nivel, &coord);
		(*nivel).obstaculos[(*nivel).tope_obstaculos-1].posicion = coord;

		(*nivel).tope_herramientas += 1;
		(*nivel).herramientas[(*nivel).tope_herramientas-1].tipo = INTERRUPTOR;
	}

	for(i = 0; i < (*nivel).tope_herramientas; i++)
	{
		obtener_coordenada(*nivel, tamanio_nivel, &coord);
		(*nivel).herramientas[i].posicion = coord;
	}

}


/*
 * Pre condiciones: recibe un caracter
 *
 * Post condiciones: devuelve true si ese caracter es P_PURO_HUESO o P_POLLITO
 */
bool pers_no_afectado_por_pinche(char personaje)
{
	return (personaje == P_PURO_HUESO || personaje == P_POLLITO);
}

/*
 * Pre condiciones: recibe un caracter
 *
 * Post condiciones: devuelve true si ese caracter es P_JOHNNY_BRAVO o P_CORAJE
 */

bool pers_no_afectado_por_guardias(char personaje)
{
	return (personaje == P_JOHNNY_BRAVO || personaje == P_CORAJE);
}

/*
 * Pre condiciones: recibe un caracter
 *
 * Post condiciones: devuelve true si ese caracter es P_BLUE o P_BELLOTA
 */
bool pers_con_vida_extra(char personaje)
{
	return (personaje == P_BLUE || personaje == P_BELLOTA);
}

/*
 * Pre condiciones: recibe un vector de elementos, su tope y una coordenada
 *
 * Post condiciones: devuelve la posición del vector que coincide con la coordenada
 */
int posicion_herramienta(elemento_t herramientas[MAX_HERRAMIENTAS], int tope_herramientas, coordenada_t coord_herramienta)
{
	int i = 0;
	while(!es_misma_coordenada(herramientas[i].posicion, coord_herramienta) &&
			i < tope_herramientas)
	{
		i++;
	}
	return i;
}


/*
 * Pre condiciones: recibe un vector de elementos, su tope por referencia y una coordenada
 *
 * Post condiciones: elimina el elemento del vector que está en la coordenada dada y disminuye el tope
 */
void eliminar_herramienta(elemento_t herramientas[MAX_HERRAMIENTAS], int *tope_herramientas, coordenada_t coord_herramienta)
{
	int pos_herramienta = posicion_herramienta(herramientas, (*tope_herramientas), coord_herramienta);
	herramientas[pos_herramienta] = herramientas[(*tope_herramientas) - 1];
	(*tope_herramientas)--;
}

/*
 * Pre condiciones: recibe una coordenada, un vector de elementos y su tope
 *
 * Post condiciones: devuelve la coordenada de la siguiente baldosa teletransportadora del vector de elementos
 */
coordenada_t siguiente_b_teletransportadora(coordenada_t pos_personaje, elemento_t herramientas[MAX_HERRAMIENTAS], int tope)
{
	coordenada_t siguiente_baldosa = pos_personaje;
	int pos_baldosa_actual;
	for(int i = 0; i < tope; i++)
	{
		if(herramientas[i].tipo == B_TELETRANSP)
		{	
			if(es_misma_coordenada(herramientas[i].posicion, pos_personaje))
			{
				pos_baldosa_actual = i;
			}
		}
	}
	int i = pos_baldosa_actual+1;

	while(i < tope)/*busco de la posicion de la baldosa para adelante*/
	{
		if(herramientas[i].tipo == B_TELETRANSP)
		{
			siguiente_baldosa = herramientas[i].posicion;
			break;
		}
		i++;
	}
	if(es_misma_coordenada(siguiente_baldosa, pos_personaje)) //no entro al if del while anterior
	{
		i = 0; //me fijo a la derecha de la baldosa actual cual es la que esta primero
		while(i < pos_baldosa_actual)
		{
			if(herramientas[i].tipo == B_TELETRANSP)
			{
				siguiente_baldosa = herramientas[i].posicion;
				break;
			}
			i++;
		}
	}
	return siguiente_baldosa;
}

/*
 * Pre condiciones: recibe un juego, el tamaño del nivel y la posicion del personaje
 *
 * Post condiciones: devuelve el juego con nuevas coordenadas para los guardias
 */
void cambiar_guardias_de_posicion(juego_t *juego, int tamanio_nivel, coordenada_t pos_personaje)
{
 	coordenada_t coord;
 	for(int i = 0; i < (*juego).niveles[(*juego).nivel_actual-1].tope_obstaculos; i++)
	{
		if((*juego).niveles[(*juego).nivel_actual-1].obstaculos[i].tipo == GUARDIA)
		{
			obtener_coordenada_aleatoria(&coord, tamanio_nivel);
			while(!es_coordenada_valida((*juego).niveles[(*juego).nivel_actual-1], coord) || es_misma_coordenada(coord,pos_personaje))
			{
				obtener_coordenada_aleatoria(&coord, tamanio_nivel);
			}

			(*juego).niveles[(*juego).nivel_actual-1].obstaculos[i].posicion = coord;
		}
	}
}

/*
 * Pre condiciones: recibe un juego_t por referencia, una coordenada_t por referencia y 3 booleanos por referencia.
 *
 * Post condiciones: devuelve todos los parámetros que recibe actualizados.
 */
void elementos_en_el_camino(juego_t *juego, coordenada_t *pos_personaje, bool *teletransporte_personaje, bool *piso_bomba, bool *choco_guardia)
{
	bool coord_con_pinche, coord_con_guardia, coord_con_bomba;
	bool coord_con_b_teletransp, coord_con_moneda, coord_con_interruptor, coord_con_llave;

	coord_con_pinche = esta_elemento_buscado(B_PINCHE, (*pos_personaje), (*juego).niveles[(*juego).nivel_actual-1].obstaculos, (*juego).niveles[(*juego).nivel_actual-1].tope_obstaculos);

	coord_con_guardia = esta_elemento_buscado(GUARDIA, (*pos_personaje), (*juego).niveles[(*juego).nivel_actual-1].obstaculos, (*juego).niveles[(*juego).nivel_actual-1].tope_obstaculos);

	coord_con_bomba = esta_elemento_buscado(BOMBA, (*pos_personaje), (*juego).niveles[(*juego).nivel_actual-1].obstaculos, (*juego).niveles[(*juego).nivel_actual-1].tope_obstaculos);

	coord_con_moneda = esta_elemento_buscado(MONEDA, (*pos_personaje), (*juego).niveles[(*juego).nivel_actual-1].herramientas, (*juego).niveles[(*juego).nivel_actual-1].tope_herramientas);

	coord_con_llave = esta_elemento_buscado(LLAVE, (*pos_personaje), (*juego).niveles[(*juego).nivel_actual-1].herramientas, (*juego).niveles[(*juego).nivel_actual-1].tope_herramientas);

	coord_con_interruptor = esta_elemento_buscado(INTERRUPTOR, (*pos_personaje), (*juego).niveles[(*juego).nivel_actual-1].herramientas, (*juego).niveles[(*juego).nivel_actual-1].tope_herramientas);

	coord_con_b_teletransp = esta_elemento_buscado(B_TELETRANSP, (*pos_personaje), (*juego).niveles[(*juego).nivel_actual-1].herramientas, (*juego).niveles[(*juego).nivel_actual-1].tope_herramientas);


	if(coord_con_pinche && !pers_no_afectado_por_pinche((*juego).personaje.tipo))
	{
		(*juego).personaje.movimientos -= MOV_PERDIDOS_POR_PINCHE;
	}
	else if(coord_con_guardia)
	{
		
		if(!pers_no_afectado_por_guardias((*juego).personaje.tipo))
		{
			(*choco_guardia) = true;
			(*juego).personaje.movimientos = 0;
		}

	}
	else if(coord_con_bomba)
	{
		(*piso_bomba) = true;
	}
	else if(coord_con_moneda)
	{
		(*juego).personaje.movimientos += MOV_GANADOS_POR_MONEDA;
		eliminar_herramienta((*juego).niveles[(*juego).nivel_actual - 1].herramientas, &((*juego).niveles[(*juego).nivel_actual - 1].tope_herramientas), (*pos_personaje));
	}
	else if(coord_con_llave)
	{
		if((*juego).nivel_actual == 3 || (*juego).nivel_actual == 4)
		{
			if((*juego).personaje.presiono_interruptor)
			{
				(*juego).personaje.tiene_llave = true;
			}
		}
		else
		{
			(*juego).personaje.tiene_llave = true;
		}
	}
	else if(coord_con_interruptor)
	{
		(*juego).personaje.presiono_interruptor = true;
	}
	else if(coord_con_b_teletransp)
	{
		(*pos_personaje) = siguiente_b_teletransportadora((*pos_personaje), (*juego).niveles[(*juego).nivel_actual - 1].herramientas, (*juego).niveles[(*juego).nivel_actual - 1].tope_herramientas);
		(*teletransporte_personaje) = true;
	
	}
}

/*
 * Pre condiciones: recibe un caracter (ARRIBA, ABAJO, IZQUIERDA, DERECHA)
 *
 * Post condiciones: devuelve -1 si el caracter es ARRIBA o IZQUIERDA. Devuelve 1 si el caracter es ABAJO o DERECHA
 */
 int cantidad_desplazamiento(char movimiento)
 {
 	int desplazamiento = 0;

 	if( (movimiento == ARRIBA) || (movimiento == IZQUIERDA) )
 	{
 		desplazamiento = -1;
 	}
 	else if ((movimiento == ABAJO) || (movimiento == DERECHA))
 	{
 		desplazamiento = 1;
 	}

	return desplazamiento;
 }

/*
 * Pre condiciones: recibe un caracter (ARRIBA, ABAJO, IZQUIERDA, DERECHA), una coordenada por referencia y un entero que puede ser 1 o -1
 *
 * Post condiciones: devuelve la coordenada actualizada: - una fila menos si el caracter es ARRIBA
 *														 - una fila más si el caracter es ABAJO
 *														 - una columna menos si el caracter es DERECHA
 *														 - una columna más si el caracter es IZQUIERDA
 */
void mover_personaje_un_lugar(char direccion, coordenada_t *posicion_personaje, int desplazamiento)
{
	if(direccion == ARRIBA || direccion == ABAJO)
	{
		(*posicion_personaje).fil += desplazamiento;
	}
	else
	{
		(*posicion_personaje).col += desplazamiento;
	}
}


void mover_personaje(juego_t* juego, char movimiento)
{
	int tamanio_nivel;
	int desplazamiento;

	coordenada_t pos_personaje;
	pos_personaje = (*juego).personaje.posicion;
	
	bool teletransporte_personaje = false;
	bool piso_bomba = false;
	bool choco_guardia = false;

	tamanio_nivel = tamanio_de_nivel((*juego).nivel_actual);

	desplazamiento = cantidad_desplazamiento(movimiento);
 						
	mover_personaje_un_lugar(movimiento, &pos_personaje, desplazamiento);

	while( (!hay_pared((*juego).niveles[(*juego).nivel_actual-1], (pos_personaje))) &&
			(!(es_misma_coordenada((*juego).niveles[(*juego).nivel_actual-1].salida, pos_personaje) && ((*juego).personaje.tiene_llave)) ))
	{
		elementos_en_el_camino(juego, &pos_personaje, &teletransporte_personaje, &piso_bomba, &choco_guardia);
		if(teletransporte_personaje)
			break;
		if( piso_bomba || choco_guardia)
		{
			(*juego).personaje.posicion = (pos_personaje);
			(*juego).personaje.movimientos = 0;
			break;
		}

		mover_personaje_un_lugar(movimiento, &pos_personaje, desplazamiento);

	}
	if(!teletransporte_personaje && 
		(!es_misma_coordenada((*juego).niveles[(*juego).nivel_actual-1].salida, pos_personaje)) &&
		(!choco_guardia) && (!piso_bomba))
	{
		/*lo muevo una posición para atrás para corregir*/
		mover_personaje_un_lugar(movimiento, &pos_personaje, desplazamiento*(-1));
	}

	if(!choco_guardia)
	{
		(*juego).personaje.posicion = (pos_personaje);
	}

	cambiar_guardias_de_posicion(juego, tamanio_nivel, (pos_personaje));

	(*juego).personaje.movimientos--;

	if((*juego).personaje.movimientos <= 0)
	{
		(*juego).personaje.movimientos = 0;
		
		if((*juego).personaje.murio == false)
		{
			(*juego).personaje.murio = true;
			if(pers_con_vida_extra((*juego).personaje.tipo))
			{
				(*juego).personaje.movimientos = MOVIMIENTOS_INICIAL;				
			}
		}
	}
}

/*
*	Pre: recibe una matriz cuadrada y su tope para llenarla con la información de juego
*
*	Post: devuelve la matriz nivel con toda la información de juego acomodada
*/
void crear_mapa_nivel(char nivel[MAX_FIL_NIVEL][MAX_COL_NIVEL], int tope, juego_t juego)
{
	int fil, col;
	int i, j;

	/*inicializo con espacios*/
	for(i = 0; i < tope; i++)
	{
		for(j = 0; j < tope; j++)
		{
			nivel[i][j] = POS_VACIA;
		}
	}

	/*pongo la entrada*/
	fil = juego.niveles[(juego.nivel_actual)-1].entrada.fil;
	col = juego.niveles[(juego.nivel_actual)-1].entrada.col;
	nivel[fil][col] = ENTRADA;

	/*pongo la salida*/
	fil = juego.niveles[(juego.nivel_actual)-1].salida.fil;
	col = juego.niveles[(juego.nivel_actual)-1].salida.col;	
	nivel[fil][col] = SALIDA;

	/*pongo las paredes*/
	for(i = 0; i < juego.niveles[(juego.nivel_actual)-1].tope_paredes; i++)
	{
		fil = juego.niveles[(juego.nivel_actual)-1].paredes[i].fil;
		col = juego.niveles[(juego.nivel_actual)-1].paredes[i].col;

		nivel[fil][col] = PARED;
	}

	/*pongo los obstaculos*/
	for(i = 0; i < juego.niveles[(juego.nivel_actual)-1].tope_obstaculos; i++)
	{
		fil = juego.niveles[(juego.nivel_actual)-1].obstaculos[i].posicion.fil;
		col = juego.niveles[(juego.nivel_actual)-1].obstaculos[i].posicion.col;

		nivel[fil][col] = juego.niveles[(juego.nivel_actual)-1].obstaculos[i].tipo;
	}

	/*pongo las herramientas*/
	for(i = 0; i < juego.niveles[(juego.nivel_actual)-1].tope_herramientas; i++)
	{
		fil = juego.niveles[(juego.nivel_actual)-1].herramientas[i].posicion.fil;
		col = juego.niveles[(juego.nivel_actual)-1].herramientas[i].posicion.col;

		nivel[fil][col] = juego.niveles[(juego.nivel_actual)-1].herramientas[i].tipo;
	}

	/*pongo al personaje*/
	fil = juego.personaje.posicion.fil;
	col = juego.personaje.posicion.col;

	nivel[fil][col] = PERSONAJE;
}

/*
 *	Pre: -
 *
 *	Post: muestra las referencias del juego por pantalla
 */
void mostrar_referencias()
{
	printf(GRIS "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n" FIN);

	printf(GRIS "*  " FIN);
	printf(CELESTE_SUAVE "%c" FIN, B_PINCHE);
	printf(": baldosa pinche - resta 2 movimientos");
	                                  printf(GRIS "                                                      *\n" FIN);

	printf(GRIS "*  " FIN);
	printf(AZUL "%c" FIN, GUARDIA);
	printf(": guardia robot - quita todos los movimientos");
	                                         printf(GRIS "                                               *\n" FIN);

	printf(GRIS "*  " FIN);
	printf(ROJO_CLARO "%c" FIN, BOMBA);
	printf(": bomba - al pisarla se pierde automáticamente");
	                                          printf(GRIS "                                              *\n" FIN);

	printf(GRIS "*  " FIN);
	printf(VERDE_OSCURO "%c" FIN, ENTRADA);
				printf(": entrada del nivel"); 
	                           printf(GRIS "                                                                         *\n" FIN);
	printf(GRIS "*  " FIN);
 	printf(VERDE_CLARO "%c" FIN, SALIDA);
	            printf(": salida del nivel"); 
	                          printf(GRIS "                                                                          *\n" FIN);

	printf(GRIS "*  " FIN);
	printf(MARRON "%c" FIN, LLAVE);
	printf(": llave - es necesaria para poder salir");
	                                   printf(GRIS "                                                     *\n" FIN);

	printf(GRIS "*  " FIN);
	printf(CELESTE "%c" FIN, INTERRUPTOR);
	printf(": interruptor - es necesario tocarlo para ver la llave");
	                                                  printf(GRIS "                                      *\n" FIN);

	printf(GRIS "*  " FIN);
	printf(AMARILLO "%c" FIN, MONEDA);
	printf(": AlgoCoin - suma 3 movimientos");
	                           printf(GRIS "                                                             *\n" FIN);

	printf(GRIS "*  " FIN);
	printf(MAGENTA "%c" FIN, B_TELETRANSP);
	printf(": baldosa teletransportadora - al pisarla se transporta a otra baldosa teletransportadora");
	                                                                                     printf(GRIS "   *\n" FIN);

	printf(GRIS "*  " FIN);
	printf("%c", PERSONAJE);
	printf(": personaje");
	       printf(GRIS "                                                                                 *\n" FIN);


	printf(GRIS "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n" FIN);

}

/*
 * Pre: recibe una matriz cuadrada válida y su tope
 *
 * Post: imprime la matriz por pantalla
 */
void mostrar_nivel(char nivel[MAX_FIL_NIVEL][MAX_COL_NIVEL], int tope)
{
	for(int i = 0; i < tope; i++)
	{
		printf("\t \t \t \t");
		for(int j = 0; j < tope; j++)
		{
			if(nivel[i][j] == B_PINCHE)
				printf(CELESTE_SUAVE "%c "FIN, nivel[i][j]);

			else if(nivel[i][j] == BOMBA)
				printf(ROJO_CLARO "%c "FIN, nivel[i][j]);

			else if(nivel[i][j] == GUARDIA)
				printf(AZUL "%c "FIN, nivel[i][j]);

			else if(nivel[i][j] == ENTRADA)
				printf(VERDE_OSCURO "%c "FIN, nivel[i][j]);

			else if(nivel[i][j] == SALIDA)
				printf(VERDE_CLARO "%c "FIN, nivel[i][j]);

			else if(nivel[i][j] == LLAVE)
				printf(MARRON "%c " FIN , nivel[i][j]);

			else if(nivel[i][j] == MONEDA)
				printf(AMARILLO "%c "FIN , nivel[i][j]);

			else if(nivel[i][j] == B_TELETRANSP)
				printf(MAGENTA "%c " FIN , nivel[i][j]);

			else if(nivel[i][j] == INTERRUPTOR)
				printf(CELESTE "%c " FIN, nivel[i][j]);

			else if(nivel[i][j] == PERSONAJE)
				printf("%c ", nivel[i][j]);

			else if(nivel[i][j] == PARED)
				printf(GRIS "%c "FIN, nivel[i][j]);

			else
				printf("%c ", nivel[i][j]);
		}
		printf("\n");
	}
}

/*
 * Pre condiciones: recibe un juego_t ya inicializado
 *
 * Post condiciones: muestra las características relevantes del juego por pantalla
 */
void mostrar_encabezado(juego_t juego)
{
	printf("Movimientos restantes: %i", juego.personaje.movimientos);
	printf("\t\t\t\t\tPersonaje: ");
	if(juego.personaje.tipo == P_BLUE )
	{
		printf("Blue\n");
		printf("\t\t\t\t\t\t\t\tHabilidad: 10 movimientos extra al perder\n");
	}
	else if(juego.personaje.tipo == P_BELLOTA)
	{
		printf("Bellota\n");
		printf("\t\t\t\t\t\t\t\tHabilidad: 10 movimientos extra al perder\n");
	}
	else if(juego.personaje.tipo == P_POLLITO )
	{
		printf("Pollito\n");
		printf("\t\t\t\t\t\t\t\tHabilidad: no lo afectan los pinches\n");
	}
	else if(juego.personaje.tipo == P_CORAJE )
	{
		printf("Coraje\n");
		printf("\t\t\t\t\t\t\t\tHabilidad: no lo afectan los guardias robot\n");
	}
	else if(juego.personaje.tipo == P_JOHNNY_BRAVO )
	{
		printf("Johnny Bravo\n");
		printf("\t\t\t\t\t\t\t\tHabilidad: no lo afectan los guardias robot\n");
	} 
	else if(juego.personaje.tipo == P_PURO_HUESO )
	{
		printf("Puro Hueso\n");
		printf("\t\t\t\t\t\t\t\tHabilidad: no lo afectan los pinches\n");
	}

	if((juego.nivel_actual == 3 || juego.nivel_actual == 4))
	{
		if(juego.personaje.presiono_interruptor)
		{
			printf(" Interruptor ");
			printf(VERDE_CLARO"✔\n" FIN);
		}
		else
		{
			printf(" Interruptor ");
			printf(ROJO_OSCURO "✖\n" FIN);
		}		
	}

	if(juego.personaje.tiene_llave)
	{
		printf(" Llave ");
		printf(VERDE_CLARO"✔\n" FIN);
		printf(" Salida ");
		printf(VERDE_CLARO"●\n" FIN);

	}
	else
	{
		printf(" Llave ");
		printf(ROJO_OSCURO "✖\n" FIN);
		printf(" Salida ");		
		printf(ROJO_OSCURO "●\n" FIN);
	}
}

void mostrar_juego(juego_t juego)
{
	char mapas_niveles[MAX_NIVELES][MAX_FIL_NIVEL][MAX_COL_NIVEL];
	int tamanio_nivel;

	coordenada_t pos_llave;
	coordenada_t pos_salida;

	tamanio_nivel = tamanio_de_nivel(juego.nivel_actual);	

	
	crear_mapa_nivel(mapas_niveles[juego.nivel_actual - 1], tamanio_nivel, juego);

	
	for(int i = 0; i < juego.niveles[(juego.nivel_actual)-1].tope_herramientas; i++)
	{
		if(juego.niveles[(juego.nivel_actual)-1].herramientas[i].tipo == LLAVE)
		{
			pos_llave = juego.niveles[(juego.nivel_actual)-1].herramientas[i].posicion;
		}
	}

	if(juego.personaje.presiono_interruptor == false && (juego.nivel_actual == 3 || juego.nivel_actual == 4))
	{
		mapas_niveles[(juego.nivel_actual)-1][pos_llave.fil][pos_llave.col] = POS_VACIA;			
	}
	else
	{		
		mapas_niveles[(juego.nivel_actual)-1][pos_llave.fil][pos_llave.col] = LLAVE;
	}
	
	if(juego.personaje.tiene_llave == false)
	{	
		pos_salida = juego.niveles[(juego.nivel_actual)-1].salida;

		mapas_niveles[(juego.nivel_actual)-1][pos_salida.fil][pos_salida.col] = POS_VACIA;			
	}
	else
	{
		pos_salida = juego.niveles[(juego.nivel_actual)-1].salida;

		mapas_niveles[(juego.nivel_actual)-1][pos_salida.fil][pos_salida.col] = SALIDA;
	}
	
	mapas_niveles[(juego.nivel_actual)-1][juego.personaje.posicion.fil][juego.personaje.posicion.col] = PERSONAJE;

	mostrar_encabezado(juego);

	printf("\t \t \t \t");
	printf(" -  Nivel %i  - ", juego.nivel_actual);

	if(pers_con_vida_extra(juego.personaje.tipo))
	{
		if(juego.personaje.murio)
			printf(ROJO_OSCURO "♡\n" FIN);
		else
			printf(ROJO_OSCURO "♥\n" FIN);
	}
	else
	{
		printf("\n");
	}
	
	mostrar_nivel(mapas_niveles[juego.nivel_actual - 1], tamanio_nivel);
	mostrar_referencias();
}

int estado_juego(juego_t juego)
{
	int estado = JUEGO_JUGANDO;

	if(juego.nivel_actual == 4)
	{
		if(estado_nivel(juego.personaje, juego.niveles[juego.nivel_actual-1].salida) == NIVEL_GANADO)
		{
			estado = JUEGO_GANADO;
		}
	}

	if((juego.personaje.movimientos <= 0) && (juego.personaje.murio) && 
		!es_misma_coordenada(juego.personaje.posicion,juego.niveles[juego.nivel_actual-1].salida))
	{
		estado = JUEGO_PERDIDO;
	}

	return estado;
}

int estado_nivel(personaje_t personaje, coordenada_t salida)
{
	int estado = NIVEL_JUGANDO;

	if( (es_misma_coordenada(personaje.posicion, salida)) && personaje.tiene_llave && (personaje.movimientos >= 0))
	{
		estado = NIVEL_GANADO;
	}

	return estado;
}