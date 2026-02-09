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

    cout<< "Exhaustiva:\n";
    cout << "[8] Calcular Ingresos Totales\n";
    cout << "[9] Busqueda de Cliente por Patron\n";

    cout << "Divide y Venceras:\n";
    cout << "[10] Ordenar Asientos Ocupados (Merge Sort)\n";
    cout << "[11] Busqueda de Asiento Ocupado (Busqueda Binaria)\n";

    cout<< "Algoritmos Voraces:\n";
    cout << "[12] Cambio (Vuelto) \n";
    cout << "[13] Asignacion Rapida de Asientos\n";
    
    cout<< "Programacion Dinamica:\n";
    cout << "[14] Problema de la Mochila (OCC) \n";
    cout << "[15] Similitud de Nombres (LCS) \n";

    cout<< "Backtracking:\n";
    cout << "[16] Generar combinaciones de asientos \n";
    cout << "[17] Generar permutaciones de nombres \n";
    
    cout << "\n[18] Salir\n";

    opcion = ingresarEntero();
    return opcion;
}

void MenuUI::mostrarAsientos(ListaCircularDoble &boleteria)
{
    system("cls");
    boleteria.mostrarDesdeInicio();
    system("pause");
}
