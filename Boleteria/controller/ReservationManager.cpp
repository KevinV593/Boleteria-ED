#include "ReservationManager.hpp"
#include "../utils/Validaciones.hpp"
#include "../utils/Persistencia.hpp"
#include "../utils/ConcurrenciaOCC.hpp"
#include "../utils/Tiempo.hpp"
#include <iostream>
#include <windows.h>

using namespace std;

string ReservationManager::obtenerNombrePorCedula(ListaCircularDoble &lista, const string &cedula)
{
    if (lista.estaVacia())
        return "";

    Nodo *actual = lista.getCabeza();
    do
    {
        if (actual->dato.estaOcupado && actual->dato.cedulaCliente == cedula)
            return actual->dato.nombreCliente;
        actual = actual->siguiente;
    } while (actual != lista.getCabeza());

    return "";
}

void ReservationManager::buscarReservasPorCedula(ListaCircularDoble &boleteria)
{
    system("cls");
    Persistencia::cargarReservas(boleteria);

    cout << "Ingrese la cedula: ";
    string cedula = ingresarCedula();

    Nodo *a = boleteria.getCabeza();
    do
    {
        if (a->dato.estaOcupado && a->dato.cedulaCliente == cedula)
            a->dato.mostrarDatos();
        a = a->siguiente;
    } while (a != boleteria.getCabeza());

    system("pause");
}

void ReservationManager::reservarAsiento(ListaCircularDoble &boleteria)
{
    system("cls");
    cout << "--- RESERVAR ASIENTO ---\n\n";
    Persistencia::cargarReservas(boleteria);

    cout << "Ingrese el numero de asiento: ";
    int num = ingresarEntero();

    Nodo *n = boleteria.buscar(num);

    if (!n)
    {
        cout << "Error: Asiento inexistente.\n";
        system("pause");
        return;
    }

    if (n->dato.estaOcupado)
    {
        cout << "Error: Asiento ya reservado.\n";
        system("pause");
        return;
    }

    cout << "Asiento disponible.\n";

    cout << "Ingrese la cedula del cliente: ";
    string cedula = ingresarCedula();

    string nombre = obtenerNombrePorCedula(boleteria, cedula);

    if (!nombre.empty())
    {
        cout << "Cedula ya registrada como: " << nombre << endl;
    }
    else
    {
        cout << "Ingrese el nombre del cliente: ";
        nombre = ingresarNombre();
    }

    cout << "\n Procesando reserva con verificacion de concurrencia...\n";

    bool exito = ConcurrenciaOCC::actualizarAsiento(
        num,
        "OCUPADO",
        nombre,
        cedula,
        n->dato.getCategoria(),
        Tiempo::obtenerFecha(),
        Tiempo::obtenerHora());

    if (exito)
    {
        Persistencia::cargarReservas(boleteria);
    }
    else
    {
        cout << "\n Refrescando disponibilidad de asientos...\n";
        Persistencia::cargarReservas(boleteria);
        cout << "    Por favor, elige otro asiento.\n";
    }

    system("pause");
}

void ReservationManager::cancelarReserva(ListaCircularDoble &boleteria)
{
    system("cls");
    cout << "--- CANCELAR RESERVA ---\n\n";

    Persistencia::cargarReservas(boleteria);

    if (boleteria.estaVacia())
    {
        cout << "No hay reservas registradas.\n";
        system("pause");
        return;
    }

    cout << "Ingrese la cedula: ";
    string cedula = ingresarCedula();

    Nodo *cabeza = boleteria.getCabeza();
    Nodo *actual = cabeza;

    vector<Nodo *> encontrados;

    do
    {
        if (actual->dato.estaOcupado &&
            actual->dato.cedulaCliente == cedula)
        {
            encontrados.push_back(actual);
        }
        actual = actual->siguiente;
    } while (actual != cabeza);

    if (encontrados.empty())
    {
        cout << "Error: No se encontro reserva para esa cedula.\n";
        system("pause");
        return;
    }

    if (encontrados.size() == 1)
    {
        int numAsiento = encontrados[0]->dato.numeroAsiento;

        cout << "Cancelando asiento " << numAsiento << "...\n";
        bool exito = ConcurrenciaOCC::cancelarAsientoOCC(numAsiento);

        if (exito)
        {
            cout << "Reserva cancelada exitosamente.\n";
            Persistencia::cargarReservas(boleteria);
        }
        else
        {
            cout << "Error al cancelar la reserva.\n";
        }

        system("pause");
        return;
    }

    cout << "\nLa cedula tiene " << encontrados.size() << " reservas.\n";

    cout << "Cancelar todas? (1 = Si / 0 = No): ";
    int cancelarTodo = ingresarEntero();

    if (cancelarTodo == 1)
    {
        bool error = false;

        for (Nodo *nodo : encontrados)
        {
            int numAsiento = nodo->dato.numeroAsiento;
            if (!ConcurrenciaOCC::cancelarAsientoOCC(numAsiento))
            {
                cout << "Error al cancelar el asiento " << numAsiento << ".\n";
                error = true;
            }
        }

        if (!error)
        {
            cout << "Reservas canceladas exitosamente.\n";
            Persistencia::cargarReservas(boleteria);
        }

        system("pause");
        return;
    }

    bool seguir = true;

    while (seguir && !encontrados.empty())
    {
        cout << "\nReservas encontradas:\n";
        for (size_t i = 0; i < encontrados.size(); i++)
        {
            cout << "[" << i + 1 << "] Asiento "
                 << encontrados[i]->dato.numeroAsiento
                 << " (" << encontrados[i]->dato.getCategoria() << ")\n";
        }

        cout << "Seleccione reserva a cancelar: ";
        int idx = ingresarEntero() - 1;

        if (idx < 0 || idx >= (int)encontrados.size())
        {
            cout << "Opcion invalida.\n";
        }
        else
        {
            int numAsiento = encontrados[idx]->dato.numeroAsiento;

            cout << "Cancelando asiento " << numAsiento << "...\n";
            bool exito = ConcurrenciaOCC::cancelarAsientoOCC(numAsiento);

            if (exito)
            {
                cout << "Reserva cancelada.\n";
                encontrados.erase(encontrados.begin() + idx);
                Persistencia::cargarReservas(boleteria);
            }
            else
            {
                cout << "Error al cancelar la reserva.\n";
            }
        }

        if (encontrados.empty())
        {
            cout << "No quedan reservas para esta cedula.\n";
            break;
        }

        cout << "Cancelar otro? (1 = Si / 0 = No): ";
        seguir = ingresarEntero() == 1;
    }

    system("pause");
}

