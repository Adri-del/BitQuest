
#ifndef JUEGO_H
#define JUEGO_H

#include "mapas.h"
#include <stdbool.h>
typedef struct Juego {
    // Posicion del jugador
    int posX; 
    int posY;

    int nivel; // Nivel actual
    float puntaje[3]; // Puntaje del jugador en cada nivel
    int monedas[3]; // Monedas recolectadas en cada nivel
    int monedasTotales[3]; // MOnedas totales en cada nivel
    bool llave; // Bandera de llave por nivel
    int pasos[3]; // Contador de pasos en cada nivel

}Juego;

void cargarMapa(struct Juego* juego, struct Mapa* mapa);
void cambiarNivel(struct Juego* juego, struct Mapa* mapa);
void moverJugador(struct Juego* juego, char direccion, struct Mapa* mapa);
void mostrarStatus(struct Juego* juego);
void mostrarResumenJuego(struct Juego* juego);
void mostrarResumenNivel(struct Juego* juego);

//Funciones NASM
bool validarMovimiento(char **mapa, int posX, int posY, int newX, int newY, bool llave);
int contarCaracter(char* linea, int longitud, char lt); // Contar monedas por nivel
bool detectarObjeto( char** mapa, int col, int posX, int posY, char buscar);
int contarCeldasLibres( char* mapa, int celdasTotales );
float calcularPuntaje(float colectCoins, float totalCoins, float pasos, float multiplicador);

#endif
