#include "escape_laboratorio.h"
#include "detector_personajes.h"
#include "utiles.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#define ARRIBA 'w'
#define ABAJO 's'
#define DERECHA 'd'
#define IZQUIERDA 'a'

#define PERSONAJE 'X'

/*	OBSTÁCULOS	*/
#define B_PINCHE 'P'
#define BOMBA 'B'
#define GUARDIA 'G'

/*	HERRAMIENTAS	*/
#define INTERRUPTOR 'I'
#define B_TELETRANSP 'T'
#define MONEDA 'M'
#define LLAVE 'L'

#define JUEGO_PERDIDO -1
#define JUEGO_GANADO 1
#define JUEGO_JUGANDO 0

#define NIVEL_GANADO 1
#define NIVEL_JUGANDO 0

/*	COLORES	 */
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

/*
 * Pre condiciones: recibe un caracter
 *
 * Post condiciones: devuelve true si el caracter es ARRIBA, ABAJO, IZQUIERDA o DERECHA. De lo contrario devuelve false
 */
bool es_movimiento_valido(char movimiento)
{
	return (movimiento == ARRIBA ||
			movimiento == ABAJO ||
			movimiento == IZQUIERDA ||
			movimiento == DERECHA);
}

/*
 * Pre condiciones: -
 *
 * Post condiciones: muestra por pantalla el mensaje "Laboratorio de Dexter"
 */
void mostrar_msj_laboratorio_de_dexter()
{
	printf("  _          _                     _             _             _        ____            _            \n");
	printf(" | |    __ _| |__   ___  _ __ __ _| |_ ___  _ __(_) ___     __| | ___  |  _ \\  _____  _| |_ ___ _ __ \n");
	printf(" | |   / _` | '_ \\ / _ \\| '__/ _` | __/ _ \\| '__| |/ _ \\   / _` |/ _ \\ | | | |/ _ \\ \\/ / __/ _ \\ '__|\n");
	printf(" | |__| (_| | |_) | (_) | | | (_| | || (_) | |  | | (_) | | (_| |  __/ | |_| |  __/>  <| ||  __/ |   \n");
	printf(" |_____\\__,_|_.__/ \\___/|_|  \\__,_|\\__\\___/|_|  |_|\\___/   \\__,_|\\___| |____/ \\___/_/\\_\\\\__\\___|_|   \n");
}

/*
 * Pre condiciones: recibe un movimiento que es ARRIBA, ABAJO, IZQUIERDA o DERECHA
 *
 * Post condiciones: muestra por pantalla el movimiento recibido en rojo y el resto en amarillo
 */
void mostrar_movimiento_hecho(char movimiento)
{
	if(movimiento == ARRIBA)
	{
		printf(ROJO_CLARO"                                            ⇧ %c\n"FIN, ARRIBA);
		printf(AMARILLO"                                         ⇦ %c    %c ⇨\n"FIN, IZQUIERDA, DERECHA);
		printf(AMARILLO"                                            ⇩ %c\n"FIN, ABAJO);
	}
	else if(movimiento == IZQUIERDA)
	{
		printf(AMARILLO"                                            ⇧ %c\n"FIN, ARRIBA);
		printf(ROJO_CLARO"                                         ⇦ %c"FIN, IZQUIERDA);
		printf(AMARILLO"    %c ⇨\n"FIN, DERECHA);
		printf(AMARILLO"                                            ⇩ %c\n"FIN, ABAJO);
	}
	else if(movimiento == DERECHA)
	{
		printf(AMARILLO"                                            ⇧ %c\n"FIN, ARRIBA);
		printf(AMARILLO"                                         ⇦ %c"FIN, IZQUIERDA);
		printf(ROJO_CLARO"    %c ⇨\n"FIN, DERECHA);		
		printf(AMARILLO"                                            ⇩ %c\n"FIN, ABAJO);
	}
	else
	{
		printf(AMARILLO"                                            ⇧ %c\n"FIN, ARRIBA);
		printf(AMARILLO"                                         ⇦ %c    %c ⇨\n"FIN, IZQUIERDA, DERECHA);
		printf(ROJO_CLARO"                                            ⇩ %c\n"FIN, ABAJO);
	}	
}


/*
 * Pre condiciones: -
 *
 * Post condiciones: muestra por pantalla los movimientos con flechas indicando la dirección a la que corresponden
 */
