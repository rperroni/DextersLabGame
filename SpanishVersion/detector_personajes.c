#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "detector_personajes.h"

/*	Colores 	*/
#define COLOR_FIN 				"\033[0m"
#define COLOR_NEGRO 			"\033[0m\033[30m"
#define COLOR_GRIS 				"\033[1m\033[30m"
#define COLOR_ROJO_OSCURO 		"\033[0m\033[31m"
#define COLOR_ROJO_CLARO 		"\033[1m\033[31m"
#define COLOR_VERDE_OSCURO 		"\033[0m\033[32m"
#define COLOR_VERDE_CLARO 		"\033[1m\033[32m"
#define COLOR_AMARILLO	 		"\033[1m\033[33m"
#define COLOR_MARRON		 	"\033[0m\033[33m"
#define COLOR_AZUL	 			"\033[1m\033[34m"
#define COLOR_AZUL_SUAVE	 	"\033[0m\033[34m"
#define COLOR_MAGENTA_SUAVE 	"\033[0m\033[35m"
#define COLOR_MAGENTA 			"\033[1m\033[35m"
#define COLOR_CELESTE_SUAVE 	"\033[0m\033[36m"
#define COLOR_CELESTE 			"\033[1m\033[36m"
#define COLOR_BLANCO		 	"\033[1m\033[37m"

/*   Personajes   */
const char JOHNNY_BRAVO = 'J';
const char BELLOTA = 'S';
const char POLLITO = 'P';
const char BLUE = 'B';
const char PURO_HUESO = 'H';
const char CORAJE = 'C';

/*  Alturas predeterminadas de los personajes  */
const int ALTURA_JOHNNY = 180;
const int ALTURA_BELLOTA = 120;
const int ALTURA_POLLITO = 50;
const int ALTURA_BLUE = 140;
const int ALTURA_PURO_HUESO = 200;
const int ALTURA_CORAJE = 30;

/*   Puntos por sabor preferido   */
const int PUNTOS_SALADO = 5;
const int PUNTOS_DULCE = 15;
const int PUNTOS_AMARGO = 20;

/*   Puntos por color preferido   */
const int PUNTOS_ROSA_AMARILLO = 5;
const int PUNTOS_NEGRO_BLANCO = 15;
const int PUNTOS_AZUL_VERDE = 20;

/*   Límites del año de nacimiento   */
const int ANIO_MAX = 2008;
const int ANIO_MIN = 1988;

/*   Preferencia de sabor   */
const char DULCE = 'D';
const char SALADO = 'S';
const char AMARGO = 'A';

/*   Límites talla de zapatos   */
const int TALLA_MAX = 47;
const int TALLA_MIN = 33;
const int SIN_ZAPATOS = 0;

/*   Preferencia de color   */
const char AZUL = 'A';
const char AMARILLO = 'Y';
const char VERDE = 'V';
const char NEGRO = 'N';
const char BLANCO = 'B';
const char ROSA = 'R';

/*   Límites de altura   */
const int ALTURA_MAX = 250;
const int ALTURA_MIN = 10;

/*   Multiplicadores   */
const int x1 = 1;
const int x2 = 2;
const int x3 = 3;
const int x4 = 4;

const int TALLA_MAX_x2 = 37;
const int TALLA_MAX_x3 = 42;

/*   Separadores de asignación de puntaje   */
const int PUNTAJE_JBoC_MIN = 0; //Johnny Bravo o Coraje
const int PUNTAJE_JBoC_MAX = 80; //el puntaje esta entre 0 y 80

const int PUNTAJE_PHoP_MAX = 160; //Puro Hueso o Pollito
								  //el puntaje está entre 80 y 160
const int PUNTAJE_BEoBL = 240; //Bellota o Blue
								//el puntaje está entre 160 y 240

/*
 *Pre: esta funcion recibe un numero entero positivo.
 *     
 *Post: devuelve true si el numero ingresado esta comprendido 
 *		entre ANIO_MIN y ANIO_MAX;	o false si no.
*/
bool es_anio_valido(int anio)
{
	return( anio >= ANIO_MIN && anio <= ANIO_MAX);
}

