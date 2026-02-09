#pragma once
#include "../model/ListaCircularDoble.hpp"
#include "../model/Nodo.hpp"
#include <vector>
#include <string>

using namespace std;

class ReservationManager
{
public:
    // Obtiene el nombre de un cliente por su cédula
    static string obtenerNombrePorCedula(ListaCircularDoble &lista, const string &cedula);

    // Búsqueda de reservas por cédula
    static void buscarReservasPorCedula(ListaCircularDoble &boleteria);

    // Reservar un asiento
    static void reservarAsiento(ListaCircularDoble &boleteria);

    // Cancelar asiento(s) de un cliente
    static void cancelarReserva(ListaCircularDoble &boleteria);

    // Actualizar una reserva existente
    static void actualizarReserva(ListaCircularDoble &boleteria);
};