// Funcion principal, menuu inicial y control general del juego

#include "juego.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    Juego* juego = malloc(sizeof(Juego));
    iniciarJuego(juego);
    char tecla;
    do{
        mostrarMapa(juego);
        scanf("%c", &tecla);
        moverJugador(juego, tecla);
    }while(juego->fin == false);
}