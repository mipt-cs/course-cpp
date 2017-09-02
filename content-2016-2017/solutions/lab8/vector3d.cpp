#include "vector3d.hpp"

#include <cmath>

const vector3d vector3d::X = {1, 0, 0};
const vector3d vector3d::Y = {0, 1, 0};
const vector3d vector3d::Z = {0, 0, 1};
const vector3d vector3d::ZERO = {0, 0, 0};

vector3d::vector3d(double x, double y, double z): x(x), y(y), z(z)
{

}

vector3d::vector3d(): vector3d(0, 0, 0)
{

}

double vector3d::length() const
{
    return std::sqrt((*this) * (*this));
}

void vector3d::normalize()
{
    double l = length();

    x /= l;
    y /= l;
    z /= l;
}

vector3d operator+(const vector3d& v1, const vector3d& v2)
{
    return vector3d(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
vector3d operator-(const vector3d& v1, const vector3d& v2)
{
    return vector3d(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vector3d operator-(const vector3d& v)
{
    return vector3d(-v.x, -v.y, -v.z);
}

double operator*(const vector3d& v1, const vector3d& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vector3d operator*(const vector3d& v, double q)
{
    return vector3d(v.x * q, v.y * q, v.z * q);
}

vector3d operator*(double q, const vector3d& v)
{
    return v * q;
}

vector3d operator^(const vector3d& v1, const vector3d& v2)
{
    return vector3d(v1.y  * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

bool operator==(const vector3d& v1, const vector3d& v2)
{
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

bool operator!=(const vector3d& v1, const vector3d& v2)
{
    return !(v1 == v2);
}

std::ostream& operator<<(std::ostream& os, const vector3d& v)
{
    return (os << "{" << v.x << ", " << v.y << ", " << v.z << "}");
}