#include <iostream>
#include <conio.h>
#include <windows.h>
#include "../model/ListaCircularDoble.hpp"
#include "../model/TipoAsiento.hpp"

#include "../view/Menu.hpp"

using namespace std;

// --- Función principal ---
int main() {
    ListaCircularDoble miBoleteria;
    inicializarEvento(miBoleteria, 5); // Evento con 5 asientos
    menuBoletosMain(miBoleteria);
    return 0;
}
