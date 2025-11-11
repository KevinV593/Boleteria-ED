#include "ListaCircularDoble.hpp" 
#include "Nodo.hpp"               
#include <iostream>          

using namespace std;

ListaCircularDoble::ListaCircularDoble() : cabeza(nullptr) {
}

ListaCircularDoble::~ListaCircularDoble() {
    // TODO: Agregar código para borrar todos los nodos
    // y evitar fugas de memoria.
}

bool ListaCircularDoble::estaVacia() {
    return cabeza == nullptr;
    //retorna TRUE si se cumple 
}



// --- Implementaciones de los otros métodos ---
// (Los agregaremos aquí, uno por uno)

void ListaCircularDoble::insertarPorCabeza(int valor) {
    Nodo* nuevoNodo = new Nodo(valor);

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

void ListaCircularDoble::eliminar(int valor) {
    // TODO: Implementar lógica de eliminación
}

Nodo* ListaCircularDoble::buscar(int valor) {
    // TODO: Implementar lógica de búsqueda
    return nullptr; // Temporal
}

void ListaCircularDoble::mostrarAdelante() {
    // TODO: Implementar lógica de mostrar
}

void ListaCircularDoble::mostrarAtras() {
    // TODO: Implementar lógica de mostrar
}