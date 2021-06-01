#ifndef POINT_H
#define POINT_H

#include "coretypes.h"

#include <cmath>
#include <iostream>

template <typename T>
inline bool isNaN(const T x) {
    return std::isnan(x);
}
template <>
inline bool isNaN(const int x) {
    return false;
}

template <typename T>
class Point3 {
public:
    Point3() {
        x = y = z = 0;
    }
    Point3(T x, T y, T z) : x(x), y(y), z(z) {}
    Point3(const Point3<T>& p) {
        x = p.x;
        y = p.y;
        z = p.z;
    }

    Point3<T>& operator=(const Point3<T>& p) {
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }
    bool operator==(const Point3<T>& p) const {
        return x == p.x && y == p.y && z == p.z;
    }
    bool operator!=(const Point3<T>& p) const {
        return x != p.x || y != p.y || z != p.z;
    }
    Point3<T> operator+(const Point3<T>& p) const {
        return Point3<T>(x + p.x, y + p.y, z + p.z);
    }
    Point3<T> operator-(const Point3<T>& p) const {
        return Point3<T>(x - p.x, y - p.y, z - p.z);
    }
    template <typename U>
    Point3<T> operator/(U f) const {
        Float inv = (Float)1 / f;
        return Point3<T>(inv * x, inv * y, inv * z);
    }
    bool HasNaNs() const {
        return isNaN(x) || isNaN(y) || isNaN(z);
    }
    Point3<T> operator-() const {
        return Point3<T>(-x, -y, -z);
    }

public:
    T x, y, z;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Point3<T>& v) {
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
    return os;
}

typedef Point3<Float> Point3f;
typedef Point3<int> Point3i;

#endif