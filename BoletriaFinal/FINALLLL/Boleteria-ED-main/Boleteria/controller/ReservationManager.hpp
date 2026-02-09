#pragma once
#include "../model/ListaCircularDoble.hpp"
#include "../model/Nodo.hpp"
#include <vector>
#include <string>

using namespace std;

class ReservationManager
{
public:

static string obtenerNombrePorCedula(ListaCircularDoble &lista, const string &cedula);


static void buscarReservasPorCedula(ListaCircularDoble &boleteria);


static void reservarAsiento(ListaCircularDoble &boleteria);


static void cancelarReserva(ListaCircularDoble &boleteria);


static void actualizarReserva(ListaCircularDoble &boleteria);
};