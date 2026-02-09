#include "Dinamica.hpp"
#include <iostream>
#include <cctype> 
#include <cstring>  
#include <algorithm> 
using namespace std;

//    LCS - Longest Common Subsequence

string aMinusculas(string cadena) {
    string nueva = cadena;
    for (int i = 0; i < nueva.length(); i++) {
        nueva[i] = tolower(nueva[i]);
    }
    return nueva;
}

void Dinamica::lcs(ListaCircularDoble &boleteria)
{
    system("cls");
    cout << "--- SIMILITUD DE NOMBRES DE CLIENTES (LCS) ---\n";

    Persistencia::cargarReservas(boleteria);

    if (boleteria.estaVacia()) {
        cout << "No hay clientes registrados.\n";
        system("pause");
        return;
    }

    cout << "Clientes disponibles para comparar:\n";
    
    // Contamos primero cuantos ocupados hay para dimensionar el array temporal
    int contadorOcupados = 0;
    Nodo* aux = boleteria.getCabeza();
    do {
        if (aux->dato.estaOcupado) contadorOcupados++;
        aux = aux->siguiente;
    } while (aux != boleteria.getCabeza());

    if (contadorOcupados == 0) {
        cout << "No hay asientos ocupados.\n";
        system("pause");
        return;
    }

    string* nombresVistos = new string[contadorOcupados];
    int contadorVistos = 0;

    Nodo* actual = boleteria.getCabeza();
    int idx = 1;

    do {
        if (actual->dato.estaOcupado) {
            string nombreActual = actual->dato.nombreCliente;
            bool repetido = false;

            for (int k = 0; k < contadorVistos; k++) {
                if (nombresVistos[k] == nombreActual) {
                    repetido = true;
                    break;
                }
            }

            if (!repetido) {
                cout << " [" << idx << "] " << nombreActual << "\n";
                nombresVistos[contadorVistos] = nombreActual;
                contadorVistos++;
                idx++;
            }
        }
        actual = actual->siguiente;
    } while (actual != boleteria.getCabeza());
    
    delete[] nombresVistos; 

    char nombre1[100], nombre2[100];
    cout << "\nIngrese primer nombre a comparar: ";
    cin.ignore(); 
    cin.getline(nombre1, 100);
    cout << "Ingrese segundo nombre a comparar: ";
    cin.getline(nombre2, 100);

    actual = boleteria.getCabeza();
    bool encontrado1 = false, encontrado2 = false;
    Nodo* inicio = actual;

    string n1Str = aMinusculas(string(nombre1));
    string n2Str = aMinusculas(string(nombre2));

    do {
        if (actual->dato.estaOcupado) {
            string cliente = aMinusculas(actual->dato.nombreCliente);

            if (cliente.find(n1Str) != string::npos) encontrado1 = true;
            if (cliente.find(n2Str) != string::npos) encontrado2 = true;
        }
        actual = actual->siguiente;
    } while (actual != inicio);

    if (!encontrado1 || !encontrado2) {
        cout << "ADVERTENCIA: Uno o ambos nombres no coinciden exactamente con la lista.\n";
        cout << "Se procedera con el calculo de todas formas...\n";
    }

    int m = 0, n = 0;
    while (*(nombre1 + m) != '\0') m++;
    while (*(nombre2 + n) != '\0') n++;

    int filas = m + 1, columnas = n + 1;
    
    int* dp = new int[filas * columnas];

    for (int i = 0; i < filas * columnas; i++) *(dp + i) = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {

            char c1 = tolower(*(nombre1 + i - 1));
            char c2 = tolower(*(nombre2 + j - 1));

            if (c1 == c2) {
                *(dp + i * columnas + j) = *(dp + (i - 1) * columnas + (j - 1)) + 1;
            } else {
                int arriba = *(dp + (i - 1) * columnas + j);
                int izquierda = *(dp + i * columnas + (j - 1));
                *(dp + i * columnas + j) = (arriba > izquierda) ? arriba : izquierda;
            }
        }
    }

    int len = *(dp + m * columnas + n);
    char* subsecuencia = new char[len + 1];
    subsecuencia[len] = '\0';

    int i = m, j = n;
    while (i > 0 && j > 0) {

        char c1 = tolower(*(nombre1 + i - 1));
        char c2 = tolower(*(nombre2 + j - 1));

        if (c1 == c2) {
            subsecuencia[len - 1] = *(nombre1 + i - 1);
            i--; j--; len--;
        } else if (*(dp + (i - 1) * columnas + j) >= *(dp + i * columnas + (j - 1))) {
            i--;
        } else {
            j--;
        }
    }

    cout << "\n----------------RESULTADOS----------------\n";
    cout << "Nombre 1: " << nombre1 << "\n";
    cout << "Nombre 2: " << nombre2 << "\n";
    cout << "Longitud LCS: " << *(dp + m * columnas + n) << endl;
    cout << "Similitud encontrada: " << subsecuencia << endl;

    float porcentaje = (float(*(dp + m * columnas + n)) * 2.0f / (m + n)) * 100.0f;
    cout << "Porcentaje de coincidencia: " << porcentaje << "%" << endl;

    delete[] subsecuencia;
    delete[] dp;
    
}