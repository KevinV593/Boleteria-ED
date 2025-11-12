#include <iostream>
#include <limits>
#include "../model/TipoAsiento.hpp"
#include "../model/Boleto.hpp"
#include "../model/ListaCircularDoble.hpp"
#include "../utils/Validaciones.hpp" 
#include "../utils/Persistencia.hpp" 

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
        cout << "****************************" << endl;
        cout << "*    RESERVA DE BOLETOS    *" << endl;
        cout << "****************************" << endl;
        cout << "[1] Reservar Asiento" << endl;
        cout << "[2] Cancelar Reserva" << endl;
        cout << "[3] Mostrar Asientos" << endl;
        cout << "[4] Buscar Reserva" << endl;
        cout << "[5] Salir" << endl;

        int opcion = _getch() - '0'; // Convertir char a número

        if (opcion < 1 || opcion > 5) {
            cout << "\nOpcion no valida\n";
            Sleep(500); // Pausa corta para que el usuario vea el mensaje
            continue;
        }

        switch (opcion) {
           case 1: { // Reservar Asiento
           system("cls");
            cout << "///    RESERVAR    ///" << endl << endl;
                cout << "Ingrese el numero de asiento a reservar: ";
                int numAsiento = ingresarEntero();

                Nodo* nodoBuscado = miBoleteria.buscar(numAsiento);
                if (nodoBuscado == nullptr) {
                    cout << "Error: El asiento " << numAsiento << " no existe." << endl;
                } else if (nodoBuscado->dato.estaOcupado) {
                    cout << "Error: Este asiento no esta disponible para reservar" << endl;
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
                Persistencia::guardarReservas(miBoleteria);
                system("pause");
                break;
            }

            case 2: { // Cancelar Reserva
            system("cls");
                cout << "Ingrese el numero de asiento a cancelar: ";
                int numAsiento = ingresarEntero();

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
                Persistencia::guardarReservas(miBoleteria);
                system("pause");
                break;
            }


            case 3: { // Mostrar Asientos
            system("cls");
                cout << "\n--- Estado Actual de Asientos ---" << endl << endl;
                miBoleteria.mostrarDesdeInicio();
                system("pause");
                break;
            }

            case 4: { // Buscar por Cedula
            system("cls");
            cout << "--- BUSCAR RESERVA---" << endl << endl;
            
            cout << "Ingrese la cedula a buscar: ";
            string cedula = ingresarCedula();

            if (miBoleteria.estaVacia()) {
                    cout << "No hay asientos registrados." << endl;
                } 

            else {
                Nodo* actual = miBoleteria.getCabeza(); 
                bool encontrado = false;

                    cout<< "\nResultados de Busqueda:\n";
                    do {
                        if (actual->dato.estaOcupado && actual->dato.cedulaCliente == cedula) {
                            cout << "\n--- Asiento Encontrado ---\n";
                            actual->dato.mostrarDatos();
                            cout<<endl<<endl;
                            encontrado = true;
                        }
                        actual = actual->siguiente;
                    } while (actual != miBoleteria.getCabeza());
                    
                    if (!encontrado) {
                        cout << "No se encontro un asiento reservado con la cedula " << cedula << ".\n";
                    }
                }
                system("pause");
                break;
            }

            case 5: { // Salir
                menu = false;
                system("cls");
                cout << "Gracias por usar el sistema." << endl;
                break;
            }
        }
    }
};
