// Funciones del juego
#include "juego.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

// Inicializa las estructuras y variables para una nueva partita
void iniciarJuego(Juego* juego) {
    juego->mapa = malloc(sizeof(Mapa));
    juego->mapa->tamMap = 60; // Mapa de 60*60
    juego->mapa->tamView = 20; // Vista de 20*20
    juego->mapa->mapa = malloc(juego->mapa->tamMap*sizeof(char*));
    for(int i = 0; i < juego->mapa->tamMap; i++) {
        juego->mapa->mapa[i] = malloc(60 * sizeof(char));
    }
    juego->fin = false; // Bandera que marca el final del juego
    juego->nivel = 1; // Empezamos por el primer nivel
    juego->llave = false; // Bandera que marca el estado de la llave

    for(int i = 0; i < 4; i++) // Contenedores de los pasos dados
        juego->pasos[i] = 0;

    for(int i = 0; i < 4; i++) // Contenedores de las monedas
        juego->monedas[i] = 0;

    for(int i = 0; i < 4; i++) // Contenedores de monedas totales
        juego->monedasTotales[i] = 0;

    for(int i = 0; i < 4; i++) // Contenedores de los puntajes
        juego->puntaje[i] = 0;
    
    cargarMapa(juego); // Carga el mapa (en este primer caso: nivel 1)
}

// Muestra la ventana visible del mapa centrada en el jugador
void mostrarMapa(Juego* juego) {
    // Verifica que la posición del jugador sea valida
    if (juego->posX < 0 || juego->posX >= juego->mapa->tamMap || 
        juego->posY < 0 || juego->posY >= juego->mapa->tamMap)  return;

    // Calcula los limites de la ventana visible
    int left = juego->posX - (juego->mapa->tamView / 2);
    int above = juego->posY - (juego->mapa->tamView / 2);
    int right = juego->posX + (juego->mapa->tamView / 2) -1;
    int down = juego->posY + (juego->mapa->tamView / 2) -1;

    for (int y = above; y <= down; y++) {
        for (int x = left; x <= right; x++) {
            // Ignoramos casillas fuera del mapa
            if (x < 0 || x >= juego->mapa->tamMap || y < 0 || y >= juego->mapa->tamMap)
                continue;

            // Mostramos la posicion del jugador
            if (x == juego->posX && y == juego->posY) { // Posicion del jugador
                printf("\033[91mP\033[0m"); // Jugador (Rojo)
                continue;
            }
            
            // Muestra cada elemento del mapa
            switch(juego->mapa->mapa[y][x]) {
                case '#': // Pared (Gris)
                    printf("\033[90m#\033[0m");
                    break;

                case 'M': // Moneda (Amarillo)
                    printf("\033[93mM\033[0m");
                    break;

                case 'K': // Llave (Cian)
                    printf("\033[96mK\033[0m");
                    break;

                case 'D': // Puerta (Verde)
                    printf("\033[92mD\033[0m");
                    break;
                case '.' : // Casilla libre (Blanco)
                    printf("\033[97m.\033[0m");
                    break;
                case 'E': // Salida (Morado)
                    printf("\033[95mE\033[0m");
                    break;

                default: break;
            }
        }
        printf("\n");
    } 
}

// Carga el mapa desde un archivo
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
        case 4: // Nivel 4
            file = fopen("nivel4.txt", "r");
        default:
            break;
    }

    if(!file) {
        printf("Error Archivo: nivel.txt");
        juego->fin = true;
        return;
    }
    
    char buffer[100];
    // Guarda e contenido del archivo en la matriz mapa
    for(int i = 0; i < juego->mapa->tamMap; i++) {
        if(!fgets(buffer, sizeof(buffer), file)) break;
        for(int j = 0; j < juego->mapa->tamMap; j++) {
            char c = buffer[j];
            juego->mapa->mapa[i][j] = c;

            if(c == 'P') { // Posicion inicial del jugador
                juego->posX = j;
                juego->posY = i;
                continue;
            }
        }
    }

    // Cuenta las monedas por nivel
    juego->monedasTotales[juego->nivel-1] = contarCaracter(juego->mapa->mapa, juego->mapa->tamMap, 'M');
    fclose(file);
}

// Calcula el puntaje del nivel y realiza la transicion al siguiente nivel
void cambiarNivel(Juego* juego) {
    // Convierte los datos a float para calcular el puntaje
    float nivel = (float)juego->nivel;
    float colectCoins = (float)juego->monedas[juego->nivel-1];
    float totalCoins = (float)juego->monedasTotales[juego->nivel-1];
    float pasos = (float)juego->pasos[juego->nivel-1];

    // Calcula y almacena el puntaje obtenido en el nivel
    juego->puntaje[juego->nivel-1] = calcularPuntaje(colectCoins, totalCoins, pasos, nivel);

    mostrarResumenNivel(juego);

    if(juego->nivel < 4) { // Vamos al siguiente nivel
        juego->nivel++; // Siguiente nivel
        juego->llave = false; // Reseatemos bandera de llave
        juego->pasos[juego->nivel - 1] = 0; // Reseatemos contador de pasos

        cargarMapa(juego);

    } else { // Fin del juego
        juego->nivel++;
        mostrarResumenJuego(juego);
        juego->fin = true; // Marca que el juego ha finalizado
    }
}

