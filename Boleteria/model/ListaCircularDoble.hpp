#pragma once
#include "Nodo.hpp"

class ListaCircularDoble {
private:
    Nodo* cabeza;

public:
    ListaCircularDoble();
    ~ListaCircularDoble();

    void insertarPorCabeza(const Boleto& boleto);
    void insertarPorFinal(const Boleto& boleto);    
    
    void eliminar(int numeroAsiento);
    Nodo* buscar(int numeroAsiento);

    void mostrarDesdeInicio();

    bool estaVacia();

};