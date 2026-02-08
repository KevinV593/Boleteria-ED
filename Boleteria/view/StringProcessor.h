#pragma once
#include <string>
#include <cctype>

using namespace std;

// Estructura interna para procesar caracteres
struct NodoLetra
{
    char letra;
    NodoLetra *siguiente;
};

class StringProcessor
{
public:
    // Muestra el nombre completo con caracteres ordenados
    static void mostrarNombreOrdenado(string nombreOriginal);

    // Procesa y muestra el nombre agrupado por chunks de tamaño tamanoGrupo
    static void procesarPorGrupos(string nombre, int tamanoGrupo);

private:
    static void agregarLetra(NodoLetra *&cabeza, NodoLetra *&fin, char c);
    static void ordenarSubLista(NodoLetra *cabeza);
    static void liberarSubLista(NodoLetra *&cabeza);
};
