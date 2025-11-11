#include <iostream>
#include <string>
#include <limits> 

#include "Boleto.hpp"
#include "ListaCircularDoble.hpp"

using namespace std;

// --- Función para inicializar los asientos del evento ---
void inicializarEvento(ListaCircularDoble& boleteria, int totalAsientos) {
    cout << "Inicializando evento con " << totalAsientos << " asientos..." << endl;
    
   
    for (int i = totalAsientos; i >= 1; i--) {
        boleteria.insertarPorCabeza( Boleto(i) );
    }
    cout << "¡Evento listo!" << endl;
}

// --- Función para limpiar el buffer de cin ---
void limpiarBufferCin() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    // 1. Crear la instancia de nuestro Modelo
    ListaCircularDoble miBoleteria;

    // 2. Pre-cargar la lista con los asientos disponibles
    inicializarEvento(miBoleteria, 5); // Creamos un evento de 5 asientos

    int opcion = 0;
    do {
        // 3. Mostrar el Menú (VISTA)
        cout << "\n--- BOLETERÍA DE EVENTO ---" << endl;
        cout << "1. Reservar Asiento" << endl;
        cout << "2. Cancelar Reserva" << endl;
        cout << "3. Mostrar Asientos" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione una opción: ";

        // 4. Leer la entrada (CONTROLADOR)
        if (!(cin >> opcion)) {
            cout << "Error: Por favor ingrese un número." << endl;
            cin.clear();
            limpiarBufferCin();
            continue; // Vuelve al inicio del bucle
        }
        limpiarBufferCin(); // Limpiar el 'enter'

        // 5. Procesar la opción (CONTROLADOR llama al MODELO)
        switch (opcion) {
            case 1: { // --- Reservar Asiento ---
                cout << "Ingrese el número de asiento a reservar: ";
                int numAsiento = 0;
                cin >> numAsiento;
                limpiarBufferCin();
                
                Nodo* nodoBuscado = miBoleteria.buscar(numAsiento);

                if (nodoBuscado == nullptr) {
                    cout << "Error: El asiento " << numAsiento << " no existe." << endl;
                } else if (nodoBuscado->dato.estaOcupado) {
                    cout << "Error: El asiento ya está ocupado por " 
                         << nodoBuscado->dato.nombreCliente << "." << endl;
                } else {
                    cout << "Ingrese el nombre del cliente: ";
                    string nombre;
                    getline(cin, nombre); // Usamos getline para nombres con espacios
                    
                    // Llamamos al Modelo para actualizar el dato
                    nodoBuscado->dato.reservar(nombre);
                    cout << "¡Asiento " << numAsiento << " reservado a " << nombre << "!" << endl;
                }
                break;
            }

            case 2: { // --- Cancelar Reserva ---
                cout << "Ingrese el número de asiento a cancelar: ";
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
                    // Llamamos al Modelo para actualizar el dato
                    nodoBuscado->dato.cancelar();
                    cout << "¡Reserva de " << nombreCliente << " en asiento " 
                         << numAsiento << " cancelada!" << endl;
                }
                break;
            }

            case 3: { // --- Mostrar Asientos ---
                cout << "\n--- Estado Actual de Asientos ---" << endl;
                miBoleteria.mostrarDesdeInicio();
                break;
            }

            case 4: { // --- Salir ---
                cout << "Gracias por usar el sistema." << endl;
                break;
            }

            default: {
                cout << "Opción no válida. Intente de nuevo." << endl;
                break;
            }
        } // fin switch

    } while (opcion != 4);

    return 0;
}