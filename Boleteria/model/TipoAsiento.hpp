#pragma once
#include <string>

enum TipoAsiento {
    GENERAL,
    TRIBUNA,
    PALCO
};

static const std::string PREFIJOS_CODIGO[] = { 
    "GEN", 
    "TRI", 
    "PAL" 
};

static const std::string NOMBRES_CATEGORIA[] = { 
    "General", 
    "Tribuna", 
    "Palco" 
};