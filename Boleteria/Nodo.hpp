#pragma once

struct Nodo {
    int dato;
    Nodo *siguiente;
    Nodo *anterior;

    Nodo(int valor) : dato (valor), 
    siguiente (nullptr), 
    anterior (nullptr) {}
};