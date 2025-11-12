#include "Validaciones.hpp"
#include <iostream>
#include <conio.h>
#include <cstdio>  
#include <cstdlib> 

using namespace std;

int Validaciones::leerInt()
{
    const int INT_TAM = 12;
    char ingreso[INT_TAM] = "";
    char c;

    int pos = 0;
    do
    {
        c = getch(); // Función de conio.h

        if (pos != INT_TAM - 1)
        {
            // Permite números y el signo '-' solo al inicio
            if (('0' <= c && c <= '9') || (c == '-' && pos == 0))
            {
                putchar(c); // Muestra el carácter
                ingreso[pos] = c;
                ingreso[++pos] = '\0';
            }
        }

        // Manejo del 'backspace' (borrar)
        if ((c == '\b' || c == 127) && pos != 0) {
            putchar('\b');
            putchar(' ');
            putchar('\b');
            ingreso[--pos] = '\0';
        }
    } while ((c != '\n' && c != '\r') || pos == 0); // Sale con Enter

    putchar('\n');
    return atoi(ingreso);
};