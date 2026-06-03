// Funciones del juego
#include "juego.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void iniciarJuego(Juego* juego) {
    juego->mapa = malloc(sizeof(Mapa));
    juego->mapa->tamMap = 60;
    juego->mapa->tamView = 20;
    juego->mapa->mapa = malloc(juego->mapa->tamMap*sizeof(char*));
    for(int i = 0; i < juego->mapa->tamMap; i++) {
        juego->mapa->mapa[i] = malloc(60 * sizeof(char));
    }
    juego->fin = false;
    juego->nivel = 1;
    juego->llave = false;

    for(int i = 0; i < 3; i++) 
        juego->pasos[i] = 0;

    for(int i = 0; i < 3; i++) 
        juego->monedas[i] = 0;

    for(int i = 0; i < 3; i++) 
        juego->monedasTotales[i] = 0;

    for(int i = 0; i < 3; i++) 
        juego->puntaje[i] = 0;
    
    
    cargarMapa(juego);
}

void mostrarMapa(Juego* juego) { // Mostramos el mapa
    if (juego->posX < 0 || juego->posX >= juego->mapa->tamMap || 
        juego->posY < 0 || juego->posY >= juego->mapa->tamMap)  return;

    // Vista de 20*20
    int left = juego->posX - (juego->mapa->tamView / 2);
    int above = juego->posY - (juego->mapa->tamView / 2);
    int right = juego->posX + (juego->mapa->tamView / 2) -1;
    int down = juego->posY + (juego->mapa->tamView / 2) -1;

    for (int y = above; y <= down; y++) {
        for (int x = left; x <= right; x++) {
            if (x < 0 || x >= juego->mapa->tamMap || y < 0 || y >= juego->mapa->tamMap) { // Ignoramos casillas fuera del mapa
                continue;
            } else if (x == juego->posX && y == juego->posY) { // Posicion del jugador
                printf("P");
            } else {
                printf("%c", juego->mapa->mapa[y][x]); // Imprimimos el mapa
            }
        }
        printf("\n");
    } 
}

void cargarMapa(Juego* juego) { // Cargar el mapa del nivel actual
    FILE* file;
    switch (juego->nivel) {
        case 1: // Nivel 1
            file = fopen("nivel1.txt", "r");
            break;
        case 2: // Nivel 2
            file = fopen("nivel2.txt", "r");
               break;
        case 3: // Nivel 3
            file = fopen("nivel3.txt", "r");
            break; 
        default:
            break;
    }

    if(!file) {
        printf("Error Archivo: nivel.txt");
        juego->fin = true;
        return;
    }
    
    char buffer[100];
    for(int i = 0; i < juego->mapa->tamMap; i++) {
        if(!fgets(buffer, sizeof(buffer), file)) break;
        for(int j = 0; j < juego->mapa->tamMap; j++) {
            char c = buffer[j];
            juego->mapa->mapa[i][j] = c;

            if(c == 'P') {
                juego->posX = j;
                juego->posY = i;
                continue;
            }

            if(c == 'M') juego->monedasTotales[juego->nivel-1]++;

        }
    }

}

void cambiarNivel(Juego* juego) {
    if(juego->nivel < 3) { // Vamos al siguiente nivel
        float nivel = (float)juego->nivel;
        float contCoins = (float)juego->monedas[juego->nivel-1];
        float totalCoins = (float)juego->monedasTotales[juego->nivel-1];
        float pasos = (float)juego->pasos[juego->nivel-1];

        // Calculamos el puntaje
        juego->puntaje[juego->nivel-1] = ((contCoins/totalCoins)/pasos) * nivel * 10000;

        mostrarResumenNivel(juego);
        juego->nivel++; 
        juego->llave = false; // Reseatemos bandera de llave
        juego->pasos[juego->nivel - 1] = 0; // Reseatemos contador de pasos
        cargarMapa(juego);
    } else { // Fin del juego
        mostrarResumenJuego(juego);
        juego->fin = true;
    }
}

