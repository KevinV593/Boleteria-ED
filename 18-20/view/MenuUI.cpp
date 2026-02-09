#include "MenuUI.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "../utils/Validaciones.hpp"

using namespace std;

int MenuUI::mostrarMenuPrincipal()
{
    int opcion;
    system("cls");
    cout << "****************************\n";
    cout << "*    RESERVA DE BOLETOS    *\n";
    cout << "****************************\n";

    cout << "[1] Reservar Asiento\n";
    cout << "[2] Cancelar Reserva\n";
    cout << "[3] Mostrar Asientos\n";
    cout << "[4] Buscar Reserva\n";
    cout << "[5] Actualizar Reserva\n";
    cout << "[6] Ordenar Caracteres de Nombres\n";
    cout << "[7] Ordenar Nombre por Grupos\n";
    cout << "[8] Calcular Ingresos Totales\n";

    cout << "[9] Asientos Ocupados\n";

    cout << "[10] Asignacion Rapida de Asientos\n";
    cout << "[11] Generar combinaciones de asientos \n";
    cout << "[12] Generar permutaciones de nombres \n";
    
    cout << "\n[13] Salir\n\n";

    opcion = ingresarEntero();
    return opcion;
}

void MenuUI::mostrarAsientos(ListaCircularDoble &boleteria)
{
    system("cls");
    boleteria.mostrarDesdeInicio();
    system("pause");
}
