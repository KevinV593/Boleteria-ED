#pragma once
#include "TipoAsiento.hpp"
#include <string>
#include <iostream>

using namespace std;

class Boleto {
public:
    
    int numeroAsiento;
    string nombreCliente;
    bool estaOcupado;
    TipoAsiento categoria;
    
    Boleto(int num, TipoAsiento cat) : 
        numeroAsiento(num), 
        categoria(cat), 
        nombreCliente("N/A"), 
        estaOcupado(false) {
    }

    void reservar(string cliente) {
        nombreCliente = cliente;
        estaOcupado = true;
    }

    void cancelar() {
        nombreCliente = "N/A";
        estaOcupado = false;
    }

    string getCategoria() {
        switch(categoria) {
            case GENERAL: return "General";
            case TRIBUNA: return "Tribuna";
            case PALCO:   return "Palco";
            default:      return "N/A";
        }
    }
    void mostrarDatos() {
        cout << "[Asiento " << numeroAsiento << "]";
        cout << " - Categoria: " << getCategoria();
        if (estaOcupado) {
            cout << " - RESERVADO (: " << nombreCliente << ")";
        } else {
            cout << " - (DISPONIBLE)";
        }
    }
};