void moverJugador(Juego* juego, char direccion) {
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
        case 'q': 
                juego->posX = -1; 
                mostrarResumenJuego(juego);
                juego->fin = true;
                return; 
                break;
        case 'Q': 
                juego->posX = -1; 
                mostrarResumenJuego(juego);
                juego->fin = true;
                return; 
            break;
        default: return; 
    }

    // Tambien verifica si el jugador tiene la llave
    if (newX>=60 || newX < 0 || newY>=60 ||newY<0)
        return; 

    char casilla = juego->mapa->mapa[newY][newX]; // Contenido de la siguiente casilla
    switch (casilla)
    {
    case 'M':
        juego->monedas[juego->nivel-1]++;
        juego->mapa->mapa[newY][newX] = '.';
        break;
        
    case 'D':
        juego->mapa->mapa[newY][newX] = 'E';
        return;
        break;
        
    case '.':
        break;
        
    case '#':
        return;
        break;

    case 'E':
        cambiarNivel(juego);
        break;
        
    case 'K':
        juego->llave = true;
        juego->mapa->mapa[newY][newX] = '.';
        break;
    
    default: return;
        break;
    }
    juego->mapa->mapa[juego->posY][juego->posX] = '.';
    juego->posX = newX;
    juego->posY = newY;

    /*
    if(detectarObjeto(juego->mapa->mapa, juego->mapa->tamMap, newX, newY, 'D')) {
        if(juego->llave) // Por si ocurrio un error en la validacion de movimiento
                    cambiarNivel(juego); // Vamos al siguiente nivel
    }

    if(detectarObjeto(juego->mapa->mapa, juego->mapa->tamMap, newX, newY, 'M')) 
        juego->monedas[juego->nivel - 1]++; // Aumenta el contador de monedas
    
    if(detectarObjeto(juego->mapa->mapa, juego->mapa->tamMap, newX, newY, 'K')) 
        juego->monedas[juego->nivel - 1]++; // Aumenta el contador de llaves
    
    if(detectarObjeto(juego->mapa->mapa, juego->mapa->tamMap, newX, newY, '.')) { // Casilla vacia, nos movemos
        juego->posX = newX;
        juego->posY = newY;
    }*/
}

void mostrarResumenJuego(Juego* juego) {
    printf("\n======================================\n");
    printf("Juego Completado\n");
    printf("Monedas totales recolectadas: %d / %d\n", juego->monedas[0] + juego->monedas[1] + juego->monedas[2],
                                                        juego->monedasTotales[0] + juego->monedasTotales[1] + juego->monedasTotales[2]);
    printf("Pasos Totales: %d\n", juego->pasos[0] + juego->pasos[1] + juego->pasos[2]);
    printf("Niveles completados: %d / 3\n" , juego->nivel);
    printf("Puntaje Final: %d\n", (int)(juego->puntaje[0] + juego->puntaje[1] + juego->puntaje[2]));
    printf("\n======================================\n");
}

void mostrarResumenNivel(Juego *juego) {
    printf("\n======================================\n");
    printf("Nivel: Completado\n");
    printf("Monedas: %d / %d\n", juego->monedas[juego->nivel - 1], juego->monedasTotales[juego->nivel - 1]);
    printf("Pasos: %d\n", juego->pasos[juego->nivel - 1]);
    printf("Puntaje: %d\n", (int)(juego->puntaje[juego->nivel - 1]));
    printf("\n======================================\n");
}

void mostrarStatus(Juego* juego) {
    printf("Nivel: %d\n", juego->nivel);
    printf("Llave: %s\n", juego->llave ? "Sí" : "No");
    printf("Pasos: %d\n", juego->pasos[juego->nivel - 1]);
    printf("Monedas: %d / %d\n", juego->monedas[juego->nivel - 1], juego->monedasTotales[juego->nivel - 1]);
}

