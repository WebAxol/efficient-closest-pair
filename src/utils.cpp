#include <cmath>
#include "utils.h"

// Distancia euclidiana en 2D
double dist(Point2D point1, Point2D point2) {
    double dx = point1.x - point2.x;
    double dy = point1.y - point2.y;

    return std::sqrt(dx * dx + dy * dy);
}

// Distancia euclidiana en 3D
double dist(Point3D point1, Point3D point2) {
    double dx = point1.x - point2.x;
    double dy = point1.y - point2.y;
    double dz = point1.z - point2.z;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}