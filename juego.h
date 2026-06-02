
typedef struct Juego {
    // Posicion del jugador
    int posX; 
    int posY;

    int nivel; // Nivel actual
    int puntaje[3]; // Puntaje del jugador en cada nivel
    int monedas[3]; // Monedas recolectadas en cada nivel
    int monedasTotales[3]; // MOnedas totales en cada nivel
    bool llave; // Bandera de llave por nivel
    int pasos[3]; // Contador de pasos en cada nivel

} Juego;