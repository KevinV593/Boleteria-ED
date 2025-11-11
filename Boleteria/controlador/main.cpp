#include <iostream>
#include <conio.h>
#include <windows.h>
#include "../modelo/ListaCircularDoble.hpp"
#include "../Vista/Menu.hpp"

using namespace std;

// --- Función principal ---
int main() {
    ListaCircularDoble miBoleteria;
    inicializarEvento(miBoleteria, 5); // Evento con 5 asientos
    menuBoletosMain(miBoleteria);
    return 0;
}
