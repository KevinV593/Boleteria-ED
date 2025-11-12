#pragma once
#include <fstream>
#include <iostream>
#include "../model/ListaCircularDoble.hpp"
#include "../model/Boleto.hpp"
#include "Tiempo.hpp"

using namespace std;

class Persistencia {
public:
    static void guardarReservas(ListaCircularDoble& lista, const string& archivo = "reservas.txt") {
        ofstream out(archivo);
        if (!out.is_open()) {
            cout << "Error al abrir archivo para guardar reservas." << endl;
            return;
        }

        if (lista.estaVacia()) {
            out.close();
            return;
        }

        Nodo* actual = lista.getCabeza();
        do {
            Boleto& b = actual->dato;
            out << b.numeroAsiento << ";"
                << b.getCategoria() << ";"
                << b.nombreCliente << ";"
                << b.cedulaCliente << ";"
                << b.fechaReserva << ";"
                << b.horaReserva << ";"
                << b.estaOcupado << "\n";
            actual = actual->siguiente;
        } while (actual != lista.getCabeza());

        out.close();
    }

    static void cargarReservas(ListaCircularDoble& lista, const string& archivo = "reservas.txt") {
        ifstream in(archivo);
        if (!in.is_open()) return;

        string linea;
        while (getline(in, linea)) {
            if (linea.empty()) continue;

            int numero;
            string categoriaStr, nombre, cedula, fecha, hora, ocupadoStr;

            size_t pos = 0;
            string token;
            int campo = 0;
            string temp = linea;
            while ((pos = temp.find(';')) != string::npos) {
                token = temp.substr(0, pos);
                switch (campo) {
                    case 0: numero = stoi(token); break;
                    case 1: categoriaStr = token; break;
                    case 2: nombre = token; break;
                    case 3: cedula = token; break;
                    case 4: fecha = token; break;
                    case 5: hora = token; break;
                }
                temp.erase(0, pos + 1);
                campo++;
            }
            ocupadoStr = temp; // último campo
            bool estaOcupado = (stoi(ocupadoStr) != 0);

            Nodo* nodo = lista.buscar(numero);
            if (nodo != nullptr) { // <-- No filtramos por estaOcupado
                nodo->dato.nombreCliente = nombre;
                nodo->dato.cedulaCliente = cedula;
                nodo->dato.fechaReserva = fecha;
                nodo->dato.horaReserva = hora;
                nodo->dato.estaOcupado = estaOcupado;
            }
        }

        in.close();
    }
};
