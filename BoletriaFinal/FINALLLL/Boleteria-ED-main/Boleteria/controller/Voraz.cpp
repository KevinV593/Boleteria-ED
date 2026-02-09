#include "Voraz.hpp"

void Voraz::cambioDeMoneda() {
        system("cls");
        cout << "--- Simulacion de Caja (Cambio de Moneda) ---\n";

            cout << "\nTotal a pagar: $"; int costo = ingresarEntero();
            cout << "Pago del cliente: $"; int pago = ingresarEntero();

            if (pago < costo) {
                cout << "Pago insuficiente.\n";
            } else {
                int cambio = pago - costo;
                cout << "Cambio a devolver: $" << cambio << endl;

                // Array estático (o dinámico si quieres ser purista, aquí uso dinámico)
                int tamMonedas = 4;
                int* monedas = new int[tamMonedas];
                *(monedas + 0) = 20; *(monedas + 1) = 10; *(monedas + 2) = 5; *(monedas + 3) = 1;

                cout << "Desglose optimo:\n";
                for (int i = 0; i < tamMonedas; i++) {

                    if (cambio >= *(monedas + i)) {
                        int cantidad = cambio / *(monedas + i);
                        cambio = cambio % *(monedas + i);
                        cout << " -> " << cantidad << " billete(s) de $" << *(monedas + i) << endl;
                    }
                }
                delete[] monedas;
            }
        }
        
void Voraz::asignacionRapida(ListaCircularDoble &boleteria) {
    system("cls");
    cout << "--- RESERVA VORAZ (PRIMER DISPONIBLE) ---\n\n";


    Persistencia::cargarReservas(boleteria);

    if (boleteria.estaVacia()) {
        cout << "Error: La boleteria no ha sido inicializada.\n";
        system("pause");
        return;
    }


    
    int asientosDisponibles = 0;
    Nodo* aux = boleteria.getCabeza();
    
    do {
        if (!aux->dato.estaOcupado) {
            asientosDisponibles++;
        }
        aux = aux->siguiente;
    } while (aux != boleteria.getCabeza());

    if (asientosDisponibles == 0) {
        cout << "LO SENTIMOS: El evento esta completamente LLENO (Sold Out).\n";
        system("pause");
        return;
    }

    cout << "INFO: Hay " << asientosDisponibles << " asientos libres en este momento.\n\n";


    cout << "Ingrese cantidad de asientos a reservar (Max: " << asientosDisponibles << "): ";
    int cantidad = ingresarEntero(); 

    if (cantidad <= 0) {
        cout << "Cantidad invalida.\n";
        system("pause");
        return;
    }

    if (cantidad > asientosDisponibles) {
        cout << "\nERROR: No puede reservar " << cantidad 
             << " asientos porque solo quedan " << asientosDisponibles << " disponibles.\n";
        system("pause");
        return;
    }


    cout << "Ingrese la cedula del cliente: ";
    string cedula = ingresarCedula();

    string nombre = ReservationManager::obtenerNombrePorCedula(boleteria, cedula);

    if (!nombre.empty()) {
        cout << "Cliente identificado: " << nombre << endl;
    } else {
        nombre = ingresarNombre(); 
    }

    string fechaHoy = Tiempo::obtenerFecha();
    string horaActual = Tiempo::obtenerHora();

    int asignados = 0;
    Nodo* actual = boleteria.getCabeza();
    int vueltas = 0; 

    cout << "\nProcesando asignacion automatica (Voraz)...\n";

    do {
        if (!actual->dato.estaOcupado) {
            
            bool exitoOCC = ConcurrenciaOCC::actualizarAsiento(
                actual->dato.numeroAsiento,
                "OCUPADO",
                nombre,
                cedula,
                actual->dato.getCategoria(),
                fechaHoy,
                horaActual
            );

            if (exitoOCC) {
                actual->dato.reservar(nombre, cedula);
                
                cout << " [EXITO] Asiento " << actual->dato.numeroAsiento 
                     << " (" << actual->dato.getCategoria() << ") asignado.\n";
                
                asignados++;
            } else {
                cout << " [BLOQUEADO] El asiento " << actual->dato.numeroAsiento 
                     << " fue ocupado por otro proceso.\n";
            }
        }
        
        actual = actual->siguiente;

        if (actual == boleteria.getCabeza()) {
            vueltas++;
            if (vueltas == 1) Persistencia::cargarReservas(boleteria);
            if (vueltas >= 2) break; 
        }

    } while (asignados < cantidad);

    cout << "\n--------------------------------------------------\n";
    if (asignados == 0) {
        cout << "ERROR: No se pudo reservar ningun asiento (Evento lleno o alta concurrencia).\n";
    } else if (asignados < cantidad) {
        cout << "PARCIAL: Solo se reservaron " << asignados 
             << " de " << cantidad << " solicitados.\n";
    } else {
        cout << "EXITO: Se han reservado " << cantidad << " asientos correctamente.\n";
    }

    Persistencia::cargarReservas(boleteria); 

    system("pause");
}