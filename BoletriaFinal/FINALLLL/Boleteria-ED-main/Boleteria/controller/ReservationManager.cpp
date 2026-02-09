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

    cout << "--- ACTUALIZAR RESERVA (MULTI-ASIENTO / MEMORIA DINAMICA) ---\n\n";

    if (boleteria.estaVacia())
    {
        cout << "No hay asientos registrados en el sistema.\n";
        system("pause");
        return;
    }

    cout << "Ingrese la cedula del cliente a buscar: ";
    string cedulaOriginal = ingresarCedula();

    // ---------------------------------------------------------
    // PASO 1: CONTAR RESERVAS
    // ---------------------------------------------------------
    int cantidadReservas = 0;
    Nodo* explorador = boleteria.getCabeza();
    
    do {
        if (explorador->dato.estaOcupado && explorador->dato.cedulaCliente == cedulaOriginal) {
            cantidadReservas++;
        }
        explorador = explorador->siguiente;
    } while (explorador != boleteria.getCabeza());

    if (cantidadReservas == 0)
    {
        cout << "\nNo se encontraron reservas para la cedula: " << cedulaOriginal << "\n";
        system("pause");
        return;
    }

    // ---------------------------------------------------------
    // PASO 2: CREAR ARREGLO DINÁMICO
    // ---------------------------------------------------------
    Nodo** misReservas = new Nodo*[cantidadReservas];

    // ---------------------------------------------------------
    // PASO 3: LLENAR EL ARREGLO
    // ---------------------------------------------------------
    explorador = boleteria.getCabeza();
    int indice = 0;
    do {
        if (explorador->dato.estaOcupado && explorador->dato.cedulaCliente == cedulaOriginal) {
            *(misReservas + indice) = explorador;
            indice++;
        }
        explorador = explorador->siguiente;
    } while (explorador != boleteria.getCabeza());

    // Mostrar resumen
    cout << "\nSe encontraron " << cantidadReservas << " boletos a nombre de " 
         << (*misReservas)->dato.nombreCliente << ".\n";
    
    for (int i = 0; i < cantidadReservas; i++) {
        Nodo* n = *(misReservas + i);
        cout << " - Asiento " << n->dato.numeroAsiento 
             << " (" << n->dato.getCategoria() << ")\n";
    }
    cout << "\n";

    // ---------------------------------------------------------
    // PASO 4: SOLICITAR NUEVOS DATOS (CON VALIDACIÓN DE INTEGRIDAD)
    // ---------------------------------------------------------
    string nuevoNombre = (*misReservas)->dato.nombreCliente;
    string nuevaCedula = (*misReservas)->dato.cedulaCliente;

    // A. CAMBIO DE NOMBRE
    cout << "Desea cambiar el NOMBRE del titular? (1 = Si / 0 = No): ";
    if (ingresarEntero() == 1) {
        nuevoNombre = ingresarNombre();
    }

    // B. CAMBIO DE CÉDULA (Aquí estaba el error de duplicidad)
    cout << "Desea cambiar la CEDULA del titular? (1 = Si / 0 = No): ";
    if (ingresarEntero() == 1) {
        cout << "Ingrese la nueva cedula: ";
        string inputCedula = ingresarCedula();

        // VALIDACIÓN CRÍTICA: ¿Esa cédula ya existe en el sistema?
        string nombrePropietario = obtenerNombrePorCedula(boleteria, inputCedula);

        // Si la cédula existe Y el nombre asociado NO es el que estamos intentando poner
        if (!nombrePropietario.empty() && nombrePropietario != nuevoNombre) {
            cout << "\n[ALERTA DE INTEGRIDAD] La cedula " << inputCedula 
                 << " ya esta registrada a nombre de: " << nombrePropietario << ".\n";
            
            cout << "No pueden existir dos nombres distintos para la misma cedula.\n";
            cout << "Opciones:\n";
            cout << "  1. Usar la cedula existente y cambiar el nombre a '" << nombrePropietario << "' (Fusionar).\n";
            cout << "  0. Cancelar el cambio de cedula.\n";
            cout << "Seleccione: ";
            
            if (ingresarEntero() == 1) {
                nuevaCedula = inputCedula;
                nuevoNombre = nombrePropietario; // FORZAMOS EL NOMBRE CORRECTO
                cout << " >> Datos actualizados: El titular ahora sera " << nuevoNombre << ".\n";
            } else {
                cout << " >> Cambio de cedula cancelado. Se mantiene la anterior.\n";
            }
        } 
        else {
            // Si la cédula es nueva (nadie la tiene) O es del mismo usuario, procedemos.
            nuevaCedula = inputCedula;
        }
    }

    cout << "\n--- PROCESANDO ACTUALIZACIONES PARA: " << nuevoNombre << " (" << nuevaCedula << ") ---\n";

    // ---------------------------------------------------------
    // PASO 5: ITERAR Y PROCESAR
    // ---------------------------------------------------------
    for (int i = 0; i < cantidadReservas; i++) 
    {
        Nodo* nodoActual = *(misReservas + i);
        int asientoViejo = nodoActual->dato.numeroAsiento;
        
        cout << "\n[" << (i+1) << "/" << cantidadReservas << "] ";
        cout << "Procesando Asiento " << asientoViejo << "...\n";

        cout << "   Desea CAMBIAR este asiento de lugar? (1 = Si, Mover / 0 = No, Mantener): ";
        int opcionMover = ingresarEntero();
        bool cambioExitoso = false; 

        if (opcionMover == 1) 
        {
            // === INTENTO DE MOVER ASIENTO ===
            cout << "   Ingrese el NUEVO numero de asiento deseado: ";
            int asientoNuevoID = ingresarEntero();

            Nodo* nodoDestino = boleteria.buscar(asientoNuevoID);

            if (!nodoDestino) {
                cout << "   [ERROR] El asiento " << asientoNuevoID << " no existe. Se mantendra el asiento " << asientoViejo << ".\n";
            } 
            else if (nodoDestino->dato.estaOcupado) {
                cout << "   [ERROR] El asiento " << asientoNuevoID << " ya esta ocupado. Se mantendra el asiento " << asientoViejo << ".\n";
            }
            else {
                // Intentar reservar el nuevo
                bool reservadoNuevo = ConcurrenciaOCC::actualizarAsiento(
                    asientoNuevoID, 
                    "OCUPADO", 
                    nuevoNombre, 
                    nuevaCedula, 
                    nodoDestino->dato.getCategoria(),
                    Tiempo::obtenerFecha(),
                    Tiempo::obtenerHora()
                );

                if (reservadoNuevo) {
                    bool liberadoViejo = ConcurrenciaOCC::cancelarAsientoOCC(asientoViejo);
                    
                    if (liberadoViejo) {
                        cout << "   [EXITO] Cambio realizado: Asiento " << asientoViejo 
                             << " -> Asiento " << asientoNuevoID << "\n";
                        
                        nodoDestino->dato.reservar(nuevoNombre, nuevaCedula);
                        nodoActual->dato.cancelar(); 
                        cambioExitoso = true;
                    } else {
                        cout << "   [ALERTA] Se reservo el nuevo pero fallo al liberar el viejo.\n";
                    }
                } else {
                    cout << "   [FALLO] No se pudo reservar el asiento " << asientoNuevoID << ".\n";
                }
            }
        }

        // === FALLBACK: ACTUALIZAR DATOS EN EL ASIENTO ACTUAL ===
        // Se ejecuta si no quiso mover O si el movimiento falló
        if (!cambioExitoso) 
        {
            // VALIDACIÓN ADICIONAL: Verificar si realmente cambiaron los datos para no escribir en vano
            if (nodoActual->dato.nombreCliente != nuevoNombre || nodoActual->dato.cedulaCliente != nuevaCedula) 
            {
                bool actualizado = ConcurrenciaOCC::sobreescribirDatosReserva(
                    asientoViejo,
                    nuevoNombre,
                    nuevaCedula,
                    nodoActual->dato.getCategoria(),
                    nodoActual->dato.fechaReserva,
                    nodoActual->dato.horaReserva
                );

                if (actualizado) {
                    cout << "   [DATOS ACTUALIZADOS] Asiento " << asientoViejo << " actualizado con: " << nuevoNombre << " - " << nuevaCedula << ".\n";
                    nodoActual->dato.nombreCliente = nuevoNombre;
                    nodoActual->dato.cedulaCliente = nuevaCedula;
                } else {
                    cout << "   [ERROR] Fallo al actualizar datos del asiento " << asientoViejo << ".\n";
                }
            }
            else {
                cout << "   [INFO] No hubo cambios en los datos ni en el asiento.\n";
            }
        }
    }

    // ---------------------------------------------------------
    // PASO 6: LIBERAR MEMORIA
    // ---------------------------------------------------------
    delete[] misReservas;

    Persistencia::cargarReservas(boleteria);
    cout << "\nProceso finalizado.\n";
    system("pause");
}