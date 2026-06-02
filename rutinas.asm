// Funciones implementadas en NASM

// Funciones Obligatorias
// Funciones NASM

bits 64
default rel
global contarCaracter
global validarMovimiento
global calcularPuntaje
global detectarObjeto

global contarCeldasLibres

section .text

contarCaracter:
    // Inicializamos en ceros
    xor eax, eax
    xor r9d, r9d

    comparar:
        cmp r9d, edx  // Fin de la cadena
        je fin // Terminamos de iterar

        mov r10b, [rcx + r9] // Nos desplazamos en la cadena

        cmp r10b, r8b // Comparamos caracteres
        jne iterar

        inc eax // agregamos un caracter encontrado al contador
    
    iterar:
        inc r9d 
        jmp comparar // Iteramos

    fin:
ret

validarMovimiento:
    mov eax, [rsp + 40]
    mov r10, [rcx + (rax * 8)]

    mov eax, r9d
    mov r11b, [r10 + rax]

    // Casillas validas
    cmp r11b, 'M'
    je movimientoValido

    cmp r11b, 'K'
    je movimientoValidocmp
    
    r11b, '.'
    je movimientoValido

    // Casillas invalidas
    cmp r11b, '#'
    je movimientoInvalido

    cmp r11b, 'D'
    jne movimientoInvalido // Casilla anomala

    // Valida si el jugador tiene llave para abrir la puerta
    mov eax, [rsp + 48]
    cmp eax, 0
    jne movimientoValido

    movimientoInvalido: // Devuelve 0
        xor eax, eax
        ret

    movimientoValido: // Devuelve 1
        mov eax, 1
        ret
ret

calcularPuntaje:
    // calcularPuntaje(xmm0, xmm1, xmm2, xmm3)
    divss xmm0, xmm1
    divss xmm0, xmm2
    mulss xmm0, xmm3
ret // Refresamos xmm0

detectarObjeto:
    mov eax, r9d
    mov r10, [rcx + (rax * 8)]

    mov eax, r8d

    mov r11b, [r10 + rax]

    mov al, [rsp + 40]

    cmp r11b, al
    je encontrado

    xor eax, eax
    ret

    encontrado:
        mov eax, 1
ret

contarCeldasLibres:
    xor eax, eax
    xor r8d, r8d

    contar:
        cmp r8d, edx // Fin de la fila
        je fin

        mov r9b, [rcx + r8]

        cmp r9b, [rcx + r8]
        cmp r9b, '.'
        jne iterar
        inc eax // Encontro una casilla libre

        iterar: // Salto
            inc r8d
            jmp contar

    fin:
ret

