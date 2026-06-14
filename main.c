// Funcion principal, menuu inicial y control general del juego

#include "juego.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// Funcion principal del programa
int main() {
    Juego* juego = malloc(sizeof(Juego));

    // Inicializamos las variables y estructuras de la partida
    iniciarJuego(juego);

    char tecla;
    pantallaInicio();

    while(true){
        // Finaliza el ciclo si la bandera fin es verdadera
        // Fin del juego
        if(juego->fin == true) break;

        // Limpiamos la impresion del mapa anterior
        system("cls"); 

        // Mostramos las estadisticas del juego en la parte superior de la pantalla
        mostrarStatus(juego);
        mostrarMapa(juego);

        // Esperamos el siguiente movimiento del jugador
        tecla = getch();
        moverJugador(juego, tecla);
    }
}