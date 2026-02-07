#include <iostream>
#include <conio.h>
#include <windows.h>
#include "../model/ListaCircularDoble.hpp"
#include "../view/Menu.hpp"
#include "../utils/Persistencia.hpp"

using namespace std;


int main() {
    ListaCircularDoble miBoleteria;
    
    int numeroTotalDeAsientos = 20; //Parametro a Eleguir
    inicializarEvento(miBoleteria, numeroTotalDeAsientos);
    Persistencia::cargarReservas(miBoleteria);
    menuBoletosMain(miBoleteria);
    Persistencia::guardarReservas(miBoleteria);

    return 0;
}
