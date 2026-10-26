// Modelos

#pragma once

struct Point2D {
    double x;
    double y;
};

struct Point3D {
    double x;
    double y;
    double z;
};

template<typename T>
struct Closest {
    double dist;
    T i;
    T j;
};