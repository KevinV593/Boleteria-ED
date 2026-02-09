#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include "../model/Boleto.hpp"
#include "../model/ListaCircularDoble.hpp"
#include "../utils/hash.hpp" 

using namespace std;

class ConcurrenciaOCC {
private:
    /**
     * @brief Lee una línea específica del archivo y extrae los datos del asiento
     * @param archivo Ruta del archivo (default: "server/reservas.txt")
     * @param numeroAsiento El número del asiento a buscar (línea 1 = asiento 1, línea 2 = asiento 2, etc)
     * @param estadoActual Referencia a bool que contendrá el estado ocupado/disponible
     * @param linea Referencia a string que contendrá toda la línea leída
     * @return true si se leyó correctamente, false si hubo error
     */
    static bool leerLineaDelAsiento(
        const string& archivo,
        int numeroAsiento,
        bool& estadoActual,
        string& linea
    ) {
        ifstream in(archivo);
        if (!in.is_open()) {
            cout << " Error: No se puede abrir " << archivo << endl;
            return false;
        }

        string lineaTemp;
        int lineaActual = 0;

        // Buscar la línea correspondiente al numeroAsiento
        while (getline(in, lineaTemp)) {
            lineaActual++;
            if (lineaActual == numeroAsiento) {
                // Extraer el estado (último campo tras el último ';')
                size_t ultimoPunto = lineaTemp.rfind(';');
                if (ultimoPunto != string::npos) {
                    string estadoStr = lineaTemp.substr(ultimoPunto + 1);
                    estadoActual = (stoi(estadoStr) != 0);
                    linea = lineaTemp;
                    in.close();
                    return true;
                }
            }
        }

        in.close();
        cout << " Error: Asiento " << numeroAsiento << " no encontrado en el archivo" << endl;
        return false;
    }

    /**
     * @brief Escribe una línea modificada en una posición específica del archivo
     * @param archivo Ruta del archivo
     * @param numeroAsiento El número del asiento (línea a modificar)
     * @param lineaNueva La nueva línea completa a escribir
     * @return true si la escritura fue exitosa, false en caso contrario
     */
    static bool escribirLineaDelAsiento(
        const string& archivo,
        int numeroAsiento,
        const string& lineaNueva
    ) {
        ifstream in(archivo);
        if (!in.is_open()) {
            cout << "Error: No se puede leer " << archivo << " para actualizar" << endl;
            return false;
        }

        vector<string> lineas;
        string linea;
        int lineaActual = 0;

        // Leer todas las líneas
        while (getline(in, linea)) {
            lineaActual++;
            if (lineaActual == numeroAsiento) {
                lineas.push_back(lineaNueva);  // Reemplazar la línea
            } else {
                lineas.push_back(linea);
            }
        }
        in.close();

        // Escribir todo el archivo actualizado
        ofstream out(archivo, ios::trunc);
        if (!out.is_open()) {
            cout << "Error: No se puede escribir en " << archivo << endl;
            return false;
        }

        for (int i = 0; i < lineas.size(); i++) {
            out << lineas[i];
            if (i < lineas.size() - 1) {
                out << "\n";
            }
        }

        out.close();

        string archivoHash = archivo.substr(0, archivo.find_last_of('.')) + ".hash";
        guardarHash(archivo, archivoHash);
        
        return true;
    }

public:
    /**
     * @brief Implementa OCC: Lectura → Comparación → Escritura segura para actualizar un asiento
     * 
     * ALGORITMO:
     * 1. RE-LEE la línea del asiento en el archivo (verificación de estado actual)
     * 2. COMPARA si el asiento sigue "Disponible" (estaOcupado == 0)
     * 3. Si está disponible → ESCRIBE los cambios inmediatamente
     * 4. Si NO está disponible → CANCELA, muestra error y devuelve false
     *
     * @param idAsiento El número del asiento a actualizar (1-based)
     * @param nuevoEstado El nuevo estado: "DISPONIBLE" o "OCUPADO" 
     * @param nombreCliente Nombre del cliente (solo si nuevoEstado == "OCUPADO")
     * @param cedulaCliente Cédula del cliente (solo si nuevoEstado == "OCUPADO")
     * @param categoriaAsiento Categoría del asiento (ej: "General", "Tribuna", "Palco")
     * @param archivo Ruta del archivo (default: "server/reservas.txt")
     * 
     * @return true si la actualización fue exitosa
     * @return false si hubo conflicto (el asiento ya fue ocupado por otro usuario)
     */
    static bool actualizarAsiento(
        int idAsiento,
        const string& nuevoEstado,
        const string& nombreCliente = "N/A",
        const string& cedulaCliente = "N/A",
        const string& categoriaAsiento = "General",
        const string& fechaReserva = "N/A",
        const string& horaReserva = "N/A",
        const string& archivo = "server/reservas.txt"
    ) {
        // ============ PASO 1: RE-LEER ============
        bool estadoActualEnArchivo;
        string lineaOriginal;

        if (!leerLineaDelAsiento(archivo, idAsiento, estadoActualEnArchivo, lineaOriginal)) {
            return false;
        }


        bool esDisponible = !estadoActualEnArchivo;

        if (!esDisponible) {
            cout << " ERROR: EL ASIENTO ACABA DE SER OCUPADO     \n";
            return false;
        }



        int estado = (nuevoEstado == "OCUPADO") ? 1 : 0;
        
        string lineaNueva = to_string(idAsiento) + ";"
                          + categoriaAsiento + ";"
                          + nombreCliente + ";"
                          + cedulaCliente + ";"
                          + fechaReserva + ";"
                          + horaReserva + ";"
                          + to_string(estado);

        if (!escribirLineaDelAsiento(archivo, idAsiento, lineaNueva)) {
            cout << " Error durante la escritura del archivo" << endl;
            return false;
        }


        cout << "RESERVA CONFIRMADA CON EXITO               \n";
        cout << "El asiento " << idAsiento << " ha sido reservado.";
        cout << " Cambios persistidos en el server.\n";
        return true;
    }

