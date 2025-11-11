#pragma once
#include "Nodo.hpp"

class ListaCircularDoble {
private:
    Nodo* cabeza;

public:
    ListaCircularDoble();
    ~ListaCircularDoble();

    void insertarPorCabeza(int valor);
    void eliminar(int valor);

    Nodo* buscar(int valor);

    void mostrarDesdeInicio();

    bool estaVacia();

};