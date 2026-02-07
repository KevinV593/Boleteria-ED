#include <iostream>
#include <limits>
#include "../model/TipoAsiento.hpp"
#include "../model/Boleto.hpp"
#include "../model/ListaCircularDoble.hpp"
#include "../utils/Validaciones.hpp"
#include "../utils/Persistencia.hpp"
#include "../utils/ConcurrenciaOCC.hpp"
#include "../utils/Tiempo.hpp"
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

int pedirConfirmacion(const string& mensaje) {
    int opcion;
    while (true) {
        cout << mensaje << " (1 = Si / 0 = No): ";
        opcion = ingresarEntero();
        if (opcion == 1 || opcion == 0) return opcion;
        cout << "Entrada invalida.\n";
    }
}

#include <cctype>

struct NodoLetra {
    char letra;
    NodoLetra* siguiente;
};

void agregarLetra(NodoLetra*& cabeza, NodoLetra*& fin, char c) {
    NodoLetra* nuevo = new NodoLetra{ c, nullptr };
    if (!cabeza) cabeza = fin = nuevo;
    else { fin->siguiente = nuevo; fin = nuevo; }
}

void ordenarSubLista(NodoLetra* cabeza) {
    if (!cabeza) return;
    bool ordenado;
    do {
        ordenado = true;
        for (NodoLetra* a = cabeza; a->siguiente; a = a->siguiente) {
            if (tolower(a->letra) > tolower(a->siguiente->letra)) {
                swap(a->letra, a->siguiente->letra);
                ordenado = false;
            }
        }
    } while (!ordenado);
}

void liberarSubLista(NodoLetra*& cabeza) {
    while (cabeza) {
        NodoLetra* aux = cabeza;
        cabeza = cabeza->siguiente;
        delete aux;
    }
}

void procesarPorGrupos(string nombre, int tamanoGrupo) {
    if (tamanoGrupo <= 0) return;

    const char* lector = nombre.c_str();
    while (*lector) {
        NodoLetra* cabeza = nullptr;
        NodoLetra* fin = nullptr;
        int c = 0;

        while (c < tamanoGrupo && *lector) {
            agregarLetra(cabeza, fin, *lector++);
            c++;
        }

        ordenarSubLista(cabeza);
        for (NodoLetra* n = cabeza; n; n = n->siguiente)
            cout << n->letra;

        liberarSubLista(cabeza);
    }
    cout << endl;
}

