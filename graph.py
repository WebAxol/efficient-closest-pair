import csv
import os
import sys

import matplotlib.pyplot as plt

OUT_DIR = os.path.join("out", "img")


def read_input(path):
    """Devuelve {nombre: [(x, y[, z]), ...]} respetando el orden del archivo."""
    with open(path) as f:
        tokens = f.read().split()

    pos = 0
    cases = {}
    t = int(tokens[pos]); pos += 1
    for _ in range(t):
        name = tokens[pos]; pos += 1
        n = int(tokens[pos]); pos += 1
        points = []
        for _ in range(n):
            points.append(tuple(float(v) for v in tokens[pos:pos + dim_of(path)]))
            pos += dim_of(path)
        cases[name] = points
    return cases


def dim_of(path):
    return 3 if "3d" in path else 2


def read_output(path, dim):
    """Devuelve {nombre: (dist, punto_i, punto_j)}.
    Formato: nombre,dist,coords_i...,coords_j..."""
    results = {}
    with open(path, newline="") as f:
        for row in csv.reader(f):
            if not row:
                continue
            name, dist = row[0], float(row[1])
            vals = [float(v) for v in row[2:]]
            results[name] = (dist, tuple(vals[:dim]), tuple(vals[dim:2 * dim]))
    return results


def sq_dist(a, b):
    return sum((u - v) ** 2 for u, v in zip(a, b))


def snap(point, points):
    """El CSV exporta con precisión limitada, se busca el punto original más cercano."""
    return min(points, key=lambda p: sq_dist(p, point))


def plot_case(name, points, result, dim):
    dist, p1, p2 = result
    p1, p2 = snap(p1, points), snap(p2, points)

    fig = plt.figure(figsize=(7, 6))
    ax = fig.add_subplot(111, projection="3d") if dim == 3 else fig.add_subplot(111)

    others = [p for p in points if p != p1 and p != p2]
    pair = [p1, p2]

    # Todos los puntos en azul, el par más cercano en rojo
    ax.scatter(*zip(*others), color="tab:blue", s=25, label="Puntos")
    ax.scatter(*zip(*pair), color="red", s=60, label="Par más cercano")
    ax.plot(*zip(*pair), color="red", linewidth=1.5)

    ax.set_title(f"{name} ({dim}D, n={len(points)})  distancia = {dist:.5f}")
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    if dim == 3:
        ax.set_zlabel("z")
    else:
        ax.set_aspect("equal", adjustable="datalim")
        ax.grid(True, alpha=0.3)
    ax.legend()
    fig.tight_layout()
    return fig


def visualize(input_path, output_path, dim, show=False):
    cases = read_input(input_path)
    results = read_output(output_path, dim)

    for name, points in cases.items():
        if name not in results:
            print(f"Sin resultado en {output_path} para '{name}', se omite")
            continue
        fig = plot_case(name, points, results[name], dim)
        os.makedirs(OUT_DIR, exist_ok=True)
        fig.savefig(os.path.join(OUT_DIR, f"{name.rsplit('.', 1)[0]}-plot.png"), dpi=150)
        if not show:
            plt.close(fig)


if __name__ == "__main__":
    # Siempre guarda las imágenes en out/img; con --show además abre las ventanas
    show = "--show" in sys.argv

    visualize("in/input-2d.txt", "out/output-2d.csv", 2, show)
    visualize("in/input-3d.txt", "out/output-3d.csv", 3, show)

    if show:
        plt.show()
