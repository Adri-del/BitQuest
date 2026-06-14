// Declaracion de los mapas de cada nivel
#ifndef MAPAS_H
#define MAPAS_H

#include <stdio.h>
typedef struct Mapa {
    char** mapa; // Matriz del mapa
    int tamMap; // Tamaño del mapa
    int tamView; // Tamaño de la vista del mapa
}Mapa;

#endif
