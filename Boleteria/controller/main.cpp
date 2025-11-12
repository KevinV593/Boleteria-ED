#include <iostream>
#include <conio.h>
#include <windows.h>
#include "../model/ListaCircularDoble.hpp"
#include "../view/Menu.hpp"
#include "../utils/Persistencia.hpp"

using namespace std;

// para compilar(ignoralo) g++ controller/main.cpp model/ListaCircularDoble.cpp utils/Validaciones.cpp -o Boleteria.exe

// --- Función principal ---
int main() {
    ListaCircularDoble miBoleteria;

    inicializarEvento(miBoleteria, 5);
    Persistencia::cargarReservas(miBoleteria);
    menuBoletosMain(miBoleteria);
    Persistencia::guardarReservas(miBoleteria);

    return 0;
}
