bits 64
default rel
global contarCaracter
global validarMovimiento
global calcularPuntaje
global detectarObjeto
global contarCeldasLibres

section .data
    tresMil dd 3000.0 ; Multiplicador de monedas
    mil dd 1000.0 ; Multiplicador de niveles
section .text

; Cuenta la cantidad de apariciones de un caracter dentro de la matriz
; Se  usa para contar el numero de monedas en un mapa
contarCaracter:
    ;Inicializamos en ceros
    xor eax, eax ;dato que se retorna
    xor r9d, r9d ; i = 0

    filas: 
        cmp r9d, edx  
        je fin ; i < filas

        mov r10, [rcx + r9*8] ; r10 = mapa[i]

        xor r11d, r11d ; j = 0

    columnas:
        cmp byte [r10 + r11], 0
        je siguiente_fila ; j < columnas

        mov r12b, [r10 + r11] ; mapa[i][j]

        cmp r12b, r8b ; mapa[i][j] == letra
        jne saltar 

        inc eax ; contador++

    saltar:
        inc r11d ; j++
        jmp columnas

    siguiente_fila:
        inc r9d ; i++
        jmp filas

    fin:
 ret

; Regresa 1 si el movimiento es valido y 0 si no lo es
validarMovimiento:
    mov eax, [rsp + 40]
    mov r10, [rcx + rax * 8] ; fila

    mov eax, r9d
    mov r11b, [r10 + rax] ; columna

    ;Casillas validas
    cmp r11b, 'M'
    je movimientoValido

    cmp r11b, 'E'
    je movimientoValido

    cmp r11b, 'K'
    je movimientoValido
    
    cmp r11b, '.'
    je movimientoValido

    ; Casillas invalidas
    cmp r11b, '#'
    je movimientoInvalido

    cmp r11b, 'D'
    jne movimientoInvalido ; Casilla anomala

    ;Valida si el jugador tiene llave para abrir la puerta
    mov eax, [rsp + 48]
    cmp eax, 0
    jne movimientoValido

    movimientoInvalido: ; Devuelve 0
        xor eax, eax
        ret

    movimientoValido: ; Devuelve 1
        mov eax, 1
        ret
ret

; Calcula el puntaje obtenido en un nivel
calcularPuntaje:
    movss xmm4, xmm0
    divss xmm4, xmm1 ; Monedas recolectadas / monedas totales
    mulss xmm4, [tresMil] ; Monedas recolectadas * 3000

    movss xmm5, xmm3
    mulss xmm5, [mil] ; Numero del nivel * 1000

    subss xmm4, xmm2 ; Monedas - pasos realizados
    addss xmm4, xmm5 ; Puntaje + nivel

    movss xmm0, xmm4 ; Regresamos el puntaje obtenido
ret ;Refresamos xmm0

; Devuelve 1 si el objeto se encuentra en esa posicion o 0 si no esel caso
detectarObjeto:
    mov eax, r9d
    mov r10, [rcx + rax * 8] ; Fila

    mov eax, r8d ; Columna

    mov r11b, [r10 + rax] ; Contenido de la celda

    mov al, [rsp + 40] ; Caracter a buscar

    cmp r11b, al
    je encontrado

    xor eax, eax ; Regresamos 0 (false)
    ret

    encontrado:
        mov eax, 1 ; Regresamos 1 (verdadero)
ret

; Devuelve el numero de celdas libres
contarCeldasLibres:
    xor eax, eax ; iniciamos en cero
    xor r8d, r8d ; fila 0 (i=0)

    contar:
        cmp r8d, edx ;Fin de la fila (i=tamMap)
        je end

        mov r9b, [rcx + r8] ; Celda actual

        cmp r9b, '.'
        jne salto ; Casilla no libre
        inc eax ;Encontro una casilla libre

        salto:
            inc r8d ; i++
            jmp contar

    end:
ret