void mostrar_movimientos_posibles()
{
	printf(AMARILLO"                                            ⇧ %c\n"FIN, ARRIBA);
	printf(AMARILLO"                                         ⇦ %c    %c ⇨\n"FIN, IZQUIERDA, DERECHA);
	printf(AMARILLO"                                            ⇩ %c\n"FIN, ABAJO);
}

/*
 * Pre condiciones: -
 * 
 * Post condiciones: muestra el mensaje de inicio del juego y las instrucciones de juego.
 */
void mostrar_mensaje_inicial()
{
	mostrar_msj_laboratorio_de_dexter();
	printf(ROJO_CLARO "\n\n¡¡Intruso detectado!! Activando sistemas de seguridad...\n" FIN);

	printf("¡Cuidado! En este laboratorio, las baldosas son resbaladizas.\n\n");

	printf(AMARILLO "INSTRUCCIONES:\n" FIN);
	printf("¡Oh no! Quedaste atrapado en el laboratorio. Este laboratorio tiene 4 pisos.\nInicias con 10 movimientos en el piso 1 y cada vez que pasas de nivel (piso) se suman 10 movimientos a los que te quedaban. Si te quedás sin movimientos perdés.\n");
	printf("Como las baldosas son resbaladizas solo podrás moverte de pared a pared.\n");
	printf("La salida del laboratorio se ubica en el último nivel. Para poder salir debes alcanzar la salida en el nivel 4.\n");
	printf("\n¿Qué hay en cada nivel?\n");
	printf("En cada nivel hay obstáculos que deberás sortear para alcanzar la salida y herramientas que serán útiles para llegar a la salida\n\n");
	
	printf(ROJO_CLARO "Obstáculos: \n" FIN);
	printf(" - Baldosas pinche (");
	printf(CELESTE_SUAVE "%c" FIN, B_PINCHE);
	printf("): si pisas una de estas baldosas perderás 2 movimientos.\n");

	printf(" - Guardia Robot (");
	printf(AZUL "%c" FIN, GUARDIA);
	printf("): si chocas con uno perderás todos tus movimientos. Además cada vez que te muevas se moverán a posiciones nuevas.\n");

	printf(" - Bomba (");
	printf(ROJO_CLARO "%c" FIN, BOMBA);
	printf("): al pisar la bomba se pierde automáticamente.\n\n");
	
	printf(VERDE_CLARO "Herramientas: \n" FIN);

	printf(" - Llave (");
	printf(MARRON "%c" FIN, LLAVE);
	printf("): deberás obtener la llave para poder pasar por la salida del nivel.\n"); 
	
	printf(" - Interruptor (");
	printf(CELESTE "%c" FIN, INTERRUPTOR);
	printf("): si en el nivel hay un interruptor deberás tocarlo para poder ver la llave.\n");

	printf(" - AlgoCoins (");
	printf(AMARILLO "%c" FIN, MONEDA);
	printf("): las monedas o AlgoCoins te sumarán 3 movimientos. Una vez que agarres una desaparecerá.\n");

	printf(" - Baldosas teletransportadoras (");
	printf(MAGENTA "%c" FIN, B_TELETRANSP);
	printf("): al pasar por una baldosa teletransportadora te teletransportarás automáticamente a otra baldosa \n \tteletransportadora\n\n");
	

	printf(" - %c", PERSONAJE);
	printf(": personaje. Habilidades disponibles:\n");
	printf("                                     - Blue y Bellota: estos personajes obtendrán 10 movimientos extra en caso de perder.\n");
	printf("                                     - Puro Hueso y Pollito: estos personajes no son afectados por las baldosas pinche.\n");
	printf("                                     - Coraje y Johnny Bravo: estos personajes no serán atrapados por los guardias.\n");

	printf("¿Cómo te moves?\n");
	printf(AMARILLO"       ⇧ %c\n"FIN, ARRIBA);
	printf(AMARILLO"    ⇦ %c    %c ⇨\n"FIN, IZQUIERDA, DERECHA);
	printf(AMARILLO"       ⇩ %c\n"FIN, ABAJO);
	
	printf("\n¡Buena suerte!\n\n");

	printf("Ingresa 'e' para empezar...\n");

}

/*
 * Pre condiciones: -
 *
 * Post condiciones: muestra por pantalla el mensaje "Felicidades! Has logrado escapar del laboratorio :)"
 */
