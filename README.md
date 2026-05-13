# Magic Car Pet / Fly - modern SDL2 port

Port moderno del demo DJGPP/DOS de 1996 a C++17 + SDL2.

## Qué conserva

- Código original del engine (`mcpet.cpp`) y texturizador (`tpf.cpp`) casi sin cambios.
- Assets originales: `fade.dat`, `h256.pcx`, `g256.pcx`, `t1.pcc` ... `t6.pcc`.
- Resolución lógica 320x200, escalada en una ventana SDL.

## Qué se reemplazó

- VGA modo `0x13` -> ventana SDL2.
- Handler de teclado DOS -> eventos SDL2.
- Assembler fixed-point x86 -> C++ portable con `int64_t`.
- `iostream.h` pre-standard -> eliminado.
- `grp.cc`, que faltaba en los sources, fue reimplementado como `grp_sdl.cpp`.

## Teclas

- ESC: salir
- Flechas: acelerar/frenar/girar
- Q/A: subir/bajar
- PageUp/PageDown: mirar arriba/abajo
- U/J: tilt, según el código original. El `READ.ME` decía U/I, pero el fuente usa U/J.

## Linux / WSL con entorno gráfico

```bash
sudo apt install build-essential cmake libsdl2-dev gdb
cmake -S . -B build
cmake --build build
./build/fly
```

## Nota

Este port es intencionalmente mínimo. No intenta modernizar el motor ni corregir lógica interna; solo reemplaza las partes DOS/VGA para que vuelva a ejecutar hoy.
