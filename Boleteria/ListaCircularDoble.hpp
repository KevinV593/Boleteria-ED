#pragma once
#include "Nodo.hpp"

class ListaCircularDoble {
private:
    Nodo *cabeza;

public:
    ListaCircularDoble();
    ~ListaCircularDoble();


    ///     METODOS     ///
    void insertar(int valor);
    void eliminar(int valor);

    Nodo* buscar(int valor);

    void mostrarDesdeInicio();
    void mostrarDesdeAtras();

    bool estaVacia();

};