#pragma once
#include <fstream>
#include <iostream>
#include <windows.h>
#include "../model/ListaCircularDoble.hpp"
#include "../model/Boleto.hpp"
#include "../model/Nodo.hpp"
#include "../utils/hash.hpp" 


using namespace std;

class Persistencia {
private:
    static HANDLE abrirArchivoExclusivo(const string& archivo, DWORD acceso) {
        return CreateFileA(
            archivo.c_str(),
            acceso,
            0,                  // 🚫 NO SHARE → lock real entre procesos
            nullptr,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );
    }

public:
    static bool guardarReservas(
        ListaCircularDoble& lista,
        const string& archivo = "server/reservas.txt"
    ) {
        // 🔒 lock exclusivo
        HANDLE hFile = abrirArchivoExclusivo(archivo, GENERIC_WRITE);
        if (hFile == INVALID_HANDLE_VALUE) {
            cout << "Otro proceso esta guardando. Operacion cancelada.\n";
            return false;
        }

        CloseHandle(hFile);

        ofstream out(archivo, ios::trunc);
        if (!out.is_open()) {
            return false;
        }

        if (!lista.estaVacia()) {
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
        }

        out.close();

        string archivoHash = "server/reservas.hash"; 
        guardarHash(archivo, archivoHash);

        return true;
    }

    static bool cargarReservas(
        ListaCircularDoble& lista,
        const string& archivo = "server/reservas.txt"
    ) {
        // lectura permisiva: se puede leer mientras otro escribe SOLO si no hay lock
        HANDLE hFile = CreateFileA(
            archivo.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (hFile == INVALID_HANDLE_VALUE) {
            return false;
        }

        CloseHandle(hFile);

        ifstream in(archivo);
        if (!in.is_open()) return false;

        string linea;
        while (getline(in, linea)) {
            if (linea.empty()) continue;

            int numero = 0;
            string categoriaStr, nombre, cedula, fecha, hora, ocupadoStr;

            size_t pos;
            int campo = 0;
            string temp = linea;

            while ((pos = temp.find(';')) != string::npos) {
                string token = temp.substr(0, pos);
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

            ocupadoStr = temp;
            bool estaOcupado = (stoi(ocupadoStr) != 0);

            Nodo* nodo = lista.buscar(numero);
            if (nodo) {
                nodo->dato.nombreCliente = nombre;
                nodo->dato.cedulaCliente = cedula;
                nodo->dato.fechaReserva = fecha;
                nodo->dato.horaReserva = hora;
                nodo->dato.estaOcupado = estaOcupado;
            }
        }

        in.close();
        return true;
    }
};
