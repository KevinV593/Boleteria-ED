// CODIGO COMPLETO ACTUALIZADO CON CONFIRMACIONES 1/0 VALIDADAS
// SOLO SE USA pedirConfirmacion(); NO SE AGREGA NINGUNA FUNCION EXTRA

#include <iostream>
#include <limits>
#include "../model/TipoAsiento.hpp"
#include "../model/Boleto.hpp"
#include "../model/ListaCircularDoble.hpp"
#include "../utils/Validaciones.hpp"
#include "../utils/Persistencia.hpp"
#include "../model/Nodo.hpp"
#include <vector>
#include <windows.h>
#include <conio.h>

using namespace std;


void inicializarEvento(ListaCircularDoble& boleteria, int totalAsientos) {
    int limiteGeneral = totalAsientos * 0.5;
    int limiteTribuna = totalAsientos * 0.8;

    for (int i = 1; i <= totalAsientos; i++) {
        TipoAsiento categoria;

        if (i <= limiteGeneral) categoria = GENERAL;
        else if (i <= limiteTribuna) categoria = TRIBUNA;
        else categoria = PALCO;

        boleteria.insertarPorFinal(Boleto(i, categoria));
    }
}

string obtenerNombrePorCedula(ListaCircularDoble& lista, const string& cedula) {
    if (lista.estaVacia()) return "";

    Nodo* actual = lista.getCabeza();

    do {
        if (actual->dato.estaOcupado && actual->dato.cedulaCliente == cedula)
            return actual->dato.nombreCliente;

        actual = actual->siguiente;
    } while (actual != lista.getCabeza());

    return "";
}

void limpiarBufferCin() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int pedirConfirmacion(const string& mensaje) {
    int opcion;
    while (true) {
        cout << mensaje << " (1 = Si / 0 = No): ";
        opcion = ingresarEntero();
        if (opcion == 1 || opcion == 0) return opcion;
        cout << "Entrada invalida, ingrese 1 o 0.\n";
    }
}

