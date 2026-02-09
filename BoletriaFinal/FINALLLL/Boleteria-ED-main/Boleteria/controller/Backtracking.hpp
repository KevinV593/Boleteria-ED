#pragma once
#include "../model/ListaCircularDoble.hpp"
#include "../model/Boleto.hpp"
#include "../model/TipoAsiento.hpp"
#include "../utils/Persistencia.hpp"
#include <iostream>

class Backtracking {
public:
    static void combinacionesAsientos(ListaCircularDoble &boleteria);
    static void permutacionesNombres(ListaCircularDoble &boleteria);

private:
    static int contarAsientosOcupados(ListaCircularDoble &boleteria);
    static void llenarAsientosOcupados(ListaCircularDoble &boleteria, int* ocupados);
    static int contarClientesOcupados(ListaCircularDoble &boleteria);
    static void llenarNombresOcupados(ListaCircularDoble &boleteria, std::string** nombres);

    static void combinaciones(int* arr, int n, int r, int* data, int index = 0, int i = 0);
    static void permutaciones(std::string** arr, int n, int i = 0);
};