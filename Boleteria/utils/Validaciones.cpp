#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <conio.h>

using namespace std;

class Validaciones {
public:
    // Función estática para leer enteros con validación
    static int leerInt() {
        const int INT_TAM = 12;
        char ingreso[INT_TAM] = "";
        char c;
        int pos = 0;

        do {
            c = _getch();

            // Permite números y el signo '-' solo al inicio
            if (pos != INT_TAM - 1) {
                if (('0' <= c && c <= '9') || (c == '-' && pos == 0)) {
                    putchar(c);
                    ingreso[pos] = c;
                    ingreso[++pos] = '\0';
                }
            }

            // Manejo de backspace
            if ((c == '\b' || c == 127) && pos != 0) {
                putchar('\b');
                putchar(' ');
                putchar('\b');
                ingreso[--pos] = '\0';
            }
        } while ((c != '\n' && c != '\r') || pos == 0);

        putchar('\n');
        return atoi(ingreso);
    }
};

// Funciones libres para validaciones

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
    string palabra;
    char c;
    while (true) {
        palabra.clear();
        while (true) {
            c = _getch();

            if (c == 13) break;  // Enter
            else if (c == 8) {   // Backspace
                if (!palabra.empty()) {
                    palabra.pop_back();
                    cout << "\b \b";
                }
            }
            else if (isalpha(c) || isspace(c)) {
                palabra.push_back(c);
                cout << c;
            }
        }

        if (!palabra.empty()) break;
        else cout << "\nNo se permiten cadenas vacias. Intente de nuevo: ";
    }
    cout << endl;
    return palabra;
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