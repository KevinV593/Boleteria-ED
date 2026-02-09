#include "Hash.hpp"
#include <fstream>
#include <iostream>

uint64_t calcularHashArchivo(const std::string& ruta) {
    const uint64_t FNV_OFFSET = 14695981039346656037ULL;
    const uint64_t FNV_PRIME  = 1099511628211ULL;

    uint64_t hash = FNV_OFFSET;
    std::ifstream file(ruta, std::ios::binary);

    if (!file) return 0;

    char c;
    while (file.get(c)) {
        hash ^= static_cast<unsigned char>(c);
        hash *= FNV_PRIME;
    }

    return hash;
}

void guardarHash(const std::string& ruta, const std::string& archivoHash) {
    uint64_t hash = calcularHashArchivo(ruta);
    std::ofstream out(archivoHash);
    out << hash;
}

bool verificarIntegridad(const std::string& ruta, const std::string& archivoHash) {
    std::ifstream in(archivoHash);
    if (!in) {
        guardarHash(ruta, archivoHash);
        return true;
    }

    uint64_t hashGuardado;
    in >> hashGuardado;

    uint64_t hashActual = calcularHashArchivo(ruta);

    return hashGuardado == hashActual;
}