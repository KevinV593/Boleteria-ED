#include "StringProcessor.h"
#include <iostream>

using namespace std;

void StringProcessor::agregarLetra(NodoLetra *&cabeza, NodoLetra *&fin, char c)
{
    NodoLetra *nuevo = new NodoLetra{c, nullptr};
    if (!cabeza)
        cabeza = fin = nuevo;
    else
    {
        fin->siguiente = nuevo;
        fin = nuevo;
    }
}

void StringProcessor::ordenarSubLista(NodoLetra *cabeza)
{
    if (!cabeza)
        return;
    bool ordenado;
    do
    {
        ordenado = true;
        for (NodoLetra *a = cabeza; a->siguiente; a = a->siguiente)
        {
            if (tolower(a->letra) > tolower(a->siguiente->letra))
            {
                swap(a->letra, a->siguiente->letra);
                ordenado = false;
            }
        }
    } while (!ordenado);
}

void StringProcessor::liberarSubLista(NodoLetra *&cabeza)
{
    while (cabeza)
    {
        NodoLetra *aux = cabeza;
        cabeza = cabeza->siguiente;
        delete aux;
    }
}

void StringProcessor::mostrarNombreOrdenado(string nombreOriginal)
{
    if (nombreOriginal.empty())
        return;
    NodoLetra *cabeza = nullptr;
    NodoLetra *fin = nullptr;

    for (const char *p = nombreOriginal.c_str(); *p; p++)
        agregarLetra(cabeza, fin, *p);

    ordenarSubLista(cabeza);

    for (NodoLetra *n = cabeza; n; n = n->siguiente)
        cout << n->letra;
    cout << endl;

    liberarSubLista(cabeza);
}

void StringProcessor::procesarPorGrupos(string nombre, int tamanoGrupo)
{
    if (tamanoGrupo <= 0)
        return;

    const char *lector = nombre.c_str();
    while (*lector)
    {
        NodoLetra *cabeza = nullptr;
        NodoLetra *fin = nullptr;
        int c = 0;

        while (c < tamanoGrupo && *lector)
        {
            agregarLetra(cabeza, fin, *lector++);
            c++;
        }

        ordenarSubLista(cabeza);
        for (NodoLetra *n = cabeza; n; n = n->siguiente)
            cout << n->letra;

        liberarSubLista(cabeza);
    }
    cout << endl;
}
