#include "DivideYVenceras.hpp"

void DivideYVenceras::mergePunteros(int* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Memoria Dinámica pura para sub-arreglos
    int* L = new int[n1];
    int* R = new int[n2];

    // Copiar datos a arrays temporales L[] y R[] usando punteros
    for (int i = 0; i < n1; i++)
        *(L + i) = *(arr + l + i); // Equivalente a L[i] = arr[l + i]
    for (int j = 0; j < n2; j++)
        *(R + j) = *(arr + m + 1 + j);

    // Fusionar los arrays temporales de vuelta en arr[l..r]
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (*(L + i) <= *(R + j)) {
            *(arr + k) = *(L + i);
            i++;
        } else {
            *(arr + k) = *(R + j);
            j++;
        }
        k++;
    }

    // Copiar elementos restantes de L[], si hay
    while (i < n1) {
        *(arr + k) = *(L + i);
        i++;
        k++;
    }

    // Copiar elementos restantes de R[], si hay
    while (j < n2) {
        *(arr + k) = *(R + j);
        j++;
        k++;
    }

    // Liberar memoria dinámica auxiliar
    delete[] L;
    delete[] R;
}

void DivideYVenceras::mergeSortPunteros(int* arr, int l, int r) {
    if (l < r) {
        // Misma lógica: m = l + (r - l) / 2
        int m = l + (r - l) / 2;

        // Dividir: ordenar primera y segunda mitad
        mergeSortPunteros(arr, l, m);
        mergeSortPunteros(arr, m + 1, r);

        // Vencer: mezclar las mitades
        mergePunteros(arr, l, m, r);
    }
}

int DivideYVenceras::binarySearchPunteros(int* arr, int l, int r, int x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;

        // Si el elemento está en el medio: *(arr + mid)
        if (*(arr + mid) == x)
            return mid;

        // Si el elemento es menor que el medio, está en el sub-array izquierdo
        if (*(arr + mid) > x)
            return binarySearchPunteros(arr, l, mid - 1, x);

        // Si no, está en el sub-array derecho
        return binarySearchPunteros(arr, mid + 1, r, x);
    }
    return -1; // No encontrado
}