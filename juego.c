// Funciones del juego

#include "mapas.h"
#include "juego.h"
#include <stdbool.h>
#include <stdio.h>

void mostrarMapa(int posX, int posY, const Mapa* mapa) { // Mostramos el mapa
    if (posX < 0 || posX >= mapa->tamMap || posY < 0 || posY >= mapa->tamMap)  return;

    // Vista de 20*20
    int left = posX - (mapa->tamView / 2);
    int above = posY - (mapa->tamView / 2);
    int right = posX + (mapa->tamView / 2) -1;
    int down = posY + (mapa->tamView / 2) -1;

    for (int y = above; y <= down; y++) {
        for (int x = left; x <= right; x++) {
            if (x < 0 || x >= mapa->tamMap || y < 0 || y >= mapa->tamMap) { // Ignoramos casillas fuera del mapa
                continue;
            } else if (x == posX && y == posY) { // Posicion del jugador
                printf("P");
            } else {
                printf("%c", mapa->mapa[y][x]); // Imprimimos el mapa
            }
        }
        printf("\n");
    } 
}

void cargarMapa(Juego* juego, Mapa* mapa) { // Cargar el mapa del nivel actual
    switch (juego->nivel) {
        case 1: // Nivel 1
            for(int i = 0; i < mapa->tamMap; i++) {
                for(int j = 0; j < mapa->tamMap; j++) {
                    mapa->mapa[i][j] = nivel1[i][j];
                    for(int y = 0; y < mapa->tamMap; y++) {
                        for(int x = 0; x < mapa->tamMap; x++) {
                            // Buscamos la posicion del jugador
                        }
                    }
                }
            }
            break;
        case 2: // Nivel 2
            for(int i = 0; i < mapa->tamMap; i++) {
                for(int j = 0; j < mapa->tamMap; j++) {
                    mapa->mapa[i][j] = nivel2[i][j];
                }
            }
               break;
        case 3: // Nivel 3
            for(int i = 0; i < mapa->tamMap; i++) {
                for(int j = 0; j < mapa->tamMap; j++) {
                    mapa->mapa[i][j] = nivel3[i][j];
                }
            }
            break; 
        default:
            break;
    }
}

void cambiarNivel(Juego* juego, Mapa* mapa) {
    if(juego->nivel < 3) { // Vamos al siguiente nivel
        mostrarResumenNivel(juego);
        juego->nivel++; 
        juego->llave = false; // Reseatemos bandera de llave
        juego->pasos[juego->nivel - 1] = 0; // Reseatemos contador de pasos
        cargarMapa(juego, mapa);
    } else { // Fin del juego
        mostrarResumenJuego(juego);
    }
}

void moverJugador(Juego* juego, char direccion, Mapa* mapa) {
    // Nueva posible posicion
    int newX = juego->posX;
    int newY = juego->posY;

    switch (direccion) { // Hacia donde se mueve el jugador
        // Arriba
        case 'w': newY--; break;
        case 'W': newY--; break;
        // Izquierda
        case 'a': newX--; break;
        case 'A': newX--; break;
        // Abajo
        case 's': newY++; break;
        case 'S': newY++; break;
        // Derecha
        case 'd': newX++; break;
        case 'D': newX++; break;
        case 'q': juego->posX = -1; 
                    mostrarResumenJuego(juego);
                    return; 
            break;
        case 'Q': juego->posX = -1; 
                    mostrarResumenJuego(juego);
                    return; 
            break;
        default: return; 
    }

    // Cambiar por funcion de validacion de movimiento
    // Tambien verifica si el jugador tiene la llave
    if (newX < 0 || newX >= mapa->tamMap || newY < 0 || newY >= mapa->tamMap) {
        return; 
    }

    char casilla = mapa->mapa[newY][newX]; // Contenido de la siguiente casilla
    switch(casilla) {
        case 'D': if(juego->llave) { // Por si ocurrio un error en la validacion de movimiento
                cambiarNivel(&juego->nivel, mapa); // Vamos al siguiente nivel
            }
        case 'M': // Aumenta el contador de monedas
            juego->monedas[juego->nivel - 1]++;
        case 'K': // Aumenta el contador de llaves
            juego->llave = true;
        case '.': // Casilla vacia, nos movemos
            juego->posX = newX;
            juego->posY = newY;
            break;
        default: break; // Situacion anomala
    }
}

void mostrarResumenJuego(Juego* juego) {
    printf("\n======================================\n");
    printf("Juego Completado\n");
    printf("Monedas totales recolectadas: %d / %d\n", juego->monedas[0] + juego->monedas[1] + juego->monedas[2],
                                                        juego->monedasTotales[0] + juego->monedasTotales[1] + juego->monedasTotales[2]);
    printf("Pasos Totales: %d\n", juego->pasos[0] + juego->pasos[1] + juego->pasos[2]);
    printf("Niveles completados: %d / 3\n" , juego->nivel);
    printf("Puntaje Final: %d\n", juego->puntaje[0] + juego->puntaje[1] + juego->puntaje[2]);
    printf("\n======================================\n");
}

void mostrarResumenNivel(Juego *juego) {
    printf("\n======================================\n");
    printf("Nivel: Completado\n");
    printf("Monedas: %d / %d\n", juego->monedas[juego->nivel - 1], juego->monedasTotales[juego->nivel - 1]);
    printf("Pasos: %d\n", juego->pasos[juego->nivel - 1]);
    printf("Puntaje: %d\n", juego->puntaje[juego->nivel - 1]);
    printf("\n======================================\n");
}

void mostrarStatus(Juego* juego) {
    printf("Nivel: %d\n", juego->nivel);
    printf("Llave: %s\n", juego->llave ? "Sí" : "No");
    printf("Pasos: %d\n", juego->pasos[juego->nivel - 1]);
    printf("Monedas: %d / %d\n", juego->monedas[juego->nivel - 1], juego->monedasTotales[juego->nivel - 1]);
    printf("Puntaje: %d\n", juego->puntaje[juego->nivel - 1]);
}