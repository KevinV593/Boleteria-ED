#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <conio.h>

using namespace std;

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
    string p = "";
    int esp = 0;
    char c;

    while (true) {
        c = _getch(); 

        if (c == 0 || c == -32) {
            _getch(); 
            continue; 
        }

        /// ENTER ///
        if (c == 13 && !p.empty()) { 
            if (p.back() == ' ') { p.pop_back(); cout << "\b \b"; } // Limpieza final
            break; 
        }
        // BACKSPACE //
        else if (c == 8 && !p.empty()) { 
            if (p.back() == ' ') esp--; 
            p.pop_back(); 
            cout << "\b \b";
        }
        /// LETRAS (Solo alfabéticas) ///
        else if (isalpha(c)) { 
            p += c; 
            cout << c;
        }
        // ESPACIO (máx 3 espacios) //
        else if (c == ' ' && !p.empty() && p.back() != ' ' && esp < 3) {
            p += c; 
            cout << c;
            esp++;
        }
    }
    cout << endl;
    return p;
}

bool validarCedula(string cedula) {
    if (cedula.length() != 10) return false;
    int sum = 0;
    for (int i = 0; i < 9; i++) {
        int digit = cedula[i] - '0';
        if (i % 2 == 0) {
            digit *= 2;
            if (digit > 9) digit -= 9;
        }
        sum += digit;
    }
    int verifier = cedula[9] - '0';
    return (sum % 10 == 0) ? (verifier == 0) : ((10 - (sum % 10)) == verifier);
}

string ingresarCedula() {
    string cedula;
    char c;
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
        if (validarCedula(cedula)) break;
        else cout << "Cedula invalida. Intente de nuevo." << endl;
    }
    return cedula;
}

char* ingresar(char* msj) {
    static char dato[25];
    char c;
    int i = 0;
    printf("%s: ", msj);
    while ((c = _getch()) != 13) {
        if (isdigit(c) || c == 8) {
            dato[i++] = c;
            printf("%c", c);
        }
    }
    dato[i] = '\0';
    return dato;
}