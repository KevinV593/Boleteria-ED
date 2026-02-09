# Comando:

```bash
  
g++ -Imodel -Iview -Iutils -Icontroller -o beta4 controller/main.cpp model/ListaCircularDoble.cpp utils/Validaciones.cpp view/MenuUI.cpp view/MenuController.cpp view/StringProcessor.cpp controller/EventManager.cpp controller/ReservationManager.cpp controller/Exhaustiva.cpp controller/DivideYVenceras.cpp controller/Voraz.cpp controller/Dinamica.cpp controller/Backtracking.cpp
```


## Estructura Modular

```
view/
├── Menu.hpp (9 líneas - WRAPPER de compatibilidad)
├── MenuController.h/cpp (Orquestación del menú)
├── MenuUI.h/cpp (UI - mostrar opciones, asientos)
└── StringProcessor.h/cpp (Ordenamiento de caracteres)

controller/
├── EventManager.h/cpp (Inicializar boletería)
├── ReservationManager.h/cpp (Lógica de reservas)
└── main.cpp (ACTUALIZADO)
```


### **ReservationManager** (`controller/ReservationManager.h/cpp`)

**Responsabilidad:** Toda la lógica de gestión de reservas

**Funciones públicas:**

- `obtenerNombrePorCedula()` - Busca un cliente por cédula
- `reservarAsiento()` - Reserva un asiento (case 1)
- `cancelarReserva()` - Cancelar una o más reservas (case 2)
- `buscarReservasPorCedula()` - Búsqueda de reservas (case 4)
- `actualizarReserva()` - Actualizar datos de reserva (case 5)

**Líneas:** ~370 (sacada de Menu.hpp)

**Detalles:**

- Maneja confirmaciones simples (1 = sí, 0 = no)
- Integra con `Persistencia`, `ConcurrenciaOCC`, `Validaciones`
- Limpia pantalla internamente


### **MenuController** (`view/MenuController.h/cpp`)

**Responsabilidad:** Orquestación central del menú

**Funciones públicas:**

- `ejecutar()` - Loop principal del menú

**Funciones privadas:**

- `procesarOpcion()` - Switch que distribuye a los handlers correctos

**Lógica:**

```
Loop infinito:
  1. Mostrar menú (MenuUI)
  2. Leer opción
  3. Validar (1-8)
  4. Si opción=8 → salir
  5. Si opción=1 → ReservationManager::reservarAsiento()
  6. Si opción=2 → ReservationManager::cancelarReserva()
  7. Si opcion=3 → MenuUI::mostrarAsientos()
  8. Si opción=4 → ReservationManager::buscarReservasPorCedula()
  9. Si opción=5 → ReservationManager::actualizarReserva()
  10. Si opción=6 → StringProcessor::mostrarNombreOrdenado()
  11. Si opción=7 → StringProcessor::procesarPorGrupos()
```

**Líneas:** ~160 (nuevo, orquestra todo)


### Comando:

```bash
cd "c:\Users\pc\OneDrive\Dokumente\BOLETERIA\Proyecto-ED-GIT\Boleteria"

g++ -Imodel -Iview -Iutils -Icontroller \
  -o beta1 \
  controller/main.cpp \
  model/ListaCircularDoble.cpp \
  utils/Validaciones.cpp \
  view/MenuUI.cpp \
  view/MenuController.cpp \
  view/StringProcessor.cpp \
  controller/EventManager.cpp \
  controller/ReservationManager.cpp
```
## Navegación Rápida

- **Agregar nueva opción al menú** → `MenuController.cpp:procesarOpcion()`
- **Cambiar presentación del menú** → `MenuUI.cpp`
- **Agregar nueva función de reserva** → `ReservationManager.cpp`
- **Cambiar lógica de ordenamiento** → `StringProcessor.cpp`
- **Cambiar inicialización de boletería** → `EventManager.cpp`
