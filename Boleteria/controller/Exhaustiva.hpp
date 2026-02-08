#pragma once
#include <windows.h>
#include "../model/ListaCircularDoble.hpp"
#include "../model/Boleto.hpp"
#include "../model/TipoAsiento.hpp"
#include "../utils/Persistencia.hpp"

class Exhaustiva
{
public:
    //1
    static void ingresosTotales(ListaCircularDoble &boleteria);

    //2
    static void busquedaPorPatron(ListaCircularDoble &boleteria);

};