/*
 *Pre: Ninguna.
 *
 *Post: lee el año de nacimiento del personaje. Si está entre ANIO_MIN y 
 *		ANIO_MAX devuelve el año ingresado, si no lo vuelve a pedir hasta
 *		que sea válido
*/
int anio_nacimiento_ingresado()
{
	int anio_nacimiento;

	printf("\n¿En que año naciste? (entre %i y %i):\n", 
									ANIO_MIN, ANIO_MAX);
	printf(COLOR_AMARILLO "> " COLOR_FIN);
	scanf("%i", &anio_nacimiento);

	while(!es_anio_valido(anio_nacimiento))
	{
		printf("\nAño de nacimiento inválido. Debe ser entre %i y %i:\n", 
														ANIO_MIN, ANIO_MAX);
		printf(COLOR_AMARILLO "> " COLOR_FIN);
		scanf("%i", &anio_nacimiento);		
	}

	return anio_nacimiento;
}

/*
 *Pre: Ninguna.
 *
 *Post: muestra por consola que el usuario debe ingresar una 
 *		preferencia de sabor del personaje y las 3 opciones posibles.
*/
void mostrar_sabores()
{
	printf("- '%c' para DULCE\n", DULCE);
	printf("- '%c' para SALADO\n", SALADO);
	printf("- '%c' para AMARGO\n", AMARGO);
	printf(COLOR_AMARILLO "> " COLOR_FIN);
}

/*
 *Pre: esta funcion recibe un caracter que es 'D' (dulce), 'S' (salado)
 *		o 'A' (amargo)
 *      
 *Post: devuelve true si el caracter ingresado está dentro de las 
 *		opciones o false si no lo está.
*/
bool es_sabor_valido(char sabor)
{
	return(sabor == DULCE  || 
		   sabor == SALADO || 
		   sabor == AMARGO);
}

/*
 *Pre: Ninguna.
 *
 *Post: lee el sabor preferido del personaje. Si es 'D', 'S' o 'A' y
 *		devuelve el sabor ingresado, si no lo vuelve a pedir hasta
 *		que sea válido.
*/
char sabor_preferido_ingresado()
{
	char sabor_preferido;

	printf("\n¿Qué sabor prefieres?:\n");
	mostrar_sabores();
	scanf(" %c", &sabor_preferido);

	while(!es_sabor_valido(sabor_preferido))
	{
		printf("\nSabor inválido. Debe ser alguno de estos:\n");
		mostrar_sabores();
		scanf(" %c", &sabor_preferido);
	}

	return sabor_preferido;
}

/*
 *Pre: esta funcion recibe un numero entero.
 *     
 *Post: devuelve true si el numero ingresado es SIN_ZAPATOS 
 *		o si está entre TALLA_MIN y TALLA_MAX. 
 *		De lo contrario devuelve false.
*/
bool es_talla_valida(int talla)
{
	if(talla < TALLA_MIN)
		return(talla == SIN_ZAPATOS);//si es SIN_ZAPATOS da verdadero, si no da falso
	else
		return (talla <= TALLA_MAX); //si es mayor a TALLA_MIN y menor
}									//a TALLA_MAX devuelve true, si no false

/*
 *Pre: Ninguna.
 *
 *Post: lee la talla de zapatos del personaje. Si es SIN_ZAPATOS o si está entre
 *		TALLA_MIN y TALLA_MAX devuelve el talle de zapatos, si no lo vuelve a pedir 
 *		hasta que sea válido.
*/
int talla_zapatos_ingresada()
{
	int talla_zapatos;
	printf("\n¿Cuánto calzas? (entre %i y %i). Ingresa %i si no usas zapatos:\n", 
		TALLA_MIN, TALLA_MAX, SIN_ZAPATOS);
	printf(COLOR_AMARILLO "> " COLOR_FIN);
	scanf("%i", &talla_zapatos);

	while(!es_talla_valida(talla_zapatos))
	{
		printf("\nTalla inválida. Debe estar entre %i y %i, o %i si no usas zapatos:\n", 
										TALLA_MIN, TALLA_MAX, SIN_ZAPATOS);
		printf(COLOR_AMARILLO "> " COLOR_FIN);
		scanf("%i", &talla_zapatos);
	}
	return talla_zapatos;
}
/*
 *Pre: Ninguna.
 *
 *Post: muestra por consola que el usuario debe ingresar un color 
 *		preferido del personaje y las opciones disponibles para 
 *		hacerlo.
*/
void mostrar_colores()
{
	printf(COLOR_AZUL "- '%c' para AZUL\n" COLOR_FIN, AZUL);
	printf(COLOR_AMARILLO "- '%c' para AMARILLO\n" COLOR_FIN, AMARILLO);
	printf(COLOR_VERDE_CLARO"- '%c' para VERDE\n" COLOR_FIN, VERDE);
	printf(COLOR_NEGRO"- '%c' para NEGRO\n" COLOR_FIN, NEGRO);
	printf(COLOR_BLANCO"- '%c' para BLANCO\n"COLOR_FIN, BLANCO);
	printf(COLOR_MAGENTA"- '%c' para ROSA\n"COLOR_FIN, ROSA);
	printf(COLOR_AMARILLO "> " COLOR_FIN);
}

