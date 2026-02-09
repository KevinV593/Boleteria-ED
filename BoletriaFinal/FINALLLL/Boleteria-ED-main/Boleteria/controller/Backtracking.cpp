#include "Backtracking.hpp"

using namespace std;

void Backtracking::combinacionesAsientos(ListaCircularDoble &boleteria) {
    int count = contarAsientosOcupados(boleteria);
    if (count == 0) { cout << "No hay asientos ocupados.\n"; return; }

    int* ocupados = new int[count];
    llenarAsientosOcupados(boleteria, ocupados);

    int r;
    cout << "Ingrese cuantos asientos combinar: ";
    cin >> r;

    int* data = new int[r];
    combinaciones(ocupados, count, r, data);

    delete[] data;
    delete[] ocupados;
}

void Backtracking::permutacionesNombres(ListaCircularDoble &boleteria) {
    int count = contarClientesOcupados(boleteria);
    if (count == 0) { cout << "No hay clientes ocupados.\n"; return; }

    // Contar posibles permutaciones
    long long total = 1;
    for (int i = 2; i <= count; i++) total *= i;

    cout << "Se van a generar " << total << " permutaciones.\n";
    if (total > 30) { // límite práctico para no congelar
        char confirm;
        cout << "Esto puede tardar mucho. Desea continuar? (s/n): ";
        cin >> confirm;
        if (confirm != 's' && confirm != 'S') return;
    }

    string** nombres = new string*[count];
    llenarNombresOcupados(boleteria, nombres);

    permutaciones(nombres, count, 0);

    delete[] nombres;
}

int Backtracking::contarAsientosOcupados(ListaCircularDoble &boleteria) {
    int c = 0;
    Nodo* actual = boleteria.getCabeza();
    if (!actual) return 0;
    do {
        if (actual->dato.estaOcupado) c++;
        actual = actual->siguiente;
    } while (actual != boleteria.getCabeza());
    return c;
}

void Backtracking::llenarAsientosOcupados(ListaCircularDoble &boleteria, int* ocupados) {
    int index = 0;
    Nodo* actual = boleteria.getCabeza();
    if (!actual) return;
    do {
        if (actual->dato.estaOcupado) {
            *(ocupados + index) = actual->dato.numeroAsiento;
            index++;
        }
        actual = actual->siguiente;
    } while (actual != boleteria.getCabeza());
}

int Backtracking::contarClientesOcupados(ListaCircularDoble &boleteria) {
    int c = 0;
    Nodo* actual = boleteria.getCabeza();
    if (!actual) return 0;
    do {
        if (actual->dato.estaOcupado) c++;
        actual = actual->siguiente;
    } while (actual != boleteria.getCabeza());
    return c;
}

void Backtracking::llenarNombresOcupados(ListaCircularDoble &boleteria, string** nombres) {
    int index = 0;
    Nodo* actual = boleteria.getCabeza();
    if (!actual) return;
    do {
        if (actual->dato.estaOcupado) {
            *(nombres + index) = &(actual->dato.nombreCliente);
            index++;
        }
        actual = actual->siguiente;
    } while (actual != boleteria.getCabeza());
}

void Backtracking::combinaciones(int* arr, int n, int r, int* data, int index, int i) {
    if (index == r) {
        cout << "Combinacion: ";
        for (int j = 0; j < r; j++) cout << *(data + j) << " ";
        cout << "\n";
        return;
    }
    if (i >= n) return;

    *(data + index) = *(arr + i);
    combinaciones(arr, n, r, data, index + 1, i + 1); 
    combinaciones(arr, n, r, data, index, i + 1);      
}

void Backtracking::permutaciones(string** arr, int n, int i) {
    if (i == n) {
        cout << "Permutacion: ";
        for (int j = 0; j < n; j++) cout << **(arr + j) << " ";
        cout << "\n";
        return;
    }

    for (int j = i; j < n; j++) {
        string* temp = *(arr + i);
        *(arr + i) = *(arr + j);
        *(arr + j) = temp;

        permutaciones(arr, n, i + 1);

        temp = *(arr + i);
        *(arr + i) = *(arr + j);
        *(arr + j) = temp;
    }
}
