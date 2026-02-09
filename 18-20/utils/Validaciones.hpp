#pragma once
#include <string>

using namespace std;

class Validaciones {
public:
    static int leerInt();
};

int ingresarEntero();
string ingresarLetra();
bool validarCedula(const string& cedula);
string ingresarCedula();
bool validarNombre(const string &nombre);
string ingresarNombre();