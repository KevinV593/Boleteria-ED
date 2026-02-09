#pragma once
#include "MenuController.h"

// Función de entrada para compatibilidad con main.cpp
void menuBoletosMain(ListaCircularDoble &boleteria)
{
    MenuController::ejecutar(boleteria);
}
