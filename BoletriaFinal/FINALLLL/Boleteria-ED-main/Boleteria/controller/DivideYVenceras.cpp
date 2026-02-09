#include "DivideYVenceras.hpp"

void DivideYVenceras::mergePunteros(int* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        *(L + i) = *(arr + l + i); // Equivalente a L[i] = arr[l + i]
    for (int j = 0; j < n2; j++)
        *(R + j) = *(arr + m + 1 + j);

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

    while (i < n1) {
        *(arr + k) = *(L + i);
        i++;
        k++;
    }

    while (j < n2) {
        *(arr + k) = *(R + j);
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void DivideYVenceras::mergeSortPunteros(int* arr, int l, int r) {
    if (l < r) {

        int m = l + (r - l) / 2;

        mergeSortPunteros(arr, l, m);
        mergeSortPunteros(arr, m + 1, r);

        mergePunteros(arr, l, m, r);
    }
}

int DivideYVenceras::binarySearchPunteros(int* arr, int l, int r, int x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;

        if (*(arr + mid) == x)
            return mid;

        if (*(arr + mid) > x)
            return binarySearchPunteros(arr, l, mid - 1, x);

        return binarySearchPunteros(arr, mid + 1, r, x);
    }
    return -1; // No encontrado
}