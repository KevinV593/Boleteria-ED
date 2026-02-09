#pragma once
#include "../model/ListaCircularDoble.hpp"

class MenuController
{
public:
    // Ejecuta el menú principal en un loop
    static void ejecutar(ListaCircularDoble &boleteria);

private:
    static void procesarOpcion(int opcion, ListaCircularDoble &boleteria);
};
