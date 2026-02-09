#pragma once
#include "../model/ListaCircularDoble.hpp"
#include "../model/Boleto.hpp"
#include "../model/TipoAsiento.hpp"

class DivideYVenceras
{
public:

    static void mergePunteros(int* arr, int l, int m, int r);
    static void mergeSortPunteros(int* arr, int l, int r);
    
    static int binarySearchPunteros(int* arr, int l, int r, int x);
};