/*
 *Pre: esta funcion recibe un caracter que es 'A' (azul), 'Y' 
 *		(amarillo), 'V' (verde), 'N' (negro), 'B' (blanco) o 
 *		'R' (rosa).
 *     
 *Post: devuelve true si el caracter coincide con alguna de 
 *		las opciones. Si no, devuelve false.
*/
bool es_color_valido(char color)
{
	return(color == AZUL ||
		   color == ROSA ||
		   color == VERDE ||
		   color == NEGRO ||
		   color == BLANCO ||
		   color == AMARILLO );
}

/*
 *Pre: Ninguna.
 *
 *Post: lee el color preferido del personaje. Si es alguna de las opciones
 *		devuelve el color preferido, si no lo vuelve a pedir 
 *		hasta que sea válido.
*/
char color_preferido_ingresado()
{
	char color_preferido;

	printf("\n¿Cuál color prefieres?\n");
	mostrar_colores();
	scanf(" %c", &color_preferido);

	while(!es_color_valido(color_preferido))
	{
		printf("\nColor inválido. Debe ser alguno de estos:\n");
		mostrar_colores();
		scanf(" %c", &color_preferido);
	}

	return color_preferido;
}

/*
 *Pre: esta funcion recibe numero entero.
 *     
 *Post: devuelve true si el numero está comprendido entre 
 *		ALTURA_MIN y ALTURA_MAX . Si no, devuelve false.
*/
bool es_altura_valida(int altura)
{
	return(altura >= ALTURA_MIN && 
			altura <= ALTURA_MAX);
}

/*
 *Pre: Ninguna.
 *
 *Post: lee la altura del personaje. Si esta entre ALTURA_MIN y ALTURA_MAX
 *		devuelve la altura, si no lo vuelve a pedir hasta que sea válido.
*/
int altura_ingresada()
{
	int altura_personaje;

	printf("\n¿Cuál es tu altura? (entre %i cm y %i cm):\n", 
									ALTURA_MIN, ALTURA_MAX);
	printf(COLOR_AMARILLO "> " COLOR_FIN);
	scanf("%i", &altura_personaje);

	while(!es_altura_valida(altura_personaje))
	{
		printf("\nAltura inválida. Debe estar entre %i cm y %i cm:\n", 
											ALTURA_MIN, ALTURA_MAX);
		printf(COLOR_AMARILLO "> " COLOR_FIN);
		scanf("%i", &altura_personaje);
	}

	return altura_personaje;
}

/*
 *Pre: esta función recibe un numero positivo que vale SIN_ZAPATOS, o esta 
 *		comprendido entre TALLA_MIN y TALLA_MAX.
 *     
 *Post: retorna el valor x1 si el numero ingresado es SIN_ZAPATOS, x2 si está 
 *		entre TALLA_MIN y TALLA_MAX_x2, x3 si está entre TALLA_MAX_x2 y 
 *		TALLA_MAX_x3 y x4 si es mayor a TALLA_MAX_x3 y menor o igual a 
 *		TALLA_MAX (por defecto es el ultimo else).
*/
int multiplicador_talla_zapatos (int talla_zapatos)
{
	int multiplicador;

	if(talla_zapatos == SIN_ZAPATOS)
		multiplicador = x1;

	else if( talla_zapatos >= TALLA_MIN && talla_zapatos <= TALLA_MAX_x2)
		multiplicador = x2;

	else if( talla_zapatos > TALLA_MAX_x2 && talla_zapatos <= TALLA_MAX_x3)
		multiplicador = x3;

	else if(talla_zapatos > TALLA_MAX_x3)
		multiplicador = x4;

	return multiplicador;
}

