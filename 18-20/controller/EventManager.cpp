#include "EventManager.h"

void EventManager::inicializarEvento(ListaCircularDoble &boleteria, int totalAsientos)
{
    int limiteGeneral = totalAsientos * 0.5;
    int limiteTribuna = totalAsientos * 0.8;

    for (int i = 1; i <= totalAsientos; i++)
    {
        TipoAsiento categoria;

        if (i <= limiteGeneral)
            categoria = GENERAL;
        else if (i <= limiteTribuna)
            categoria = TRIBUNA;
        else
            categoria = PALCO;

        boleteria.insertarPorFinal(Boleto(i, categoria));
    }
}
