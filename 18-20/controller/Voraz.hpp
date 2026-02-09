#pragma once
#include "../model/ListaCircularDoble.hpp"
#include "../model/Boleto.hpp"
#include "../model/TipoAsiento.hpp"
#include "../utils/Persistencia.hpp"
#include "../utils/Validaciones.hpp"
#include "../controller/ReservationManager.hpp"
#include "../utils/ConcurrenciaOCC.hpp"

class Voraz
{
public:
    //1
    static void cambioDeMoneda();
    
    //2
    static void asignacionRapida(ListaCircularDoble &boleteria);
};