#include <iostream>
#include <conio.h>
#include <windows.h>
#include "../model/ListaCircularDoble.hpp"
#include "../model/TipoAsiento.hpp"
#include "../view/Menu.hpp"

using namespace std;

// para compilar(ignoralo) g++ controller/main.cpp model/ListaCircularDoble.cpp utils/Validaciones.cpp view/Menu.hpp -o Boleteria.exe

// --- Función principal ---
int main() {
    ListaCircularDoble miBoleteria;
    inicializarEvento(miBoleteria, 5); // Evento con 5 asientos
    menuBoletosMain(miBoleteria);
    return 0;
}
