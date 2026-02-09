#pragma once
#include <string>
#include <iostream>
#include "TipoAsiento.hpp"
#include "../utils/Tiempo.hpp"

using namespace std;

class Boleto {
public:
    
    int numeroAsiento;
    string nombreCliente;
    string cedulaCliente;
    bool estaOcupado;
    TipoAsiento categoria;
    string id;
    string fechaReserva; 
    string horaReserva;

    Boleto(int num, TipoAsiento cat) : 
        numeroAsiento(num), 
        categoria(cat), 
        nombreCliente("N/A"), 
        cedulaCliente("N/A"),
        fechaReserva("N/A"), 
        horaReserva("N/A"),
        estaOcupado(false) {
            string prefijo = PREFIJOS_CODIGO[categoria];
            this->id = prefijo + "-" + to_string(numeroAsiento);
    }

    void reservar(string cliente, string& cedula) {
        nombreCliente = cliente;
        cedulaCliente = cedula;
        fechaReserva = Tiempo::obtenerFecha();
        horaReserva = Tiempo::obtenerHora();
        estaOcupado = true;
    }

    void cancelar() {
        nombreCliente = "N/A";
        cedulaCliente = "N/A";
        fechaReserva = "N/A"; 
        horaReserva = "N/A"; 
        estaOcupado = false;
    }

    string getCategoria() {
        return NOMBRES_CATEGORIA[categoria];
    }

    void mostrarAsiento() {
        cout << "\t [" << id << "]";
        cout << "\t | Estado: ";
        if (estaOcupado) {
            cout << "RESERVADO ";
        } else {
            cout << "DISPONIBLE";
        }
    }

    void mostrarDatos() {
    cout << "[" << id << "]"; 
        if (estaOcupado) {
            cout << " | RESERVADO por: " << nombreCliente;
            cout << " (CI: " << cedulaCliente << ")";
            cout << " | Fecha: " << fechaReserva << " " << horaReserva << "\n";
        } else {
            cout << " | DISPONIBLE \n";
        }
    }
};