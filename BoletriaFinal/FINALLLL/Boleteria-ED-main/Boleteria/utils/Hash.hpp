#pragma once
#include <string>
#include <cstdint>

uint64_t calcularHashArchivo(const std::string& ruta);
bool verificarIntegridad(const std::string& ruta, const std::string& archivoHash);
void guardarHash(const std::string& ruta, const std::string& archivoHash);