void mostrar_mensaje_ganador()
{
	detener_el_tiempo((float) 0.01);
	printf("               _____    _ _      _     _           _           _                                \n");
	detener_el_tiempo((float) 0.01);
	printf("              |  ___|__| (_) ___(_) __| | __ _  __| | ___  ___| |                               \n");
	detener_el_tiempo((float) 0.01);
	printf("              | |_ / _ \\ | |/ __| |/ _` |/ _` |/ _` |/ _ \\/ __| |                               \n");
	detener_el_tiempo((float) 0.01);
	printf("              |  _|  __/ | | (__| | (_| | (_| | (_| |  __/\\__ \\_|                               \n");
	detener_el_tiempo((float) 0.01);
	printf("              |_|  \\___|_|_|\\___|_|\\__,_|\\__,_|\\__,_|\\___||___(_)                               \n");
	detener_el_tiempo((float) 0.01);
	printf("  _   _             _                           _                                               \n");
	detener_el_tiempo((float) 0.01);
	printf(" | | | | __ _ ___  | | ___   __ _ _ __ __ _  __| | ___     ___  ___  ___ __ _ _ __   __ _ _ __  \n");
	detener_el_tiempo((float) 0.01);
	printf(" | |_| |/ _` / __| | |/ _ \\ / _` | '__/ _` |/ _` |/ _ \\   / _ \\/ __|/ __/ _` | '_ \\ / _` | '__| \n");
	detener_el_tiempo((float) 0.01);
	printf(" |  _  | (_| \\__ \\ | | (_) | (_| | | | (_| | (_| | (_) | |  __/\\__ \\ (_| (_| | |_) | (_| | |    \n");
	detener_el_tiempo((float) 0.01);
	printf(" |_| |_|\\__,_|___/ |_|\\___/ \\__, |_|  \\__,_|\\__,_|\\___/   \\___||___/\\___\\__,_| .__/ \\__,_|_|    \n");
	detener_el_tiempo((float) 0.01);
	printf("      _      _   _       _  |___/              _             _             __|_|                \n");
	detener_el_tiempo((float) 0.01);
	printf("   __| | ___| | | | __ _| |__   ___  _ __ __ _| |_ ___  _ __(_) ___     _  \\ \\                  \n");
	detener_el_tiempo((float) 0.01);
	printf("  / _` |/ _ \\ | | |/ _` | '_ \\ / _ \\| '__/ _` | __/ _ \\| '__| |/ _ \\   (_)  | |                \n");
	detener_el_tiempo((float) 0.01); 
	printf(" | (_| |  __/ | | | (_| | |_) | (_) | | | (_| | || (_) | |  | | (_) |   _   | |             \n");
	detener_el_tiempo((float) 0.01);    
	printf("  \\__,_|\\___|_| |_|\\__,_|_.__/ \\___/|_|  \\__,_|\\__\\___/|_|  |_|\\___/   (_)  | |        \n");
	detener_el_tiempo((float) 0.01);         
	printf("                                                                           /_/     \n");
}

/*
 * Pre condiciones: -
 *
 * Post condiciones: muestra por pantalla el mensaje de "GAME OVER" con una calavera
 */
