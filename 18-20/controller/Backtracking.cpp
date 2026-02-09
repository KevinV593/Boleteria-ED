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
    int totalOcupados = 0;
    if (!boleteria.estaVacia()) {
        Nodo* aux = boleteria.getCabeza();
        do {
            if (aux->dato.estaOcupado) totalOcupados++;
            aux = aux->siguiente;
        } while (aux != boleteria.getCabeza());
    }

    if (totalOcupados == 0) { 
        cout << "No hay clientes ocupados para permutar.\n"; 
        return; 
    }

    string** nombresUnicos = new string*[totalOcupados];
    int contadorUnicos = 0;

    Nodo* actual = boleteria.getCabeza();
    do {
        if (actual->dato.estaOcupado) {
            string nombreActual = actual->dato.nombreCliente;
            bool existe = false;

            for (int i = 0; i < contadorUnicos; i++) {
                if (*(nombresUnicos[i]) == nombreActual) {
                    existe = true;
                    break;
                }
            }

            if (!existe) {
                nombresUnicos[contadorUnicos] = new string(nombreActual);
                contadorUnicos++;
            }
        }
        actual = actual->siguiente;
    } while (actual != boleteria.getCabeza());

    long long total = 1;
    for (int i = 2; i <= contadorUnicos; i++) total *= i;

    cout << "Se encontraron " << contadorUnicos << " nombres unicos.\n";
    cout << "Se van a generar " << total << " permutaciones posibles.\n";

    if (total > 40000) { // Límite de seguridad
        cout << "ADVERTENCIA: Son demasiadas combinaciones. Desea continuar? (s/n): ";
        char confirm;
        cin >> confirm;
        if (confirm != 's' && confirm != 'S') {
            // Limpieza antes de salir
            for (int i = 0; i < contadorUnicos; i++) delete nombresUnicos[i];
            delete[] nombresUnicos;
            return;
        }
    }

    cout << "\n--- GENERANDO PERMUTACIONES ---\n";
    
    permutaciones(nombresUnicos, contadorUnicos, 0);

    for (int i = 0; i < contadorUnicos; i++) {
        delete nombresUnicos[i]; 
    }
    delete[] nombresUnicos; 

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
