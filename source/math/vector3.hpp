/*
The MIT License (MIT)

Copyright (c) 2006-2016 Thomas Fussell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#pragma once
#include <cmath>
#include <iostream>

namespace cort {

struct vector3
{
    vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    vector3() : vector3(0, 0, 0) {}
    vector3(const vector3 &other) : vector3(other.x, other.y, other.z) {}

    void normalize() { auto l = length(); if(l == 0) return; x /= l; y /= l; z /= l; }
    double length() const { return std::sqrt(x * x + y * y + z * z); }
    double dot(vector3 other) const { return x * other.x + y * other.y + z * other.z; }

    vector3 &operator=(vector3 other) { x = other.x; y = other.y; z = other.z; return *this; }
    vector3 operator+(vector3 other) const { return vector3(x + other.x, y + other.y, z + other.z); }
    vector3 operator-(vector3 other) const { return vector3(x - other.x, y - other.y, z - other.z); }
    vector3 operator*(vector3 other) const { return vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }
    vector3 operator*(double scalar) const { return vector3(x * scalar, y * scalar, z * scalar); }
    vector3 operator/(double scalar) const { return vector3(x / scalar, y / scalar, z / scalar); }
    vector3 &operator+=(vector3 other) { *this = *this + other; return *this; }
    vector3 &operator-=(vector3 other) { *this = *this - other; return *this; }
    vector3 &operator*=(vector3 other) { *this = *this * other; return *this; }
    vector3 &operator*=(double scalar) { *this = *this * scalar; return *this; }
    vector3 &operator/=(double scalar) { *this = *this * (1 / scalar); return *this; }
    
    double x;
    double y;
    double z;
};

inline vector3 operator*(double scalar, vector3 vector) { return vector * scalar; }
inline vector3 operator/(double scalar, vector3 vector) { return vector / scalar; }

inline std::ostream &operator<<(std::ostream &stream, const vector3 &v)
{
    stream << "vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return stream;
}

} // namespace cort
