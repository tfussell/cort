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

#include "vector3.hpp"

namespace cort {

class quaternion
{
public:
    static quaternion from_yaw_pitch_roll(const vector3 &v)
    {
        return from_yaw_pitch_roll(v.x, v.y, v.z);
    }

    static quaternion from_yaw_pitch_roll(double yaw, double pitch, double roll)
    {
        double cy = std::cos(yaw / 2);
        double sy = std::sin(yaw / 2);
        double cr = std::cos(-roll / 2);
        double sr = std::sin(-roll / 2);
        double cp = std::cos(-pitch / 2);
        double sp = std::sin(-pitch / 2);

        return quaternion(cr * cp * cy - sr * sp * sy,
        		  cr * sp * cy - sr * cp * sy,
        		  cr * cp * sy + sr * sp * cy,
        		  sr * cp * cy + cr * sp * sy);
    }

    quaternion(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {}
    quaternion(double w, vector3 v) : w(w), x(v.x), y(v.y), z(v.z) {}
    quaternion() : quaternion(0, 0, 0, 0) {}
    quaternion(const quaternion &other) : quaternion(other.w, other.x, other.y, other.z) {}

    void normalize() { auto l = length(); if(l == 0) return; w /= l; x /= l; y /= l; z /= l; }
    double length() const { return std::sqrt(w * w + x * x + y * y + z * z); }

    quaternion conjugate() const { return quaternion(w, vector3(x, y, z) * -1); }

    quaternion &operator=(quaternion other) { w = other.w; x = other.x; y = other.y; z = other.z; return *this; }
    quaternion operator+(quaternion other) const { return quaternion(w + other.w, x + other.x, y + other.y, z + other.z); }
    quaternion operator-(quaternion other) const { return quaternion(w - other.w, x - other.x, y - other.y, z - other.z); }
    vector3 operator*(const vector3 &v) const { return (*this * quaternion(0, v) * conjugate()).vector_part(); }
    quaternion operator*(quaternion other) const 
    {
        auto v1 = vector_part(); 
        auto v2 = other.vector_part(); 
        return quaternion(w * other.w - v1.dot(v2), w * v2 + other.w * v1 + v1 * v2);
    }
    quaternion operator*(double scalar) const { return quaternion(w * scalar, x * scalar, y * scalar, z * scalar); }
    quaternion operator/(double scalar) const { return quaternion(w / scalar, x / scalar, y / scalar, z / scalar); }

    vector3 vector_part() const { return vector3(x, y, z); }
    
    double w;
    double x;
    double y;
    double z;
};

inline std::ostream &operator<<(std::ostream &stream, const quaternion &q)
{
    stream << "quaternion(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
    return stream;
}

} // namespace cort
