
#ifndef JUEGO_H
#define JUEGO_H

#include "mapas.h"
#include <stdbool.h>
typedef struct Juego {
    // Posicion del jugador
    int posX; 
    int posY;

    Mapa *mapa;
    int nivel; // Nivel actual
    float puntaje[3]; // Puntaje del jugador en cada nivel
    int monedas[3]; // Monedas recolectadas en cada nivel
    int monedasTotales[3]; // MOnedas totales en cada nivel
    bool llave; // Bandera de llave por nivel
    int pasos[3]; // Contador de pasos en cada nivel

    bool fin;

}Juego;

void iniciarJuego(Juego *juego);

void mostrarMapa(Juego* juego);
void cargarMapa(Juego* juego);
void cambiarNivel(Juego* juego);
void moverJugador(Juego* juego, char direccion);
void mostrarStatus(Juego* juego);
void mostrarResumenJuego(Juego* juego);
void mostrarResumenNivel(Juego* juego);

//Funciones NASM
bool validarMovimiento(char **mapa, int posX, int posY, int newX, int newY, bool llave);
int contarCaracter(char** linea, int longitud, char lt); // Contar monedas por nivel
bool detectarObjeto( char** mapa, int col, int posX, int posY, char buscar);
int contarCeldasLibres( char* mapa, int celdasTotales );
float calcularPuntaje(float colectCoins, float totalCoins, float pasos, float multiplicador);

#endif
