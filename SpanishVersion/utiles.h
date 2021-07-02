#ifndef __UTILES_H__
#define __UTILES_H__
#include <stdio.h>

#define MAX_PAREDES 250

/* Pre condiciones: El parametro nivel debe contener el valor de un nivel (1 a 4).
 * Post condiciones: Devuelve el vector de coordenadas de las paredes cargado, junto a su respectivo tope.
*/
void obtener_paredes(int nivel, coordenada_t paredes[MAX_PAREDES], int *tope_paredes);

/* Pre condiciones: La variable segundos debe contener un valor positivo.
 * Post condiciones: Detendrá el tiempo los segundos indicados como parámetro.
*/
void detener_el_tiempo(float segundos);

#endif /* __UTILES_H__ */