#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <limits>
#include "../model/TipoAsiento.hpp"
#include "../model/Boleto.hpp"
#include "../model/ListaCircularDoble.hpp"
#include "../utils/Validaciones.hpp" 


// --- Función para inicializar los asientos del evento ---
void inicializarEvento(ListaCircularDoble& boleteria, int totalAsientos) {
    // Definir límites de zonas:
    // Primeros 50% -> General
    // Siguientes 25% -> Tribuna
    // Últimos 25% -> Palco
    int limiteGeneral = totalAsientos * 0.5; 
    int limiteTribuna = totalAsientos * 0.75; 

    for (int i = 1; i <= totalAsientos; i++) {
        
        TipoAsiento categoria;

        if (i <= limiteGeneral) {
            categoria = GENERAL;
        } else if (i <= limiteTribuna) {
            categoria = TRIBUNA;
        } else {
            categoria = PALCO;
        }

        boleteria.insertarPorFinal(Boleto(i, categoria));
    }
}

void limpiarBufferCin() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

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
            Sleep(500); // Pausa corta para que el usuario vea el mensaje
            continue;
        }

        switch (opcion) {
           case 1: { // Reservar Asiento
            cout << "///    RESERVAR    ///" << endl;
                cout << "Ingrese el numero de asiento a reservar: ";
                int numAsiento = ingresarEntero();

                Nodo* nodoBuscado = miBoleteria.buscar(numAsiento);
                if (nodoBuscado == nullptr) {
                    cout << "Error: El asiento " << numAsiento << " no existe." << endl;
                } else if (nodoBuscado->dato.estaOcupado) {
                    cout << "Error: El asiento ya esta ocupado por " 
                        << nodoBuscado->dato.nombreCliente 
                        << " (Cedula: " << nodoBuscado->dato.cedulaCliente << ")." << endl;
                } else {
                    cout << "Asiento encontrado." << endl;

                    cout << "Ingrese el nombre del cliente: ";
                    string nombre = ingresarLetra();

                    cout << "Ingrese la cedula del cliente: ";
                    string cedula = ingresarCedula();

                    nodoBuscado->dato.reservar(nombre, cedula);
                    cout << "Asiento " << numAsiento << " reservado a " << nombre 
                        << " (Cedula: " << cedula << ")" << endl;
                }
                system("pause");
                break;
            }

            case 2: { // Cancelar Reserva
                cout << "Ingrese el numero de asiento a cancelar: ";
                int numAsiento = leerInt();

                Nodo* nodoBuscado = miBoleteria.buscar(numAsiento);
                if (nodoBuscado == nullptr) {
                    cout << "Error: El asiento " << numAsiento << " no existe." << endl;
                } else if (!nodoBuscado->dato.estaOcupado) {
                    cout << "Info: El asiento " << numAsiento << " ya estaba libre." << endl;
                } else {
                    string nombreCliente = nodoBuscado->dato.nombreCliente;
                    string cedulaCliente = nodoBuscado->dato.cedulaCliente; 
                    nodoBuscado->dato.cancelar();
                    cout << "Reserva de " << nombreCliente << " (Cedula: " << cedulaCliente << ")"
                        << " en asiento " << numAsiento << " cancelada" << endl;
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
};
