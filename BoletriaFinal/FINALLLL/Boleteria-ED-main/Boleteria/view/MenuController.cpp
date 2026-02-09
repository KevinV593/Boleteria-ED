#include "MenuController.h"
#include "MenuUI.h"
#include "StringProcessor.h"
#include "../controller/ReservationManager.hpp"
#include "../controller/Exhaustiva.hpp"
#include "../controller/DivideyVenceras.hpp"
#include "../controller/Voraz.hpp"
#include "../controller/Dinamica.hpp"
#include "../controller/Backtracking.hpp"
#include "../utils/Persistencia.hpp"
#include "../utils/Validaciones.hpp"
#include <iostream>
#include <windows.h>
#include "../utils/hash.hpp" 


using namespace std;

const string ARCHIVO_DATOS = "server/reservas.txt";
const string ARCHIVO_HASH = "server/reservas.hash";

void MenuController::ejecutar(ListaCircularDoble &boleteria)
{

    cout << "Verificando integridad de los datos...\n";

    if (!verificarIntegridad(ARCHIVO_DATOS, ARCHIVO_HASH)) {

        system("color 4F"); 
        
        cout << "\n[PELIGRO] ALERTA DE SEGURIDAD\n";
        cout << "El archivo de reservas ha sido modificado externamente.\n";
        cout << "El hash de seguridad no coincide.\n\n";
        cout << "El sistema se cerrara por precaucion.\n";
        system("pause");
        exit(1); 
    } else {
        // CASO DE ÉXITO (TODO ESTÁ BIEN)
        system("color 07"); // <--- AGREGA ESTO: Fondo NEGRO, letras Blancas (Normal)
        
        cout << "[OK] Integridad verificada correctamente.\n";
        Sleep(1000); 
        system("cls");
    }

    // ... resto del código ...

    bool menu = true;
    while (menu)
    {
        int opcion = MenuUI::mostrarMenuPrincipal();

        if (opcion < 1 || opcion > 13)
        {
            cout << "\nOpcion no valida\n";
            Sleep(500);
            continue;
        }

        if (opcion == 13 )
        {
            menu = false;
            system("cls");
            cout << "Gracias por usar el sistema." << endl;
        }
        else
        {
            procesarOpcion(opcion, boleteria);
        }
    }
}

