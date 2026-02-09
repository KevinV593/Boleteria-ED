#pragma once
#include "../model/ListaCircularDoble.hpp"
#include "../model/Boleto.hpp"
#include "../model/TipoAsiento.hpp"

class EventManager
{
public:
    static void inicializarEvento(ListaCircularDoble &boleteria, int totalAsientos);
};