// Procesa el movimiento del jugador 
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
        // Salir del juego
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


    // Devuelve true si el movmiento es valido
    if (!validarMovimiento(juego->mapa->mapa, juego->posX, juego->posY, newX, newY, juego->llave))
        return; 

    // Si el jugador llega a la puerta con la llave
    if(detectarObjeto(juego->mapa->mapa, juego->mapa->tamMap, newX, newY, 'D')){
        juego->mapa->mapa[newY][newX] = 'E'; // La puerta se vuelve la salida
        return;
    }

    // Si se encuentra una moneda, aumenta el contador de monedas
    if(detectarObjeto(juego->mapa->mapa, juego->mapa->tamMap, newX, newY, 'M'))
        juego->monedas[juego->nivel - 1]++; // Aumenta el contador de monedas

    // Si se encuentra una llave actualiza la bandera llave
    if(detectarObjeto(juego->mapa->mapa, juego->mapa->tamMap, newX, newY, 'K'))
        juego->llave = true;
    
    // Si el jugador llega a la salida pasamos al siguiente nivel    
    if(detectarObjeto(juego->mapa->mapa, juego->mapa->tamMap, newX, newY, 'E')){
        cambiarNivel(juego);
        return;
    }
    // Libera la posicion anterior del jugador
    juego->mapa->mapa[juego->posY][juego->posX] = '.';

    // Actualiza la posición dell jugador
    juego->posX = newX;
    juego->posY = newY;

    // Incrementa el contador de pasos
    juego->pasos[juego->nivel-1]++;
}

// Resumen de la partida (el jugador completo todos los niveles o salio del juego)
void mostrarResumenJuego(Juego* juego) {
    system("cls");

    // Encabezado
    printf("\n======================================\n");
    printf("\n             FIN DEL JUEGO            \n");
    printf("\n======================================\n");
    printf("Juego Completado\n");

    // Muestra el total de monedas recoletadas en relacion con las monedas totales de la partida
    printf("Monedas totales recolectadas: %d / %d\n", juego->monedas[0] + juego->monedas[1] + juego->monedas[2],
                                                        juego->monedasTotales[0] + juego->monedasTotales[1] + juego->monedasTotales[2]);
    
    // Cantidad de movimientos realizados
    printf("Pasos Totales: %d\n", juego->pasos[0] + juego->pasos[1] + juego->pasos[2]);
    
    // Numero de niveles completados
    printf("Niveles completados: %d / 4\n" , juego->nivel - 1);
    
    // Puntaje Final
    printf("Puntaje Final: %.2f\n", (juego->puntaje[0] + juego->puntaje[1] + juego->puntaje[2]));
    printf("\n======================================\n");

}

// Estadisticas de un nivel
void mostrarResumenNivel(Juego *juego) {
    printf("\n======================================\n");
    printf("Nivel %d Completado\n", juego->nivel); // Nivel completado

    // Monedas recolectadas en relacion con las monedas totales del nivel
    printf("Monedas: %d / %d\n", juego->monedas[juego->nivel - 1], juego->monedasTotales[juego->nivel - 1]);
    
    // Pasos realizados
    printf("Pasos: %d\n", juego->pasos[juego->nivel - 1]);
    
    // Puntaje obtenido
    printf("Puntaje: %.2f\n", (juego->puntaje[juego->nivel - 1]));
    
    printf("\n======================================\n");
    printf("\nPresione Cualquier tecla para continuar...\n");
    getch();
}

// Estado actual de la partida
// Panel superior de la partida
void mostrarStatus(Juego* juego) {
    // Casillas libres
    printf("Casillas libres: %d\n", contarCeldasLibres( juego->mapa->mapa, pow(juego->mapa->tamMap, 2)));
    
    // Nivel actual
    printf("Nivel: %d\n", juego->nivel);
    
    // Estado de la llave
    printf("Llave: %s\n", juego->llave ? "Si" : "No");
    
    // Pasos realizados
    printf("Pasos: %d\n", juego->pasos[juego->nivel - 1]);
    
    // Monedas recolectadas y monedas totales
    printf("Monedas: %d / %d\n", juego->monedas[juego->nivel - 1], juego->monedasTotales[juego->nivel - 1]);
}

// Pantalla de inicio con informacion del juego
void pantallaInicio() {
    // Enacbezado
    printf("============================================================\n");
    printf("                    BITQUEST\n");
    printf("            Explorador de Matrices\n");
    printf("============================================================\n\n");

    // Reglas
    printf("COMO GANAR\n");
    printf("------------------------------------------------------------\n");
    printf("1. Encuentra la llave (K).\n");
    printf("2. Usa la llave para abrir la puerta (D).\n");
    printf("3. Desbloquea la salida (E).\n");
    printf("4. Completa los 4 niveles para terminar el juego.\n\n");

    // Simbología del mapa
    printf("SIMBOLOS DEL MAPA\n");
    printf("------------------------------------------------------------\n");
    printf("P  = Jugador\n");
    printf(".  = Camino libre\n");
    printf("#  = Pared\n");
    printf("M  = Moneda\n");
    printf("K  = Llave\n");
    printf("D  = Puerta\n");
    printf("E  = Salida\n\n");

    // Controles para mover al jugador
    printf("CONTROLES\n");
    printf("------------------------------------------------------------\n");
    printf("W  = Mover arriba\n");
    printf("A  = Mover izquierda\n");
    printf("S  = Mover abajo\n");
    printf("D  = Mover derecha\n");
    printf("Q  = Salir del juego\n\n");

    // Informacion extra a tomar en cuenta
    printf("CONSEJOS\n");
    printf("------------------------------------------------------------\n");
    printf("- No es obligatorio recoger todas las monedas.\n");
    printf("- Mientras mas monedas recolectes, mayor sera tu puntaje.\n");
    printf("- Menos pasos realizados significan mejor puntuacion.\n\n");
    printf("- Asegurate de tener la llave antes de llegar a la puerta.\n\n");

    printf("============================================================\n");
    printf("        Presiona cualquier tecla para comenzar...\n");
    printf("============================================================\n");

    getch();

    system("cls");
}

