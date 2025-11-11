#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <limits>
#include "../modelo/Boleto.hpp"
#include "../modelo/ListaCircularDoble.hpp"

// --- Función para inicializar los asientos del evento ---
void inicializarEvento(ListaCircularDoble& boleteria, int totalAsientos) {
    for (int i = totalAsientos; i >= 1; i--) {
        boleteria.insertarPorCabeza(Boleto(i));
    }
}

// --- Función para limpiar el buffer de cin ---
void limpiarBufferCin() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- Menú principal con navegación rápida usando _getch() ---
void menuBoletosMain(ListaCircularDoble& miBoleteria) {
    bool menu = true;
    while (menu) {
        system("cls"); // Limpia la pantalla
        cout << "--- BOLETERIA DE EVENTO ---" << endl;
        cout << "[1] Reservar Asiento" << endl;
        cout << "[2] Cancelar Reserva" << endl;
        cout << "[3] Mostrar Asientos" << endl;
        cout << "[4] Salir" << endl;

        int opcion = _getch() - '0'; // Convertir char a número

        if (opcion < 1 || opcion > 4) {
            cout << "\nOpcion no valida\n";
            Sleep(1000); // Pausa corta para que el usuario vea el mensaje
            continue;
        }

        switch (opcion) {
            case 1: { // Reservar Asiento
                cout << "Ingrese el numero de asiento a reservar: ";
                int numAsiento = 0;
                cin >> numAsiento;
                limpiarBufferCin();

                Nodo* nodoBuscado = miBoleteria.buscar(numAsiento);
                if (nodoBuscado == nullptr) {
                    cout << "Error: El asiento " << numAsiento << " no existe." << endl;
                } else if (nodoBuscado->dato.estaOcupado) {
                    cout << "Error: El asiento ya esta ocupado por " 
                         << nodoBuscado->dato.nombreCliente << "." << endl;
                } else {
                    cout << "Ingrese el nombre del cliente: ";
                    string nombre;
                    getline(cin, nombre);
                    nodoBuscado->dato.reservar(nombre);
                    cout << "Asiento " << numAsiento << " reservado a " << nombre << "" << endl;
                }
                system("pause");
                break;
            }

            case 2: { // Cancelar Reserva
                cout << "Ingrese el numero de asiento a cancelar: ";
                int numAsiento = 0;
                cin >> numAsiento;
                limpiarBufferCin();

                Nodo* nodoBuscado = miBoleteria.buscar(numAsiento);
                if (nodoBuscado == nullptr) {
                    cout << "Error: El asiento " << numAsiento << " no existe." << endl;
                } else if (!nodoBuscado->dato.estaOcupado) {
                    cout << "Info: El asiento " << numAsiento << " ya estaba libre." << endl;
                } else {
                    string nombreCliente = nodoBuscado->dato.nombreCliente;
                    nodoBuscado->dato.cancelar();
                    cout << "Reserva de " << nombreCliente << " en asiento " 
                         << numAsiento << " cancelada" << endl;
                }
                system("pause");
                break;
            }

            case 3: { // Mostrar Asientos
                cout << "\n--- Estado Actual de Asientos ---" << endl;
                miBoleteria.mostrarDesdeInicio();
                system("pause");
                break;
            }

            case 4: { // Salir
                menu = false;
                cout << "Gracias por usar el sistema." << endl;
                break;
            }
        }
    }
}