void mostrar_mensaje_perdedor()
{
	detener_el_tiempo((float) 0.03);
	printf("                                    \n");
	detener_el_tiempo((float) 0.03);
	printf("███▀▀▀██ ███▀▀▀███ ███▀█▄█▀███ ██▀▀▀\n");
	detener_el_tiempo((float) 0.03);
	printf("██    ██ ██     ██ ██   █   ██ ██   \n");
	detener_el_tiempo((float) 0.03);
	printf("██   ▄▄▄ ██▄▄▄▄▄██ ██   ▀   ██ ██▀▀▀\n");
	detener_el_tiempo((float) 0.03);
	printf("██    ██ ██     ██ ██       ██ ██   \n");
	detener_el_tiempo((float) 0.03);
	printf("███▄▄▄██ ██     ██ ██       ██ ██▄▄▄\n");
	detener_el_tiempo((float) 0.03);
	printf("                                    \n");
	detener_el_tiempo((float) 0.03);
	printf("███▀▀▀███ ▀███  ██▀ ██▀▀▀ ██▀▀▀▀██▄ \n");
	detener_el_tiempo((float) 0.03);
	printf("██     ██   ██  ██  ██    ██     ██ \n");
	detener_el_tiempo((float) 0.03);
	printf("██     ██   ██  ██  ██▀▀▀ ██▄▄▄▄▄▀▀ \n");
	detener_el_tiempo((float) 0.03);
	printf("██     ██   ██  █▀  ██    ██     ██ \n");
	detener_el_tiempo((float) 0.03);
	printf("███▄▄▄███    ▀█▀    ██▄▄▄ ██     ██▄\n");
	detener_el_tiempo((float) 0.03);
	printf("                                    \n");
	detener_el_tiempo((float) 0.03);
	printf("        ██               ██         \n");
	detener_el_tiempo((float) 0.03);
	printf("      ████▄   ▄▄▄▄▄▄▄   ▄████       \n");
	detener_el_tiempo((float) 0.03);
	printf("         ▀▀█▄█████████▄█▀▀          \n");
	detener_el_tiempo((float) 0.03);
	printf("           █████████████            \n");
	detener_el_tiempo((float) 0.03);
	printf("           ██▀▀▀███▀▀▀██            \n");
	detener_el_tiempo((float) 0.03);
	printf("           ██   ███   ██            \n");
	detener_el_tiempo((float) 0.03);
	printf("           █████▀▄▀█████            \n");
	detener_el_tiempo((float) 0.03);
	printf("            ███████████             \n");
	detener_el_tiempo((float) 0.03);
	printf("        ▄▄▄██  █▀█▀█  ██▄▄▄         \n");
	detener_el_tiempo((float) 0.03);
	printf("        ▀▀██           ██▀▀         \n");
	detener_el_tiempo((float) 0.03);
	printf("          ▀▀           ▀▀           \n");
	detener_el_tiempo((float) 0.03);
	printf("                                    \n");
}

/*
 * Pre condiciones: recibe un entero que es JUEGO_GANADO, JUEGO_PERDIDO o JUEGO_JUGANDO
 *
 * Post condiciones: muestra por pantalla el mensaje ganador si el entero es JUEGO_GANADO o el mensaje perdedor si el entero es JUEGO_PERDIDO
 */
void mostrar_mensaje_final(int estado_del_juego)
{
	
	if(estado_del_juego == JUEGO_GANADO)
	{
		detener_el_tiempo((float) 1);
		system("clear");
		mostrar_mensaje_ganador();		
	}
	else
	{
		detener_el_tiempo((float) 1);
		system("clear");
		mostrar_mensaje_perdedor();
	}
}

/*
 * Pre condiciones: recibe un entero que puede ser 1, 2, 3 o 4
 *
 * Post condiciones: muestra por pantalla "- nivel -" con el número de nivel que recibió
 */
void mostrar_msj_nivel(int nivel)
{	
	if(nivel == 1)
	{
		printf("		                         _           _     _            \n");
		printf("		                   _ __ (_)_   _____| |   / |           \n");
		printf("		         _____    | '_ \\| \\ \\ / / _ \\ |   | |     _____ \n");
		printf("		        |_____|   | | | | |\\ V /  __/ |   | |    |_____|\n");
		printf("		                  |_| |_|_| \\_/ \\__ |_|   |_|           \n");	
	}
	if(nivel == 2)
	{
		printf("		                         _           _     ____             \n");
		printf("		                   _ __ (_)_   _____| |   |___ \\            \n");
		printf("		         _____    | '_ \\| \\ \\ / / _ \\ |     __) |     _____ \n");
		printf("		        |_____|   | | | | |\\ V /  __/ |    / __/     |_____|\n");
		printf("		                  |_| |_|_| \\_/ \\___|_|   |_____|           \n");
	}
	else if(nivel == 3)
	{
		printf("		                         _           _     _____            \n");
		printf("		                   _ __ (_)_   _____| |   |___ /            \n");
		printf("		         _____    | '_ \\| \\ \\ / / _ \\ |     |_ \\      _____ \n");
		printf("		        |_____|   | | | | |\\ V /  __/ |    ___) |    |_____|\n");
		printf("		                  |_| |_|_| \\_/ \\___|_|   |____/            \n");
	}
	else if (nivel == 4)
	{
		printf("		                         _           _     _  _              \n");
		printf("		                   _ __ (_)_   _____| |   | || |             \n");
		printf("		         _____    | '_ \\| \\ \\ / / _ \\ |   | || |_      _____ \n");
		printf("		        |_____|   | | | | |\\ V /  __/ |   |__   _|    |_____|\n");
		printf("		                  |_| |_|_| \\_/ \\___|_|      |_|             \n");
	}

}

/*
 * Pre condiciones: -
 *
 * Post condiciones: muestra por pantalla "Siguiente nivel" 
 */