void mostrarNombreOrdenado(string nombreOriginal) {
    if (nombreOriginal.empty()) return;
    NodoLetra* cabeza = nullptr;
    NodoLetra* fin = nullptr;

    for (const char* p = nombreOriginal.c_str(); *p; p++)
        agregarLetra(cabeza, fin, *p);

    ordenarSubLista(cabeza);

    for (NodoLetra* n = cabeza; n; n = n->siguiente)
        cout << n->letra;
    cout << endl;

    liberarSubLista(cabeza);
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
        cout << "[6] Ordenar Caracteres de Nombres" << endl;
        cout << "[7] Ordenar Nombre por Grupos" << endl;
        cout << "[8] Salir" << endl;

        int opcion = _getch() - '0';
        if (opcion < 1 || opcion > 8) {
            cout << "\nOpcion no valida\n";
            Sleep(500);
            continue;
        }
        switch (opcion) {

        case 1: {
            system("cls");
            cout << "--- RESERVAR ASIENTO ---\n\n";
            Persistencia::cargarReservas(miBoleteria);

            cout << "Ingrese el numero de asiento: ";
            int num = ingresarEntero();

            Nodo* n = miBoleteria.buscar(num);

            if (!n) {
                cout << "Error: Asiento inexistente.\n";
                system("pause");
                break;
            }

            if (n->dato.estaOcupado) {
                cout << "Error: Asiento ya reservado.\n";
                system("pause");
                break;
            }

            cout << "Asiento disponible.\n";

            cout << "Ingrese la cedula del cliente: ";
            string cedula = ingresarCedula();

            string nombre = obtenerNombrePorCedula(miBoleteria, cedula);

            if (!nombre.empty()) {
                cout << "Cedula ya registrada como: " << nombre << endl;
            } else {
                cout << "Ingrese el nombre del cliente: ";
                nombre = ingresarNombre();
            }

            cout << "\n Procesando reserva con verificacion de concurrencia...\n";
            
            bool exito = ConcurrenciaOCC::actualizarAsiento(
                num,                          
                "OCUPADO",              
                nombre,                      
                cedula,                        
                n->dato.getCategoria(),        
                Tiempo::obtenerFecha(),        
                Tiempo::obtenerHora()          
            );

            if (exito) {
 
                Persistencia::cargarReservas(miBoleteria);
            } else {
                cout << "\n Refrescando disponibilidad de asientos...\n";
                Persistencia::cargarReservas(miBoleteria);
                cout << "    Por favor, elige otro asiento.\n";
            }

            system("pause");
            break;
        }


        case 2: {
                system("cls");
                cout << "--- CANCELAR RESERVA ---\n\n";

                Persistencia::cargarReservas(miBoleteria);

                if (miBoleteria.estaVacia()) {
                    cout << "No hay reservas registradas.\n";
                    system("pause");
                    break;
                }

                cout << "Ingrese la cedula: ";
                string cedula = ingresarCedula();

                Nodo* cabeza = miBoleteria.getCabeza();
                Nodo* actual = cabeza;

                vector<Nodo*> encontrados;

                do {
                    if (actual->dato.estaOcupado &&
                        actual->dato.cedulaCliente == cedula) {
                        encontrados.push_back(actual);
                    }
                    actual = actual->siguiente;
                } while (actual != cabeza);

                if (encontrados.empty()) {
                    cout << "Error: No se encontro reserva para esa cedula.\n";
                    system("pause");
                    break;
                }

                if (encontrados.size() == 1) {
                    int numAsiento = encontrados[0]->dato.numeroAsiento;

                    cout << "Cancelando asiento " << numAsiento << "...\n";
                    bool exito = ConcurrenciaOCC::cancelarAsientoOCC(numAsiento);

                    if (exito) {
                        cout << "Reserva cancelada exitosamente.\n";
                        Persistencia::cargarReservas(miBoleteria);
                    } else {
                        cout << "Error al cancelar la reserva.\n";
                    }

                    system("pause");
                    break;
                }

                cout << "\nLa cedula tiene " << encontrados.size() << " reservas.\n";
                int cancelarTodo = pedirConfirmacion("Cancelar todas?");

                if (cancelarTodo == 1) {

                    bool error = false;

                    for (Nodo* nodo : encontrados) {
                        int numAsiento = nodo->dato.numeroAsiento;
                        if (!ConcurrenciaOCC::cancelarAsientoOCC(numAsiento)) {
                            cout << "Error al cancelar el asiento " << numAsiento << ".\n";
                            error = true;
                        }
                    }

                    if (!error) {
                        cout << "Reservas canceladas exitosamente.\n";
                        Persistencia::cargarReservas(miBoleteria);
                    }

                    system("pause");
                    break;
                }
                bool seguir = true;

                while (seguir && !encontrados.empty()) {

                    cout << "\nReservas encontradas:\n";
                    for (size_t i = 0; i < encontrados.size(); i++) {
                        cout << "[" << i + 1 << "] Asiento "
                            << encontrados[i]->dato.numeroAsiento
                            << " (" << encontrados[i]->dato.getCategoria() << ")\n";
                    }

                    cout << "Seleccione reserva a cancelar: ";
                    int idx = ingresarEntero() - 1;

                    if (idx < 0 || idx >= (int)encontrados.size()) {
                        cout << "Opcion invalida.\n";
                    } else {
                        int numAsiento = encontrados[idx]->dato.numeroAsiento;

                        cout << "Cancelando asiento " << numAsiento << "...\n";
                        bool exito = ConcurrenciaOCC::cancelarAsientoOCC(numAsiento);

                        if (exito) {
                            cout << "Reserva cancelada.\n";
                            encontrados.erase(encontrados.begin() + idx);
                            Persistencia::cargarReservas(miBoleteria);
                        } else {
                            cout << "Error al cancelar la reserva.\n";
                        }
                    }

                    if (encontrados.empty()) {
                        cout << "No quedan reservas para esta cedula.\n";
                        break;
                    }

                    seguir = pedirConfirmacion("Cancelar otro?");
                }

                system("pause");
                break;
            }

        case 3: {
            system("cls");
            Persistencia::cargarReservas(miBoleteria); 
            miBoleteria.mostrarDesdeInicio();
            system("pause");
            break;
        }

        case 4: {
            system("cls");
            Persistencia::cargarReservas(miBoleteria); 
            string cedula = ingresarCedula();

            Nodo* a = miBoleteria.getCabeza();
            do {
                if (a->dato.estaOcupado && a->dato.cedulaCliente == cedula)
                    a->dato.mostrarDatos();
                a = a->siguiente;
            } while (a != miBoleteria.getCabeza());

            system("pause");
            break;
        }

        case 5: {
            system("cls");
            Persistencia::cargarReservas(miBoleteria);

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

            if (pedirConfirmacion("Cambiar cedula?") == 1) {
                cout << "Ingrese la nueva cedula: ";
                cedulaTemp = ingresarCedula();
            }

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
    
                    ConcurrenciaOCC::cancelarAsientoOCC(nodoEncontrado->dato.numeroAsiento);
                    
                    bool exito = ConcurrenciaOCC::actualizarAsiento(
                        nuevoAsiento,
                        "OCUPADO",
                        nombreTemp,
                        cedulaTemp,
                        nuevoNodo->dato.getCategoria(),
                        Tiempo::obtenerFecha(),
                        Tiempo::obtenerHora()
                    );

                    if (exito) {
                        Persistencia::cargarReservas(miBoleteria);
                        cout << "\nAsiento cambiado correctamente.\n";
                    } else {
                        cout << "\nError: El asiento acaba de ser ocupado por otro usuario.\n";
                        cout << "    Refrescando disponibilidad...\n";
                        Persistencia::cargarReservas(miBoleteria);
                    }
                    
                    system("pause");
                    break;
                }
            }

            nodoEncontrado->dato.nombreCliente = nombreTemp;
            nodoEncontrado->dato.cedulaCliente = cedulaTemp;

            Persistencia::guardarReservas(miBoleteria);

            cout << "\nDatos actualizados correctamente.\n";
            system("pause");
            break;
        }


        case 6: {
            system("cls");
            Persistencia::cargarReservas(miBoleteria); 

            cout << "--- NOMBRES CON CARACTERES ORDENADOS ---\n\n";

            if (miBoleteria.estaVacia()) {
                cout << "No hay reservas registradas.\n";
                system("pause");
                break;
            }

            Nodo* actual = miBoleteria.getCabeza();
            bool hayReservas = false;

            cout << "Procesando nombres de la lista en memoria...\n\n";

            do {
                if (actual->dato.estaOcupado) {
                    hayReservas = true;
                    mostrarNombreOrdenado(actual->dato.nombreCliente);
                }
                actual = actual->siguiente;
            } while (actual != miBoleteria.getCabeza());

            if (!hayReservas) {
                cout << "La lista esta vacia de reservas activas.\n";
            }

            system("pause");
            break;
        }

        case 7: {
            system("cls");
            Persistencia::cargarReservas(miBoleteria);
            system("pause");
            break;
        }

        case 8:
            menu = false;
                system("cls");
                cout << "Gracias por usar el sistema." << endl;
            break;
        }
    }
}