/*
 *Pre: esta funcion recibe numero entero positivo entre 1988 y 2008.
 *     
 *Post: devuelve un numero entero que es 1 más el resto de la división
 *		del año de nacimiento por 20.
*/
int puntaje_anio_nacimiento(int anio_nacimiento)
{
	return ((anio_nacimiento % 20) + 1);
}

/*
 *Pre: esta funcion recibe una letra que puede ser 'D' (dulce),
 *		'S' (salado) o 'A' (amargo).
 *     
 *Post: devuelve los puntos que son asignados de acuerdo al sabor 
 *		ingresado (PUNTOS_SALADO si es SALADO, PUNTOS_DULCE si es 
 *		DULCE y PUNTOS_AMARGO si es AMARGO).
*/
int puntaje_sabor(char sabor)
{
	int puntaje;

	if(sabor == SALADO)
		puntaje = PUNTOS_SALADO;
	else if(sabor == DULCE)
		puntaje = PUNTOS_DULCE;
	else if (sabor == AMARGO)
		puntaje = PUNTOS_AMARGO;

	return puntaje;
}

/*
 *Pre: esta funcion recibe una letra: AZUL, AMARILLO, NEGRO, BLANCO
 *		ROSA o VERDE.
 *     
 *Post: devuelve el puntaje que corresponde a cada una de las opciones. 
 *		PUNTOS_ROSA_AMARILLO si es ROSA o AMARILLO, PUNTOS_NEGRO_BLANCO
 *		si es NEGRO o BLANCO y PUNTOS_AZUL_VERDE si es AZUL o VERDE.
*/
int puntaje_color (char color)
{
	int puntaje; 

	if(color == ROSA || color == AMARILLO)
		puntaje = PUNTOS_ROSA_AMARILLO;

	else if(color == NEGRO || color == BLANCO)
		puntaje = PUNTOS_NEGRO_BLANCO;

	else if(color == AZUL || color == VERDE)
		puntaje = PUNTOS_AZUL_VERDE;

	return puntaje;
}

/*
 *Pre: esta funcion recibe los parámetros ingresados por el usuario:
 *	   - Color preferido del personaje (caracter):
 *	   		- AZUL, VERDE, NEGRO, BLANCO, ROSA, AMARILLO.
 *	   - Año de nacimiento del personaje (entero): entre ANIO_MIN y ANIO_MAX
 *	   - Sabor preferido del personaje (caracter): SALADO, DULCE o AMARGO
 *	   - Talla de zapatos del personaje (entero): TALLA_MIN, TALLA_MAX o
 *	   											  SIN_ZAPATOS
 *     
 *Post: devuelve un numero entero resultante de sumar el puntaje por 
 *		color, el puntaje por año de nacimiento y el puntaje por sabor 
 *		preferido del personaje, multiplicado por un valor de acuerdo 
 *		a la talla de zapatos. Todos estos valores estan dados por 
 *		otras funciones.
*/
int puntaje_personaje(char color, int anio, char sabor, int talla)
{
	return (( puntaje_color(color) + 
			  puntaje_anio_nacimiento(anio) + 
			  puntaje_sabor(sabor) ) * 
			  multiplicador_talla_zapatos(talla) );
}

