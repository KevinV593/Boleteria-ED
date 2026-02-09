#include "Exhaustiva.hpp"

void Exhaustiva::ingresosTotales(ListaCircularDoble &boleteria)  
{
        system("cls");
        cout << "--- CALCULO DE INGRESOS ---\n";            
        double totalIngresos = 0.0;
        int cantidadReservas = 0;
            
            const double PRECIO_PAL = 100.0;
            const double PRECIO_TRI = 60.0;
            const double PRECIO_GEN = 40.0;

            Nodo* actual = boleteria.getCabeza();
            
            cout << "\nCalculando ingresos nodo por nodo...\n";
            
            do {
                if (actual->dato.estaOcupado) {
                    cantidadReservas++;
                    double precio = 0;
                    string catStr = "";

                    if (actual->dato.categoria == PALCO) { 
                        precio = PRECIO_PAL; 
                        catStr = "PAL";
                    }
                    else if (actual->dato.categoria == TRIBUNA) { 
                        precio = PRECIO_TRI; 
                        catStr = "TRI";
                    }
                    else if (actual->dato.categoria == GENERAL) { 
                        precio = PRECIO_GEN; 
                        catStr = "GEN";
                    }

                    totalIngresos += precio;
                }
                actual = actual->siguiente;
            } while (actual != boleteria.getCabeza());

            cout << "----------------------------------------\n";
            cout << "Entradas Vendidas: " << cantidadReservas << endl;
            cout << "INGRESOS TOTALES DEL EVENTO: $" << totalIngresos << endl;
            system("pause");

}

void Exhaustiva::busquedaPorPatron(ListaCircularDoble &boleteria)  
{
            system("cls");
            cout << "--- BUSQUEDA DE CLIENTE POR NOMBRE ---\n";                   
            string patron;
            cout << "\nIngrese texto a buscar (ej. 'Juan', 'Mar'): ";
            cin >> patron; 

            Persistencia::cargarReservas(boleteria);

            bool encontrado = false;
            Nodo* actual = boleteria.getCabeza();
            
            cout << "\nResultados de la busqueda exhaustiva:\n";
            
            do {
                if (actual->dato.estaOcupado) {
                    if (actual->dato.nombreCliente.find(patron) != string::npos) {
                        cout << " -> Encontrado en Asiento " << actual->dato.numeroAsiento 
                             << ": " << actual->dato.nombreCliente << endl;
                        encontrado = true;
                    }
                }
                actual = actual->siguiente;
            } while (actual != boleteria.getCabeza());

            if (!encontrado) {
                cout << "No se encontraron clientes que contengan '" << patron << "'.\n";
            }

        cout << "\n";
        system("pause");
}