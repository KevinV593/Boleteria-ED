#pragma once
#include <windows.h>
#include "../model/ListaCircularDoble.hpp"
#include "../model/Boleto.hpp"
#include "../model/TipoAsiento.hpp"
#include "../utils/Persistencia.hpp"

class Exhaustiva
{
public:
    static void ingresosTotales(ListaCircularDoble &boleteria);

    static void busquedaPorPatron(ListaCircularDoble &boleteria);

};