void MenuController::procesarOpcion(int opcion, ListaCircularDoble &boleteria)
{
    switch (opcion)
    {
    case 1:
        ReservationManager::reservarAsiento(boleteria);
        break;

    case 2:
        ReservationManager::cancelarReserva(boleteria);
        break;

    case 3:
        MenuUI::mostrarAsientos(boleteria);
        break;

    case 4:
        ReservationManager::buscarReservasPorCedula(boleteria);
        break;

    case 5:
        ReservationManager::actualizarReserva(boleteria);
        break;

    case 6:
    {
        system("cls");
        Persistencia::cargarReservas(boleteria);

        cout << "--- NOMBRES CON CARACTERES ORDENADOS ---\n\n";

        if (boleteria.estaVacia())
        {
            cout << "No hay reservas registradas.\n";
            system("pause");
            break;
        }

        Nodo *actual = boleteria.getCabeza();
        bool hayReservas = false;

        cout << "Procesando nombres de la lista en memoria...\n\n";

        do
        {
            if (actual->dato.estaOcupado)
            {
                hayReservas = true;
                StringProcessor::mostrarNombreOrdenado(actual->dato.nombreCliente);
            }
            actual = actual->siguiente;
        } while (actual != boleteria.getCabeza());

        if (!hayReservas)
        {
            cout << "La lista esta vacia de reservas activas.\n";
        }

        system("pause");
        break;
    }

    case 7:
    {
        system("cls");
        cout << "--- ORDENAMIENTO POR GRUPOS ---\n\n";

        if (boleteria.estaVacia())
        {
            cout << "No hay reservas.\n";
            system("pause");
            break;
        }

        cout << "Ingrese el tamano del grupo para agrupar las letras: ";
        int k = ingresarEntero();

        cout << "\nProcesando...\n";

        Nodo *actual = boleteria.getCabeza();
        bool hayReservas = false;

        do
        {
            if (actual->dato.estaOcupado)
            {
                hayReservas = true;
                StringProcessor::procesarPorGrupos(actual->dato.nombreCliente, k);
            }
            actual = actual->siguiente;
        } while (actual != boleteria.getCabeza());

        if (!hayReservas)
            cout << "Todas las reservas estan vacias.\n";

        cout << "\n";
        system("pause");
        break;
    }

    case 8:
    {
        Exhaustiva::ingresosTotales(boleteria);
        break; 
    }
    /*
    case 9:
    {
        Exhaustiva::busquedaPorPatron(boleteria);
        break;
    }
    */

    case 9: 
    {
    system("cls");
    cout << "--- ASIENTOS OCUPADOS ---\n";
    
    if (boleteria.estaVacia()) {
        cout << "No hay asientos para ordenar.\n";
        system("pause");
        break;
    }

    int contador = 0;
    Nodo* temp = boleteria.getCabeza();
    do {
        if (temp->dato.estaOcupado) contador++;
        temp = temp->siguiente;
    } while (temp != boleteria.getCabeza());

    if (contador == 0) {
        cout << "No hay asientos ocupados.\n";
        system("pause");
        break;
    }

    int* arregloDinamico = new int[contador];

    temp = boleteria.getCabeza();
    int i = 0;
    do {
        if (temp->dato.estaOcupado) {
            *(arregloDinamico + i) = temp->dato.numeroAsiento; 
            i++;
        }
        temp = temp->siguiente;
    } while (temp != boleteria.getCabeza());

    cout << "Ordenando " << contador << " elementos usando memoria dinamica...\n";

    DivideYVenceras::mergeSortPunteros(arregloDinamico, 0, contador - 1);

    cout << "Asientos ocupados ordenados: ";
    for (int j = 0; j < contador; j++) {
        cout << *(arregloDinamico + j) << " ";
    }
    cout << endl;

    delete[] arregloDinamico;
    
    system("pause");
    break;
    }
/*
    case 10: 
    {
    system("cls");
    cout << "--- BUSQUEDA DE ASIENTO (Busqueda Binaria) ---\n";

    if (boleteria.estaVacia()) {
        cout << "Lista vacia.\n";
        system("pause");
        break;
    }

    int contador = 0;
    Nodo* temp = boleteria.getCabeza();
    do {
        if (temp->dato.estaOcupado) contador++;
        temp = temp->siguiente;
    } while (temp != boleteria.getCabeza());

    if (contador == 0) {
        cout << "No hay asientos ocupados para buscar.\n";
        system("pause");
        break;
    }

    int* arregloDinamico = new int[contador];

    temp = boleteria.getCabeza();
    int i = 0;
    do {
        if (temp->dato.estaOcupado) {
            *(arregloDinamico + i) = temp->dato.numeroAsiento;
            i++;
        }
        temp = temp->siguiente;
    } while (temp != boleteria.getCabeza());

    DivideYVenceras::mergeSortPunteros(arregloDinamico, 0, contador - 1);

    int objetivo;
    cout << "Ingrese numero de asiento ocupado a buscar: ";
    cin >> objetivo;

    int resultadoIndex = DivideYVenceras::binarySearchPunteros(arregloDinamico, 0, contador - 1, objetivo);

    if (resultadoIndex != -1) {
        cout << "EXITO: El asiento " << objetivo << " esta ocupado y se encuentra en el indice " 
             << resultadoIndex << " del arreglo temporal.\n";
    } else {
        cout << "FALLO: El asiento " << objetivo << " no esta en la lista de ocupados.\n";
    }

    delete[] arregloDinamico;

    system("pause");
    break;
    }


    case 12: 
    {
        Voraz::cambioDeMoneda();
        system("pause");
        break;
    }
*/

    case 10: 
    {
        Voraz::asignacionRapida(boleteria);
        break;
    }
    /*
    case 14:
    {
        system("cls");
        cout << "--- PROGRAMACION DINAMICA: MOCHILA (PRESUPUESTO DE ENTRADAS) ---\n\n";

        cout << "Ingrese el presupuesto disponible: $";
        int presupuesto = ingresarEntero();

        int n = 3;

        int* precios = new int[n];
        int* valores = new int[n];

        *(precios + 0) = 100;  
        *(precios + 1) = 60;   
        *(precios + 2) = 40;   

        *(valores + 0) = 3;
        *(valores + 1) = 2;
        *(valores + 2) = 1;

        Dinamica::mochila01(precios, valores, n, presupuesto);

        delete[] precios;
        delete[] valores;

        system("pause");
        break;
    }
    */

    /*
    case 11:
    {
        system("cls");
        cout << "--- PROGRAMACION DINAMICA: SIMILITUD DE NOMBRES (LCS) ---\n\n";
        Dinamica::lcs(boleteria);
        system("pause");
        break;
    }
*/
    
    case 11:
    {
        system("cls");
        cout << "--- BACKTRACKING: COMBINACIONES DE ASIENTOS OCUPADOS ---\n\n";
        Backtracking::combinacionesAsientos(boleteria);
        system("pause");
        break;
    }
    
    case 12:
    {
        system("cls");
        cout << "--- BACKTRACKING: PERMUTACIONES DE NOMBRES DE CLIENTES ---\n\n";
        Backtracking::permutacionesNombres(boleteria);
        system("pause");
        break;
    }

    }
}