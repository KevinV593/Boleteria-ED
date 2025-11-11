#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <conio.h>

using namespace std;

int ingresarEntero();
string ingresarLetra();
bool validar(string numero);
bool validarEntero(string numero);  
bool validarLetra(string palabra);
bool validarCedula(string cedula);
string ingresarCedula();
char* ingresar(char* msj);

string ingresarStringValidado() {
    string palabra;
    char c;

    while (true) {
        palabra.clear();
        while (true) {
            c = _getch();  // Captura la entrada sin mostrarla en pantalla

            if (c == 13) {  // Enter key
                break;
            }
            else if (c == 8) {  // Backspace key
                if (!palabra.empty()) {
                    palabra.pop_back();
                    cout << "\b \b";  // Borra el car�cter en la consola
                }
            }
            else if (isalpha(c)) {  // Solo acepta letras
                palabra.push_back(c);
                cout << c;  // Muestra el car�cter en la consola
            }
        }

        // Permitir que la palabra est� vac�a para aceptar el Enter sin caracteres
        break;
    }

    cout << endl;  // Nueva l�nea despu�s de la entrada
    return palabra;
}

int ingresarEntero() {
    string numero;
    bool valido = false;
    while (!valido) {
        try {
            char c;
            while (true) {
                c = _getch();
                if (c == '\r') {
                    cout << endl;
                    break;
                }
                else if (c == '\b') {
                    if (!numero.empty()) {
                        numero.pop_back();
                        cout << "\b \b";
                    }
                }
                else if (isdigit(c)) {
                    numero += c;
                    cout << c;
                }
            }
            valido = validarEntero(numero);
            if (!valido) {
                throw numero;
            }
        }
        catch (string e) {
            cout << "\nEl numero " << e << " no es valido" << endl;
            numero.clear();
            valido = false;
        }
    }
    return atoi(numero.c_str());
}

string ingresarLetra() {
    string palabra;
    char c;

    while (true) {
        palabra.clear();
        while (true) {
            c = _getch();  // Captura la entrada sin mostrarla en pantalla

            if (c == 13) {  // Enter key
                break;
            }
            else if (c == 8) {  // Backspace key
                if (!palabra.empty()) {
                    palabra.pop_back();
                    cout << "\b \b";  // Borra el car�cter en la consola
                }
            }
            else if (isalpha(c)) {  // Solo acepta letras
                palabra.push_back(c);
                cout << c;  // Muestra el car�cter en la consola
            }
        }

        if (!palabra.empty()) {
            break;
        }
        else {
            cout << "\nNo se permiten cadenas vacias. Intente de nuevo: ";
        }
    }

    cout << endl;  // Nueva l�nea despu�s de la entrada
    return palabra;
}

bool validar(string numero) {
    int inicio = 0;
    if (numero.length() == 0) {
        return false;
    }
    if (numero[0] == '+' || numero[0] == '-') {
        inicio = 1;
        if (numero.length() == 1) {
            return false;
        }
    }
    for (int i = inicio; i < numero.length(); i++) {
        if (!isdigit(numero[i]) && numero[i] != '.') {
            return false;
        }
    }
    return true;
}

bool validarEntero(string numero) {
    int inicio = 0;
    if (numero.length() == 0) {
        return false;
    }
    if (numero[0] == '+' || numero[0] == '-') {
        inicio = 1;
        if (numero.length() == 1) {
            return false;
        }
    }
    for (int i = inicio; i < numero.length(); i++) {
        if (!isdigit(numero[i])) {
            return false;
        }
    }
    return true;
}

bool validarLetra(string palabra) {
    char c;
    for (int i = 0; i < palabra.size(); i++) {
        c = palabra[i];
        if (isalpha(c) == 0) {
            if (isspace(c) == 0) {
                return 0;
            }
        }
    }
    return 1;
}

bool validarCedula(string cedula) {
    if (cedula.length() != 10) {
        return false;
    }
    int sum = 0;
    for (int i = 0; i < 9; i++) {
        int digit = cedula[i] - '0';
        if (i % 2 == 0) {
            digit *= 2;
            if (digit > 9) {
                digit -= 9;
            }
        }
        sum += digit;
    }
    int verifier = cedula[9] - '0';
    return (sum % 10 == 0) ? (verifier == 0) : ((10 - (sum % 10)) == verifier);
}

string ingresarCedula() {
    string cedula;
    char c;
    cout << "Ingrese la cedula: ";
    while (true) {
        cedula = "";
        while (true) {
            c = _getch();
            if (c == 13) break;  // Enter
            if (c == 8) {        // Backspace
                if (!cedula.empty()) {
                    cedula.pop_back();
                    cout << "\b \b";
                }
            }
            else if (isdigit(c)) {
                cedula.push_back(c);
                cout << c;
            }
        }
        cout << endl;
        if (validarCedula(cedula)) {
            break;
        }
        else {
            cout << "Cedula invalida. Intente de nuevo." << endl;
        }
    }
    return cedula;
}

char* ingresar(char* msj) {
    static char dato[25];
    char c;
    int i = 0;
    printf("%s: ", msj);
    while ((c = _getch()) != 13) {
        if (isdigit(c) || c == 8) {  // 8 is the ASCII code for Backspace
            dato[i++] = c;
            printf("%c", c);
        }
    }
    dato[i] = '\0';
    return dato;
}
