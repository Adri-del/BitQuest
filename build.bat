@echo off

echo Rutinas NAMS...

::Compilar las rutinas nasms
nasm -f win64 rutinas.asm -o rutinas.obj

if errorlevel 1 (
    echo Error al compilar rutinas.asm
    exit /b 1
)

echo.
echo Compilando el proyecto...

::Compilar el proyecto
gcc main.c juego.c rutinas.obj -o BitQuest.exe

if errorlevel 1 (
    echo Error al compilar BitQuest
    exit /b 1
)

echo.
echo Compilacion finalizada correctamente

pause

cls

::Empezar con la ejecucion
BitQuest.exe