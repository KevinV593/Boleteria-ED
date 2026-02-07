#include <iostream>
#include <conio.h>
#include <windows.h>
#include "../model/ListaCircularDoble.hpp"
#include "../view/Menu.hpp"
#include "../utils/Persistencia.hpp"

using namespace std;

// para compilar(ignoralo) - al readme

int main() {
    HANDLE hMutex = CreateMutex(
        NULL,                
        TRUE,                
        "Global\\BoleteriaMutex" 
    );

    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBox(
            NULL,
            "El sistema ya está en ejecucion.",
            "Error",
            MB_OK | MB_ICONERROR
        );
        return 0;
    }

    ListaCircularDoble miBoleteria;
    
    int numeroTotalDeAsientos = 20; 
    inicializarEvento(miBoleteria, numeroTotalDeAsientos);
    Persistencia::cargarReservas(miBoleteria);
    menuBoletosMain(miBoleteria);
    Persistencia::guardarReservas(miBoleteria);

    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
    return 0;
    return 0;
}
