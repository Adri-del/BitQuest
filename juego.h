
#ifndef JUEGO_H
#define JUEGO_H

#include "mapas.h"
#include <stdbool.h>

// Estructura principal del juego
typedef struct Juego {
    int posX; 
    int posY;

    Mapa *mapa;
    int nivel; // Nivel actual
    float puntaje[4]; // Puntaje del jugador en cada nivel
    int monedas[4]; // Monedas recolectadas en cada nivel
    int monedasTotales[4]; // MOnedas totales en cada nivel
    bool llave; // Bandera de llave por nivel
    int pasos[4]; // Contador de pasos en cada nivel

    bool fin;

}Juego;

// Inicializacion del juego
void iniciarJuego(Juego *juego);

// Funciones logicas en C
void mostrarMapa(Juego* juego); // Muestra la ventana visible
void cargarMapa(Juego* juego); // Carga el mapa del nivel desde un archivo
void cambiarNivel(Juego* juego); // Gestiona la transición al siguiente nivel
void moverJugador(Juego* juego, char direccion); // Procesa las interacciones del jugador

// Funciones de interfaz y estadísticas en C
void mostrarStatus(Juego* juego); // Muestra el estado actual de la partida 
void mostrarResumenJuego(Juego* juego); // Muetsra el resumen final del juego
void mostrarResumenNivel(Juego* juego); // Muestra el resumen al completar un nivel
void pantallaInicio(); // Muetsra la pantalla de bienvenida del juego

//Rutinas en NASM

// Valida si el movimiento es valido
bool validarMovimiento(char **mapa, int posX, int posY, int newX, int newY, bool llave); 

// Cuenta cuantas veces aparece un caracter a lo largo del mapa (contar las monedas en u mapa)
int contarCaracter(char** mapa, int filas, char lt);

// Devuelve 1 si el objeto se encuentra en esa posicion o 0 si no es el caso
bool detectarObjeto( char** mapa, int col, int posX, int posY, char buscar);

// Duvuelve la cantidad de casillas libres en un mapa
int contarCeldasLibres( char** mapa, int celdasTotales );

// Calcula el puntaje obtenido en un nivel
float calcularPuntaje(float colectCoins, float totalCoins, float pasos, float nivel);

#endif