void ReservationManager::actualizarReserva(ListaCircularDoble &boleteria)
{
    system("cls");
    Persistencia::cargarReservas(boleteria);

    cout << "--- ACTUALIZAR RESERVA ---\n\n";

    cout << "Ingrese la cedula del cliente: ";
    string cedula = ingresarCedula();

    if (boleteria.estaVacia())
    {
        cout << "No hay asientos.\n";
        system("pause");
        return;
    }

    Nodo *actual = boleteria.getCabeza();
    Nodo *nodoEncontrado = nullptr;

    do
    {
        if (actual->dato.estaOcupado &&
            actual->dato.cedulaCliente == cedula)
        {
            nodoEncontrado = actual;
            break;
        }
        actual = actual->siguiente;
    } while (actual != boleteria.getCabeza());

    if (!nodoEncontrado)
    {
        cout << "No se encontro reserva.\n";
        system("pause");
        return;
    }

    cout << "--- Datos actuales ---\n";
    nodoEncontrado->dato.mostrarDatos();
    cout << "\n";

    string nombreTemp = nodoEncontrado->dato.nombreCliente;
    string cedulaTemp = nodoEncontrado->dato.cedulaCliente;

    cout << "Cambiar nombre? (1 = Si / 0 = No): ";
    if (ingresarEntero() == 1)
    {
        cout << "Ingrese el nuevo nombre: ";
        nombreTemp = ingresarNombre();
    }

    cout << "Cambiar cedula? (1 = Si / 0 = No): ";
    if (ingresarEntero() == 1)
    {
        cout << "Ingrese la nueva cedula: ";
        cedulaTemp = ingresarCedula();
    }

    cout << "Cambiar asiento? (1 = Si / 0 = No): ";
    if (ingresarEntero() == 1)
    {
        cout << "Ingrese el nuevo numero de asiento: ";
        int nuevoAsiento = ingresarEntero();

        Nodo *nuevoNodo = boleteria.buscar(nuevoAsiento);

        if (!nuevoNodo)
        {
            cout << "Ese asiento no existe.\n";
        }
        else if (nuevoNodo->dato.estaOcupado)
        {
            cout << "Ese asiento ya esta ocupado.\n";
        }
        else
        {
            ConcurrenciaOCC::cancelarAsientoOCC(nodoEncontrado->dato.numeroAsiento);

            bool exito = ConcurrenciaOCC::actualizarAsiento(
                nuevoAsiento,
                "OCUPADO",
                nombreTemp,
                cedulaTemp,
                nuevoNodo->dato.getCategoria(),
                Tiempo::obtenerFecha(),
                Tiempo::obtenerHora());

            if (exito)
            {
                Persistencia::cargarReservas(boleteria);
                cout << "\nAsiento cambiado correctamente.\n";
            }
            else
            {
                cout << "\nError: El asiento acaba de ser ocupado por otro usuario.\n";
                cout << "    Refrescando disponibilidad...\n";
                Persistencia::cargarReservas(boleteria);
            }

            system("pause");
            return;
        }
    }

    nodoEncontrado->dato.nombreCliente = nombreTemp;
    nodoEncontrado->dato.cedulaCliente = cedulaTemp;

    Persistencia::guardarReservas(boleteria);

    cout << "\nDatos actualizados correctamente.\n";
    system("pause");
}
