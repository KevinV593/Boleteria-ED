#include "ListaCircularDoble.hpp" 
#include "Nodo.hpp"               
#include <iostream>          

using namespace std;

ListaCircularDoble::ListaCircularDoble() : cabeza(nullptr) {
}

ListaCircularDoble::~ListaCircularDoble() {
    if (estaVacia()) {
        return; 
    }
    
    Nodo* actual = cabeza;
    Nodo* siguiente = nullptr;

    cout << "\nLimpiando memoria (ejecutando destructor)..." << endl;
    
    Nodo* ultimo = cabeza->anterior;

    while (actual != ultimo) {
        siguiente = actual->siguiente; 
        cout << "Borrando Asiento " << actual->dato.numeroAsiento << endl;
        delete actual; // Borramos el actual
        actual = siguiente; // Avanzamos al siguiente
    }

    cout << "Borrando Asiento " << ultimo->dato.numeroAsiento << endl;
    delete ultimo;

    cabeza = nullptr; // Marcamos la lista como vacía
}

bool ListaCircularDoble::estaVacia() {
    return cabeza == nullptr;
    //retorna TRUE si se cumple 
}

void ListaCircularDoble::insertarPorCabeza(const Boleto& boleto) {
    Nodo* nuevoNodo = new Nodo(boleto);

    if (estaVacia()) {
        cabeza = nuevoNodo;
        nuevoNodo->siguiente = cabeza;
        nuevoNodo->anterior = cabeza;
    } else {
        Nodo* cola = cabeza->anterior;
        nuevoNodo->siguiente = cabeza;
        nuevoNodo->anterior = cola;
        cola->siguiente = nuevoNodo;
        cabeza->anterior = nuevoNodo;
        cabeza = nuevoNodo;
    }
}

void ListaCircularDoble::insertarPorFinal(const Boleto& boleto) {
    Nodo* nuevoNodo = new Nodo(boleto);

    if (estaVacia()) {
        // Caso lista vacía: igual que antes
        cabeza = nuevoNodo;
        nuevoNodo->siguiente = cabeza;
        nuevoNodo->anterior = cabeza;
    } else {

        Nodo* cola = cabeza->anterior; 
        
        nuevoNodo->siguiente = cabeza;  
        nuevoNodo->anterior = cola;     
        
        cola->siguiente = nuevoNodo;    
        cabeza->anterior = nuevoNodo;   

    }
}

// --- Implementación de Eliminar (Corregida) ---
void ListaCircularDoble::eliminar(int numeroAsiento) {
    
    // PASO 1: Usar 'buscar' para encontrar el nodo.
    Nodo* nodoAEliminar = buscar(numeroAsiento);

    // PASO 2: Manejar el caso si no se encuentra.
    if (nodoAEliminar == nullptr) {
        cout << "No se puede eliminar: El asiento " << numeroAsiento << " no existe." << endl;
        return;
    }

    // --- PASO 3: El nodo SÍ existe. Ahora hay que desconectarlo ---

    // Sub-caso 3.1: Es el ÚNICO nodo en la lista
    // (Si su 'siguiente' apunta a sí mismo, es el único)
    if (nodoAEliminar->siguiente == nodoAEliminar) {
        cabeza = nullptr; // La lista queda vacía
    }
    // Sub-caso 3.2: Hay MÚLTIPLES nodos
    else {
        // 1. "Saltar" el nodo, conectando a sus vecinos entre sí.
        Nodo* anterior = nodoAEliminar->anterior;
        Nodo* siguiente = nodoAEliminar->siguiente;

        anterior->siguiente = siguiente;
        siguiente->anterior = anterior;

        // 2. ¿Justo borramos la 'cabeza'?
        //    Si es así, debemos mover la 'cabeza' al siguiente nodo.
        if (nodoAEliminar == cabeza) {
            cabeza = siguiente;
        }
    }

    // --- PASO 4: Liberar la memoria ---
    delete nodoAEliminar; 
    cout << "Asiento " << numeroAsiento << " eliminado (liberado) correctamente." << endl;
}

Nodo* ListaCircularDoble::buscar(int numeroAsiento) {

    if (estaVacia()) {
        return nullptr;
    }

    Nodo* actual = cabeza;

    do {
        // ¿Es este el nodo que buscamos?
        if (actual->dato.numeroAsiento == numeroAsiento) {
            return actual;
        }
        actual = actual->siguiente;
    } while (actual != cabeza);
    return nullptr;
}

void ListaCircularDoble::mostrarDesdeInicio() {
if (estaVacia()) {
        cout << "Lista (->): [ Vacía ]" << endl;
        return;
    }

    Nodo* actual = cabeza;
    cout << "Asientos del Evento:\n";
    do {
        actual->dato.mostrarAsiento();
        cout << "\n"; 
        actual = actual->siguiente;
    } while (actual != cabeza);
}

Nodo* ListaCircularDoble::getCabeza() {
    return cabeza;
};