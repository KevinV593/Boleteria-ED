#pragma once
#include <windows.h>
#include "../model/ListaCircularDoble.hpp"
#include "../model/Boleto.hpp"
#include "../model/TipoAsiento.hpp"
#include "../utils/Persistencia.hpp"

class Dinamica {
public:
    static void mochila01(int* precios, int* valores, int n, int capacidad);

    static void lcs(ListaCircularDoble &boleteria);
};


