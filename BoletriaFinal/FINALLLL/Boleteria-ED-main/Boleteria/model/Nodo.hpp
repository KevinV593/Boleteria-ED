#pragma once
#include "Boleto.hpp"
struct Nodo {
    Boleto dato;
    Nodo* siguiente;
    Nodo* anterior;

    Nodo(const Boleto& boleto) : 
    dato (boleto), 
    siguiente (nullptr), 
    anterior (nullptr) {}
};