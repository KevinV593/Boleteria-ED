#pragma once
#include "../model/ListaCircularDoble.hpp"
#include "../model/Boleto.hpp"
#include "../model/TipoAsiento.hpp"

class EventManager
{
public:
    // Inicializa la boletería con asientos distribuidos por categoría
    static void inicializarEvento(ListaCircularDoble &boleteria, int totalAsientos);
};