    /**
     * @brief Intenta reservar un asiento usando OCC
     * Versión simplificada que intenta la reserva y devuelve true/false
     * 
     * @param idAsiento Número del asiento (1-based)
     * @param nombreCliente Nombre del cliente
     * @param cedulaCliente Cédula del cliente
     * @param categoriaAsiento Categoría (ej: "General")
     * @param fechaReserva Fecha de la reserva
     * @param horaReserva Hora de la reserva
     * @param archivo Ruta del archivo (default: "server/reservas.txt")
     * @return true si la reserva fue exitosa, false si ya fue ocupado
     */

    static bool reservarAsientoOCC(
        int idAsiento,
        const string& nombreCliente,
        const string& cedulaCliente,
        const string& categoriaAsiento,
        const string& fechaReserva,
        const string& horaReserva,
        const string& archivo = "server/reservas.txt"
    ) {
        return actualizarAsiento(
            idAsiento,
            "OCUPADO",
            nombreCliente,
            cedulaCliente,
            categoriaAsiento,
            fechaReserva,
            horaReserva,
            archivo
        );
    }

    /**
     * @brief Sobreescribe los datos de una reserva existente sin verificar disponibilidad.
     * UTILIZAR SOLO CUANDO SE EDITA UN ASIENTO QUE YA POSEE EL USUARIO.
     */
    static bool sobreescribirDatosReserva(
        int idAsiento,
        const string& nombreCliente,
        const string& cedulaCliente,
        const string& categoriaAsiento,
        const string& fechaReserva,
        const string& horaReserva,
        const string& archivo = "server/reservas.txt"
    ) {
        // Construimos la línea directamente forzando el estado "1" (Ocupado)
        string lineaNueva = to_string(idAsiento) + ";" +
                            categoriaAsiento + ";" +
                            nombreCliente + ";" +
                            cedulaCliente + ";" +
                            fechaReserva + ";" +
                            horaReserva + ";1"; // Forzamos 1 porque es una edición

        // Llamamos directamente a la escritura
        if (!escribirLineaDelAsiento(archivo, idAsiento, lineaNueva)) {
            cout << " Error al actualizar los datos en el archivo." << endl;
            return false;
        }

        return true;
    }

    /**
     * @brief Cancela una reserva (libera un asiento) usando OCC
     * Nota: Para cancelaciones, no hace falta verificar disponibilidad,
     * pero mantiene la consistencia del patrón OCC
     * 
     * @param idAsiento Número del asiento
     * @param archivo Ruta del archivo
     * @return true si la cancelación fue exitosa
     */
    
    static bool cancelarAsientoOCC(
        int idAsiento,
        const string& archivo = "server/reservas.txt"
    ) {
        // Para cancelación, siempre escribimos sin verificación
        // (porque queremos liberar independientemente del estado actual)
        string lineaNueva = to_string(idAsiento) + ";General;N/A;N/A;N/A;N/A;0";
        
        if (!escribirLineaDelAsiento(archivo, idAsiento, lineaNueva)) {
            cout << "Error al cancelar la reserva" << endl;
            return false;
        }

        cout << "Asiento " << idAsiento << " liberado correctamente" << endl;
        return true;
    }
};