void menuBoletosMain(ListaCircularDoble& miBoleteria) {
    bool menu = true;
    while (menu) {
        system("cls");
        cout << "****************************" << endl;
        cout << "*    RESERVA DE BOLETOS    *" << endl;
        cout << "****************************" << endl;
        cout << "[1] Reservar Asiento" << endl;
        cout << "[2] Cancelar Reserva" << endl;
        cout << "[3] Mostrar Asientos" << endl;
        cout << "[4] Buscar Reserva" << endl;
        cout << "[5] Actualizar Reserva" << endl;
        cout << "[6] Salir" << endl;

        int opcion = _getch() - '0';

        if (opcion < 1 || opcion > 6) {
            cout << "\nOpcion no valida\n";
            Sleep(500);
            continue;
        }

        switch (opcion) {

            case 1: {
                system("cls");
                cout << "--- RESERVAR ASIENTO ---\n\n";

                cout << "Ingrese el numero de asiento: ";
                int numAsiento = ingresarEntero();

                Nodo* nodoBuscado = miBoleteria.buscar(numAsiento);

                if (nodoBuscado == nullptr) {
                    cout << "Error: Asiento inexistente.\n";
                }
                else if ( (*nodoBuscado).dato.estaOcupado ) {
                    cout << "Error: Asiento ya reservado.\n";
                }
                else {
                    cout << "Asiento disponible.\n";

                    cout << "Ingrese la cedula del cliente: ";
                    string cedula = ingresarCedula();

                    string nombreExistente = obtenerNombrePorCedula(miBoleteria, cedula);
                    string nombre;

                    if ( !(*(&nombreExistente)).empty() ) {
                        cout << "Cedula ya registrada como: " << nombreExistente << endl;
                        nombre = nombreExistente;
                    } else {
                        nombre = ingresarNombre();
                    }

                    (*nodoBuscado).dato.reservar(nombre, cedula);
                    cout << "Reserva realizada.\n";
                }

                Persistencia::guardarReservas(miBoleteria);
                system("pause");
                break;
            }

            case 2: {
                system("cls");
                cout << "--- CANCELAR RESERVA ---\n\n";

                cout << "Ingrese la cedula: ";
                string cedula = ingresarCedula();

                if (miBoleteria.estaVacia()) {
                    cout << "No hay asientos.\n";
                    system("pause");
                    break;
                }

                Nodo* cabeza = miBoleteria.getCabeza();
                Nodo* actual = cabeza;

                vector<Nodo*> encontrados;

                // --- RECORRER LISTA ---
                do {
                    if ((*actual).dato.estaOcupado &&
                        (*actual).dato.cedulaCliente == cedula) {
                        encontrados.push_back(actual);
                    }

                    actual = (*actual).siguiente;
                } while (actual != cabeza);

                if (encontrados.empty()) {
                    cout << "No existen reservas con esa cedula.\n";
                    system("pause");
                    break;
                }

                Nodo** arreglo = encontrados.data();  // puntero al primer nodo

                // --- SOLO UNA RESERVA ---
                if (encontrados.size() == 1) {
                    (*(*(arreglo))).dato.cancelar();
                    Persistencia::guardarReservas(miBoleteria);
                    cout << "Reserva cancelada.\n";
                    system("pause");
                    break;
                }

                cout << "\nLa cedula tiene " << encontrados.size() << " reservas.\n";

                int cancelarTodo = pedirConfirmacion("Cancelar todas?");

                // --- CANCELAR TODAS ---
                if (cancelarTodo == 1) {

                    for (size_t i = 0; i < encontrados.size(); i++) {
                        Nodo* nodo = *(arreglo + i);
                        (*nodo).dato.cancelar();
                    }

                    Persistencia::guardarReservas(miBoleteria);
                    cout << "Reservas canceladas.\n";
                    system("pause");
                    break;
                }

                bool seguir = true;

                // --- CANCELAR UNA POR UNA ---
                while (seguir && !encontrados.empty()) {

                    arreglo = encontrados.data(); // actualizar puntero base

                    cout << "\nReservas encontradas:\n";

                    for (size_t i = 0; i < encontrados.size(); i++) {
                        Nodo* nodo = *(arreglo + i);
                        cout << "[" << (int)(i + 1) << "] Asiento "
                            << (*nodo).dato.numeroAsiento
                            << " (" << (*nodo).dato.getCategoria() << ")\n";
                    }

                    cout << "Seleccione reserva a cancelar: ";
                    int idx = ingresarEntero() - 1;

                    if (idx < 0 || idx >= (int)encontrados.size()) {
                        cout << "Opcion invalida.\n";
                    } else {

                        Nodo* objetivo = *(arreglo + idx);
                        (*objetivo).dato.cancelar();

                        // borrar del vector SIN []
                        encontrados.erase(encontrados.begin() + idx);

                        Persistencia::guardarReservas(miBoleteria);
                        cout << "Reserva cancelada.\n";
                    }

                    if (encontrados.empty()) {
                        cout << "No quedan reservas.\n";
                        break;
                    }

                    seguir = pedirConfirmacion("Cancelar otro?");
                }

                system("pause");
                break;
            }

            case 3: {
                system("cls");
                miBoleteria.mostrarDesdeInicio();
                system("pause");
                break;
            }


            case 4: {
                system("cls");
                cout << "--- BUSCAR RESERVA ---\n\n";

                cout << "Ingrese la cedula: ";
                string cedula = ingresarCedula();

                Nodo* actual = miBoleteria.getCabeza();
                bool encontrado = false;

                if (!miBoleteria.estaVacia()) {
                    do {
                        if (actual->dato.estaOcupado &&
                            actual->dato.cedulaCliente == cedula) {
                            actual->dato.mostrarDatos();
                            encontrado = true;
                        }
                        actual = actual->siguiente;
                    } while (actual != miBoleteria.getCabeza());
                }

                if (!encontrado)
                    cout << "No existe reserva con esa cedula.\n";

                system("pause");
                break;
            }


            case 5: {
                system("cls");
                cout << "--- ACTUALIZAR RESERVA ---\n\n";

                cout << "Ingrese la cedula del cliente: ";
                string cedula = ingresarCedula();

                if (miBoleteria.estaVacia()) {
                    cout << "No hay asientos.\n";
                    system("pause");
                    break;
                }

                Nodo* actual = miBoleteria.getCabeza();
                Nodo* nodoEncontrado = nullptr;

                do {
                    if (actual->dato.estaOcupado &&
                        actual->dato.cedulaCliente == cedula) {
                        nodoEncontrado = actual;
                        break;
                    }
                    actual = actual->siguiente;
                } while (actual != miBoleteria.getCabeza());

                if (!nodoEncontrado) {
                    cout << "No se encontro reserva.\n";
                    system("pause");
                    break;
                }

                cout << "--- Datos actuales ---\n";
                nodoEncontrado->dato.mostrarDatos();
                cout << "\n";

                string nombreTemp = nodoEncontrado->dato.nombreCliente;
                string cedulaTemp = nodoEncontrado->dato.cedulaCliente;

                if (pedirConfirmacion("Cambiar nombre?") == 1) {
                    cout << "Ingrese el nuevo nombre: ";
                    nombreTemp = ingresarNombre();
                }

                // CAMBIO DE CEDULA
                if (pedirConfirmacion("Cambiar cedula?") == 1) {
                    cout << "Ingrese la nueva cedula: ";
                    cedulaTemp = ingresarCedula();
                }

                // CAMBIO DE ASIENTO
                if (pedirConfirmacion("Cambiar asiento?") == 1) {
                    cout << "Ingrese el nuevo numero de asiento: ";
                    int nuevoAsiento = ingresarEntero();

                    Nodo* nuevoNodo = miBoleteria.buscar(nuevoAsiento);

                    if (!nuevoNodo) {
                        cout << "Ese asiento no existe.\n";
                    }
                    else if (nuevoNodo->dato.estaOcupado) {
                        cout << "Ese asiento ya esta ocupado.\n";
                    }
                    else {
                        nodoEncontrado->dato.cancelar();
                        nuevoNodo->dato.reservar(nombreTemp, cedulaTemp);

                        cout << "\nAsiento cambiado correctamente.\n";

                        Persistencia::guardarReservas(miBoleteria);
                        system("pause");
                        break;
                    }
                }

                nodoEncontrado->dato.nombreCliente = nombreTemp;
                nodoEncontrado->dato.cedulaCliente = cedulaTemp;

                cout << "\nDatos actualizados correctamente.\n";

                Persistencia::guardarReservas(miBoleteria);
                system("pause");
                break;
            }

            case 6: {
                menu = false;
                system("cls");
                cout << "Gracias por usar el sistema." << endl;
                break;
            }
        }
    }
}