void mostrar_msj_nuevo_nivel()
{
	mostrar_msj_laboratorio_de_dexter();
	printf("		    ____  _             _            _               _           _ \n");
	printf("		   / ___|(_) __ _ _   _(_) ___ _ __ | |_ ___   _ __ (_)_   _____| |\n");
	printf("		   \\___ \\| |/ _` | | | | |/ _ \\ '_ \\| __/ _ \\ | '_ \\| \\ \\ / / _ \\ |\n");
	printf("		    ___) | | (_| | |_| | |  __/ | | | ||  __/ | | | | |\\ V /  __/ |\n");
	printf("		   |____/|_|\\__, |\\__,_|_|\\___|_| |_|\\__\\___| |_| |_|_| \\_/ \\___|_|\n");
	printf("		            |___/                                                  \n");
	detener_el_tiempo((float) 0.75);
}

/*
 * Pre condiciones: recibe un caracter por referencia
 *
 * Post condiciones: devuelve el caracter cargado con lo que se leyó por teclado si fue ARRIBA, ABAJO, DERECHA o IZQUIERDA
 */
void obtener_movimiento(char *movimiento)
{
	printf("Ingrese movimiento: ");
	scanf(" %c", movimiento);
	printf("\n");
	while(!es_movimiento_valido(*movimiento))
	{
		printf("Movimiento invalido. Debe ingresar %c, %c, %c o %c: ", ARRIBA, ABAJO, DERECHA, IZQUIERDA);
		scanf(" %c", movimiento);
		printf("\n");
	}
}

/*
 * Pre condiciones: recibe un juego_t por referencia
 *
 * Post condiciones: devuelve el juego actualizado
 */
int jugar(juego_t *juego)
{
	int estado_del_juego = estado_juego(*juego);
	char movimiento;
	while(estado_del_juego == JUEGO_JUGANDO)
	{
		obtener_movimiento(&movimiento);

		mover_personaje(juego, movimiento);

		system("clear");

		mostrar_msj_laboratorio_de_dexter();
		mostrar_movimiento_hecho(movimiento);
		mostrar_juego(*juego);

		if( (estado_nivel((*juego).personaje, (*juego).niveles[(*juego).nivel_actual-1].salida) == NIVEL_GANADO) &&
			((*juego).nivel_actual < 4) )
		{
			(*juego).nivel_actual++;
			
			(*juego).personaje.tiene_llave = false;
			(*juego).personaje.presiono_interruptor = false;
			(*juego).personaje.posicion = (*juego).niveles[(*juego).nivel_actual-1].entrada;

			detener_el_tiempo((float) 0.65);

			system("clear");

			mostrar_msj_nuevo_nivel();
			detener_el_tiempo((float) 0.40);
			
			mostrar_msj_nivel((*juego).nivel_actual);

			detener_el_tiempo((float) 0.75);

			printf("                         -    Te sobraron %i movimientos    -\n", (*juego).personaje.movimientos);
			detener_el_tiempo((float) 2.05);

			(*juego).personaje.movimientos += 10;

			system("clear");

			mostrar_msj_laboratorio_de_dexter();
			mostrar_movimientos_posibles();
			mostrar_juego(*juego);
		}
		estado_del_juego = estado_juego(*juego);
	}
	return estado_del_juego;
}


int main()
{
	srand((unsigned)time(NULL));

	juego_t juego;
	char accion;
	int estado_del_juego;
	char personaje;

	system("clear");

	mostrar_msj_laboratorio_de_dexter();

	detectar_personaje(&personaje);
	detener_el_tiempo((float) 2);

	system("clear");

	mostrar_mensaje_inicial();

	scanf(" %c", &accion);
	printf("\n");
	while(accion != 'e')
	{
		printf("Ingresa 'e' para empezar...\n");
		scanf(" %c", &accion);
	}


	inicializar_juego(&juego, personaje);

	juego.personaje.posicion = juego.niveles[juego.nivel_actual-1].entrada;

	system("clear");
	mostrar_msj_laboratorio_de_dexter();
	mostrar_msj_nivel(juego.nivel_actual);
	detener_el_tiempo((float) 1.5);

	system("clear");

	mostrar_msj_laboratorio_de_dexter();
	mostrar_movimientos_posibles();
	mostrar_juego(juego);

	estado_del_juego = jugar(&juego);

	mostrar_mensaje_final(estado_del_juego);


	return 0;
}