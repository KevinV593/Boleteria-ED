#pragma once
#include <string>
#include <ctime>   // Librería estándar de tiempo
#include <cstdio>  // Para sprintf o strftime

using namespace std;

class Tiempo {
public:
    // Fecha actual como string "DD/MM/AAAA"
    static string obtenerFecha() {
        time_t ahora = time(0);
        tm* tiempo = localtime(&ahora);
        char buffer[20];
        // Formato: %d (día), %m (mes), %Y (año 4 dígitos)
        strftime(buffer, 20, "%d/%m/%Y", tiempo);
        return string(buffer);
    }

    /// Hora actual como string "HH:MM:SS"
    static string obtenerHora() {
        time_t ahora = time(0);
        tm* tiempo = localtime(&ahora);
        char buffer[20];
        // Formato: %H (hora 24h), %M (minuto), %S (segundo)
        strftime(buffer, 20, "%H:%M:%S", tiempo);
        return string(buffer);
    }
};