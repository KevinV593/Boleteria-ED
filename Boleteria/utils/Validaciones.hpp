#pragma once
#include <string>

using namespace std;

class Validaciones {
public:
    static int leerInt();
};

//funciones libres (sacadas del archivo de validaciones anterior)
int ingresarEntero();
string ingresarLetra();
bool validarCedula(const string& cedula);
string ingresarCedula();
char* ingresar(char* msj);
