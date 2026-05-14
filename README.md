# Magic CarPet Fly - modern SDL2 port

Port moderno del demo Magic Car Pet 3d-project en DJGPP/DOS de 1996 a C++17 + SDL2.    
Copyright (c) 1996 Aatu Koskensilta (Zaphod.B) 

## Qué conserva

- Código original del engine (`mcpet.cpp`) y texturizador (`tpf.cpp`) casi sin cambios.
- Assets originales: `fade.dat`, `h256.pcx`, `g256.pcx`, `t1.pcc` ... `t6.pcc`.
- Resolución lógica 320x200, escalada en una ventana SDL.

## Qué se reemplazó

- VGA modo `0x13` -> ventana SDL2.
- Handler de teclado DOS -> eventos SDL2.
- Assembler fixed-point x86 -> C++ portable con `int64_t`.
- `iostream.h` pre-standard -> eliminado.
- `grp.cc`, que faltaba en los sources originales, fue reimplementado como `grp_sdl.cpp`.

## Screenshots:

<p align="center">
  <img src="screenshots/scr01.png" width="45%">
  <img src="screenshots/scr02.png" width="45%">
</p>

## Teclas

- ESC: salir
- Flechas: acelerar/frenar/girar
- Q/A: subir/bajar
- PageUp/PageDown: mirar arriba/abajo
- U/J: tilt, gira la cámara.

## Linux / WSL con entorno gráfico

```bash
sudo apt install build-essential cmake libsdl2-dev gdb
cmake -S . -B build
cmake --build build
./build/fly
```

## Nota

Este port es intencionalmente mínimo. No intenta modernizar el motor ni corregir lógica interna; solo reemplaza las partes DOS/VGA para que vuelva a ejecutar hoy.
