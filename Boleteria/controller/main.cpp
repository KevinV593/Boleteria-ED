#include <iostream>
#include <conio.h>
#include <windows.h>
#include "../model/ListaCircularDoble.hpp"
#include "../view/Menu.hpp"
#include "../utils/Persistencia.hpp"

using namespace std;

// para compilar(ignoralo) - al readme

int main() {
    ListaCircularDoble miBoleteria;

    inicializarEvento(miBoleteria, 20);
    Persistencia::cargarReservas(miBoleteria);
    menuBoletosMain(miBoleteria);
    Persistencia::guardarReservas(miBoleteria);

    return 0;
}
