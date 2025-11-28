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

int pedirConfirmacion(const string& mensaje) {
    int opcion;
    while (true) {
        cout << mensaje << " (1 = Si / 0 = No): ";
        opcion = ingresarEntero();
        if (opcion == 1 || opcion == 0) return opcion;
        cout << "Entrada invalida, ingrese 1 o 0.\n";
    }
}

// Necesario para la función tolower
#include <cctype> 

// Estructura auxiliar para manejar cada letra como un nodo
struct NodoLetra {
    char letra;
    NodoLetra* siguiente;
};

void agregarLetra(NodoLetra*& cabeza, NodoLetra*& fin, char c) {
    NodoLetra* nuevo = new NodoLetra;
    nuevo->letra = c;
    nuevo->siguiente = nullptr;

    if (cabeza == nullptr) {
        cabeza = nuevo;
        fin = nuevo;
    } else {
        fin->siguiente = nuevo;
        fin = nuevo;
    }
}

// AYUDA: Ordena una lista enlazada dada (Burbuja)
void ordenarSubLista(NodoLetra* cabeza) {
    if (cabeza == nullptr) return;

    bool ordenado = false;
    while (!ordenado) {
        ordenado = true;
        NodoLetra* actual = cabeza;

        while (actual->siguiente != nullptr) {
            // Comparamos en minúsculas
            char c1 = tolower(actual->letra);
            char c2 = tolower(actual->siguiente->letra);

            if (c1 > c2) {
                // Swap de datos
                char temp = actual->letra;
                actual->letra = actual->siguiente->letra;
                actual->siguiente->letra = temp;
                ordenado = false;
            }
            actual = actual->siguiente;
        }
    }
}

void liberarSubLista(NodoLetra*& cabeza) {
    while (cabeza != nullptr) {
        NodoLetra* aux = cabeza;
        cabeza = cabeza->siguiente;
        delete aux;
    }
}

void procesarPorGrupos(string nombre, int tamanoGrupo) {
    if (tamanoGrupo <= 0) {
        cout << "El grupo debe ser mayor a 0." << endl;
        return;
    }

    cout << "Original: " << nombre << " \t-> Resultado: ";

    const char* lector = nombre.c_str(); // Puntero al string original

    // Mientras no lleguemos al final del string '\0'
    while (*lector != '\0') {
        
        // 1. Crear una sub-lista para el grupo actual
        NodoLetra* cabezaGrupo = nullptr;
        NodoLetra* finGrupo = nullptr;
        int contador = 0;

        // Llenamos la sub-lista hasta completar el tamaño O hasta que se acabe el nombre
        while (contador < tamanoGrupo && *lector != '\0') {
            agregarLetra(cabezaGrupo, finGrupo, *lector);
            lector++; // Avanzamos puntero del string original
            contador++;
        }

        // 2. Ordenar ESTE grupo especifico
        ordenarSubLista(cabezaGrupo);

        // 3. Imprimir el grupo ya ordenado
        NodoLetra* imprimir = cabezaGrupo;
        while (imprimir != nullptr) {
            cout << imprimir->letra;
            imprimir = imprimir->siguiente;
        }

        // 4. Limpiar memoria de este grupo antes de pasar al siguiente
        liberarSubLista(cabezaGrupo);
    }
    cout << endl;
}



void mostrarNombreOrdenado(string nombreOriginal) {
    // Si el nombre está vacío, no hacemos nada
    if (nombreOriginal.length() == 0) return;

    // --- PASO 1: CONVERTIR STRING A LISTA ENLAZADA ---
    // (Esto evita el uso de arrays y corchetes)
    NodoLetra* cabeza = nullptr;
    NodoLetra* fin = nullptr;
    
    // Usamos un puntero para leer el string original sin usar [i]
    const char* lector = nombreOriginal.c_str();

    while (*lector != '\0') {
        NodoLetra* nuevo = new NodoLetra; // new simple (sin corchetes)
        nuevo->letra = *lector;
        nuevo->siguiente = nullptr;

        if (cabeza == nullptr) {
            cabeza = nuevo;
            fin = nuevo;
        } else {
            fin->siguiente = nuevo;
            fin = nuevo;
        }
        lector++; // Aritmética de punteros: avanza a la siguiente letra
    }

    // --- PASO 2: ORDENAMIENTO BURBUJA EN LA LISTA ---
    if (cabeza != nullptr) {
        bool ordenado = false;
        while (!ordenado) {
            ordenado = true;
            NodoLetra* actual = cabeza;

            // Recorremos mientras haya un nodo siguiente
            while (actual->siguiente != nullptr) {
                
                // Convertimos a minúscula solo para la comparación (A vs a)
                char c1 = tolower(actual->letra);
                char c2 = tolower(actual->siguiente->letra);

                if (c1 > c2) {
                    // Intercambiamos SOLO el contenido (char), no los nodos
                    char temp = actual->letra;
                    actual->letra = actual->siguiente->letra;
                    actual->siguiente->letra = temp;

                    ordenado = false; // Hubo cambios, repetir ciclo
                }
                actual = actual->siguiente;
            }
        }
    }

    // --- PASO 3: IMPRIMIR ---
    cout << "Original: " << nombreOriginal << " \t-> Transformado: ";
    
    NodoLetra* imprimir = cabeza;
    while (imprimir != nullptr) {
        cout << imprimir->letra;
        imprimir = imprimir->siguiente;
    }
    cout << endl;
    while (cabeza != nullptr) {
        NodoLetra* aux = cabeza;
        cabeza = cabeza->siguiente;
        delete aux; 
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
                system("cls");
                cout << "--- NOMBRES CON CARACTERES ORDENADOS ---\n\n";

                if (miBoleteria.estaVacia()) {
                    cout << "No hay reservas registradas.\n";
                    system("pause");
                    break;
                }

                Nodo* actual = miBoleteria.getCabeza();
                bool hayReservas = false;

                cout << "Procesando nombres de la lista en memoria...\n\n";

                // Recorremos la lista circular
                do {
                    if (actual->dato.estaOcupado) {
                        hayReservas = true;
                        // Llamamos a la función que creamos arriba
                        mostrarNombreOrdenado(actual->dato.nombreCliente);
                    }
                    actual = actual->siguiente;
                } while (actual != miBoleteria.getCabeza());

                if (!hayReservas) {
                    cout << "La lista esta vacia de reservas activas.\n";
                }

                cout << "\n";
                system("pause");
                break;
            }

            case 7: {
                system("cls");
                cout << "--- ORDENAMIENTO POR GRUPOS ---\n\n";
                
                if (miBoleteria.estaVacia()) {
                    cout << "No hay reservas.\n";
                    system("pause");
                    break;
                }

                cout << "Ingrese el tamano del grupo para agrupar las letras: ";
                int k = ingresarEntero();

                cout << "\nProcesando...\n";
                
                Nodo* actual = miBoleteria.getCabeza();
                bool hayReservas = false;

                do {
                    if (actual->dato.estaOcupado) {
                        hayReservas = true;
                        procesarPorGrupos(actual->dato.nombreCliente, k);
                    }
                    actual = actual->siguiente;
                } while (actual != miBoleteria.getCabeza());

                if (!hayReservas) cout << "Todas las reservas estan vacias.\n";
                
                cout << "\n";
                system("pause");
                break;
            }

            case 8: {
                menu = false;
                system("cls");
                cout << "Gracias por usar el sistema." << endl;
                break;
            }
        }
    }
}