/*
 *Pre: esta funcion recibe un numero entero que fue calculado en otra 
 *		función (puntaje_personaje()) y otro entero que debe estar entre 
 *		ALTURA_MIN y ALTURA_MAX.
 *     
 *Post: devuelve un caracter que representa al personaje identificado 
 *		de acuerdo a la diferencia entre la altura ingresada y la 
 *		altura predeterminada del personaje y el puntaje obtenido (el 
 *		primer dato que ingresa a la función). Los caracteres a 
 * 		devolver deben ser:
 *
 *		- JOHNNY_BRAVO
 *		- CORAJE
 *		- PURO_HUESO
 *		- POLLITO
 *		- BLUE
 *		- BELLOTA
*/
char personaje(int puntaje, int altura)
{
	int dif_altura_pers1, dif_altura_pers2;
	char personaje;

	if(puntaje > PUNTAJE_JBoC_MIN && puntaje <= PUNTAJE_JBoC_MAX) 
	{/* significa que es Johnny o Coraje */
		dif_altura_pers1 = abs(ALTURA_JOHNNY - altura);
		dif_altura_pers2 = abs(ALTURA_CORAJE - altura);

		if(dif_altura_pers1 > dif_altura_pers2)
			personaje = CORAJE;
		else
			personaje = JOHNNY_BRAVO;
	}

	else if(puntaje > PUNTAJE_JBoC_MAX && puntaje <= PUNTAJE_PHoP_MAX) 
	{/* significa que es Puro Hueso o Pollito */
		dif_altura_pers1 = abs(ALTURA_PURO_HUESO - altura);
		dif_altura_pers2 = abs(ALTURA_POLLITO - altura);

		if(dif_altura_pers1 > dif_altura_pers2)
			personaje = POLLITO;
		else
			personaje = PURO_HUESO;
	}

	else if(puntaje > PUNTAJE_PHoP_MAX && puntaje <= PUNTAJE_BEoBL) 
	{/* significa que es Blue o Bellota */
		dif_altura_pers1 = abs(ALTURA_BLUE - altura);
		dif_altura_pers2 = abs(ALTURA_BELLOTA - altura);

		if(dif_altura_pers1 > dif_altura_pers2)
			personaje = BELLOTA;
		else
			personaje = BLUE;
	}

	return personaje;
}

/*
 *Pre: recibe un caracter que debe ser: JOHNNY_BRAVO, BELLOTA, POLLITO
 *	   BLUE, PURO_HUESO o CORAJE.
 *
 *Post: muestra por consola un mensaje al usuario que le indica	cual 
 *		de los personajes ha sido detectado de acuerdo a la información 
 *		ingresada previamente.
*/
void mostrar_personaje(char personaje)
{
	printf("\n- ");

	if(personaje == JOHNNY_BRAVO)
		printf(COLOR_AMARILLO "Johnny Bravo" COLOR_FIN);
	else if(personaje == BELLOTA)
		printf(COLOR_VERDE_CLARO "Bellota" COLOR_FIN);
	else if(personaje == POLLITO)
		printf(COLOR_BLANCO "Pollito" COLOR_FIN);
	else if(personaje == BLUE)
		printf(COLOR_AZUL "Blue" COLOR_FIN);
	else if(personaje == PURO_HUESO)
		printf(COLOR_GRIS "Puro Hueso" COLOR_FIN);
	else if(personaje == CORAJE)
		printf(COLOR_MAGENTA "Coraje" COLOR_FIN);

	printf(" - ha sido detectado!\n");
}

void detectar_personaje(char *personaje_detectado)
{
	int anio_nacimiento, talla_zapatos, altura_personaje;
	char sabor_preferido, color_preferido;

	int puntaje_total;

	/*   Mensaje inicial   */
	printf(COLOR_ROJO_CLARO "\n\n¡Intento de ingreso al laboratorio detectado!\n" COLOR_FIN);
	printf("Responde las preguntas de seguridad:\n");

	/*   Ingreso del año de nacimiento del personaje   */	
	anio_nacimiento = anio_nacimiento_ingresado();	

	/*   Ingreso de la preferencia de sabor del personaje   */
	sabor_preferido = sabor_preferido_ingresado();	

	/*   Ingreso de la talla de zapatos del personaje   */
	talla_zapatos = talla_zapatos_ingresada();	

	/*   Ingreso del color preferido del personaje   */
	color_preferido = color_preferido_ingresado();
	
	/*   Ingreso de la altura del personaje   */
	altura_personaje = altura_ingresada();
	
	/*   Cálculo del puntaje resultante para el personaje   */
	puntaje_total = puntaje_personaje(color_preferido, anio_nacimiento, 
									 sabor_preferido, talla_zapatos);

	(*personaje_detectado) = personaje(puntaje_total, altura_personaje);

	/*	  Mostrar el personaje que ha sido detectado	  */
	mostrar_personaje( *personaje_detectado );
}
