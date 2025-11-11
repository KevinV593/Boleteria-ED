#pragma once
#include <string>
#include <iostream>

using namespace std;

class Boleto {
public:
    
    int numeroAsiento;
    string nombreCliente;
    bool estaOcupado;
    
    Boleto(int num) {
        numeroAsiento = num;
        nombreCliente = "N/A";
        estaOcupado = false;
    }

    void reservar(string cliente) {
        nombreCliente = cliente;
        estaOcupado = true;
    }

    void cancelar() {
        nombreCliente = "N/A";
        estaOcupado = false;
    }

    void mostrarDatos() {
        cout << "[Asiento " << numeroAsiento << "]";
        if (estaOcupado) {
            cout << " - Ocupado por: " << nombreCliente;
        } else {
            cout << " - (DISPONIBLE)";
        }
    }
};