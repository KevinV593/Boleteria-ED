#pragma once
#include "../model/ListaCircularDoble.hpp"

class MenuUI
{
public:
    // Muestra el menú principal y retorna la opción seleccionada
    static int mostrarMenuPrincipal();

    // Muestra la lista de asientos
    static void mostrarAsientos(ListaCircularDoble &boleteria);
};
