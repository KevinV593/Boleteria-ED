#include "Dinamica.hpp"
#include <iostream>

using namespace std;

/*
    Mochila 0/1
    precios  -> costos de entradas (100, 60, 40) Sacasdos de Voraz
    valores  -> calidad (3, 2, 1)
    n        -> numero de entradas consideradas
    capacidad-> presupuesto del cliente
*/

void Dinamica::mochila01(int* precios, int* valores, int n, int capacidad)
{
    int filas = n + 1;
    int columnas = capacidad + 1;

    int* dp = new int[filas * columnas];

    // Inicializar tabla DP
    for (int i = 0; i < filas; i++)
        for (int w = 0; w < columnas; w++)
            *(dp + i * columnas + w) = 0;

    // Llenar tabla DP
    for (int i = 1; i <= n; i++) {
        int precio = *(precios + i - 1);
        int valor  = *(valores + i - 1);

        for (int w = 0; w <= capacidad; w++) {
            int sinTomar = *(dp + (i - 1) * columnas + w);
            int tomar = (precio <= w) ? valor + *(dp + (i - 1) * columnas + (w - precio)) : 0;
            *(dp + i * columnas + w) = (tomar > sinTomar) ? tomar : sinTomar;
        }
    }

    // Nombres de boletos
    char* nombres[] = { (char*)"PALCO", (char*)"TRIBUNA", (char*)"GENERAL" };

    cout << "--- OPTIMIZACION DE COMPRA DE ENTRADAS ---\n";
    cout << "Presupuesto: $" << capacidad << endl;
    cout << "Calidad maxima alcanzable: " << *(dp + n * columnas + capacidad) << endl;

    int w = capacidad;
    cout << "Entradas seleccionadas:\n";
    for (int i = n; i > 0; i--) {
        if (*(dp + i * columnas + w) != *(dp + (i - 1) * columnas + w)) {
            cout << " - " << *(nombres + i - 1)
                 << " (precio: " << *(precios + i - 1) 
                 << ", valor: " << *(valores + i - 1) << ")\n";
            w -= *(precios + i - 1);
        }
    }

    delete[] dp;
}


/*
    LCS - Longest Common Subsequence
*/
void Dinamica::lcs(ListaCircularDoble &boleteria)
{
    system("cls");
    cout << "--- SIMILITUD DE NOMBRES DE CLIENTES (LCS) ---\n";

    Persistencia::cargarReservas(boleteria);

    Nodo* actual = boleteria.getCabeza();
    if (!actual) {
        cout << "No hay clientes registrados.\n";
        return;
    }

    // Mostrar clientes disponibles
    cout << "Clientes disponibles:\n";
    Nodo* inicio = actual;
    int idx = 1;
    do {
        if (actual->dato.estaOcupado) {
            cout << " [" << idx << "] " << actual->dato.nombreCliente << "\n";
        }
        actual = actual->siguiente;
        idx++;
    } while (actual != inicio);

    // Pedir nombres a comparar
    char nombre1[100], nombre2[100];
    cout << "\nIngrese primer nombre a comparar: ";
    cin.ignore();
    cin.getline(nombre1, 100);
    cout << "Ingrese segundo nombre a comparar: ";
    cin.getline(nombre2, 100);

    // Validar existencia usando búsqueda por subcadena
    actual = boleteria.getCabeza();
    bool encontrado1 = false, encontrado2 = false;
    inicio = actual;

    do {
        if (actual->dato.estaOcupado) {
            std::string cliente = actual->dato.nombreCliente;

            if (cliente.find(nombre1) != std::string::npos) encontrado1 = true;
            if (cliente.find(nombre2) != std::string::npos) encontrado2 = true;
        }
        actual = actual->siguiente;
    } while (actual != inicio && (!encontrado1 || !encontrado2));

    if (!encontrado1 || !encontrado2) {
        cout << "Uno o ambos nombres no se encontraron en las reservas.\n";
        return;
    }

    // Calcular LCS clásico con punteros
    int m = 0, n = 0;
    while (*(nombre1 + m) != '\0') m++;
    while (*(nombre2 + n) != '\0') n++;

    int filas = m + 1, columnas = n + 1;
    int* dp = new int[filas * columnas];

    // Inicializar tabla
    for (int i = 0; i < filas; i++)
        for (int j = 0; j < columnas; j++)
            *(dp + i * columnas + j) = 0;

    // Llenar DP
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (*(nombre1 + i - 1) == *(nombre2 + j - 1)) {
                *(dp + i * columnas + j) = *(dp + (i - 1) * columnas + (j - 1)) + 1;
            } else {
                int arriba = *(dp + (i - 1) * columnas + j);
                int izquierda = *(dp + i * columnas + (j - 1));
                *(dp + i * columnas + j) = (arriba > izquierda) ? arriba : izquierda;
            }
        }
    }

    // Reconstruir subsecuencia
    int len = *(dp + m * columnas + n);
    char* subsecuencia = new char[len + 1];
    subsecuencia[len] = '\0';

    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (*(nombre1 + i - 1) == *(nombre2 + j - 1)) {
            subsecuencia[len - 1] = *(nombre1 + i - 1);
            i--; j--; len--;
        } else if (*(dp + (i - 1) * columnas + j) >= *(dp + i * columnas + (j - 1))) {
            i--;
        } else {
            j--;
        }
    }

    cout << "\nLongitud de la subsecuencia comun mas larga: " 
         << *(dp + m * columnas + n) << endl;
    cout << "Subsecuencia comun: " << subsecuencia << endl;

    delete[] subsecuencia;
    delete[] dp;
}
