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
    string id;
    
    Boleto(int num, TipoAsiento cat) : 
        numeroAsiento(num), 
        categoria(cat), 
        nombreCliente("N/A"), 
        estaOcupado(false) {
            string prefijo = PREFIJOS_CODIGO[categoria];
            this->id = prefijo + "-" + to_string(numeroAsiento);
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
            case PALCO: return "Palco";
            default: return "N/A";
        }
    }
    void mostrarDatos() {
        cout << "[" << id << "]";
        cout << "\t- Asiento: " << numeroAsiento;
        cout << "\t- Categoria: " << getCategoria();
        if (estaOcupado) {
            cout << "\t- RESERVADO: " << nombreCliente;
        } else {
            cout << "\t - DISPONIBLE";
        }
    }
};