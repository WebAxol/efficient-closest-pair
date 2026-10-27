# Par de puntos más cercano (Divide y Vencerás) en 2D y 3D

Implementación en C++ del algoritmo de par más cercano con Divide y Vencerás (O(n log n) en 2D, O(n log² n) en 3D), validado contra fuerza bruta O(n²). Incluye un script en Python para graficar los resultados.

## Requisitos

- g++ con soporte para C++17
- Python 3 con `matplotlib` (solo para graficar)

```bash
pip install matplotlib
```

## Compilar

Desde la raíz del proyecto:

```bash
g++ -std=c++17 -O2 -o closest src/main.cpp src/utils.cpp
```

## Ejecutar

Se debe ejecutar desde la raíz, porque las rutas `in/` y `out/` son relativas:

```bash
./closest
```

El programa corre los casos 2D y 3D, compara el resultado eficiente contra el de fuerza bruta e imprime `PASA` o `FALLA` por cada caso.

## Graficar

Después de ejecutar el programa:

```bash
python3 graph.py          # guarda las imágenes
python3 graph.py --show   # además abre las ventanas
```

## Entradas y salidas

| Tipo | Ruta | Descripción |
|------|------|-------------|
| Entrada | `in/input-2d.txt` | Casos de prueba 2D |
| Entrada | `in/input-3d.txt` | Casos de prueba 3D |
| Salida | `out/output-2d.csv` | Par más cercano por caso (2D) |
| Salida | `out/output-3d.csv` | Par más cercano por caso (3D) |
| Salida | `out/img/*-plot.png` | Gráficas generadas por `graph.py` |

La carpeta `out/` se crea automáticamente y está ignorada por git.

### Formato de entrada

```
T
nombre1 N1
x y [z]
...
nombre2 N2
...
```

`T` es el número de casos; cada caso tiene un nombre, la cantidad de puntos `N` y luego una línea por punto (`x y` en 2D, `x y z` en 3D).

### Formato de salida (CSV)

Una línea por caso: `nombre,distancia,coordenadas_punto_1,coordenadas_punto_2`.

## Estructura

```
src/main.cpp    Algoritmos (fuerza bruta y D&C) y ejecución de casos
src/models.h    Point2D, Point3D y Closest<T>
src/utils.*     Distancia euclidiana 2D y 3D
graph.py        Gráficas con matplotlib
research.tex    Documento técnico del algoritmo en 3D
```
