#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <ostream>

class vector3d {
public:
    const static vector3d X;
    const static vector3d Y;
    const static vector3d Z;
    const static vector3d ZERO;

    double x, y, z;

    vector3d(double x, double y, double z);
    vector3d();

    double length() const;
    void normalize();

};

vector3d operator+(const vector3d& v1, const vector3d& v2);
vector3d operator-(const vector3d& v1, const vector3d& v2);

vector3d operator-(const vector3d& v);

double operator*(const vector3d& v1, const vector3d& v2);

vector3d operator*(const vector3d& v, double q);
vector3d operator*(double q, const vector3d& v);

vector3d operator^(const vector3d& v1, const vector3d& v2);

bool operator==(const vector3d& v1, const vector3d& v2);
bool operator!=(const vector3d& v1, const vector3d& v2);

std::ostream& operator<<(std::ostream& os, const vector3d& v);

#endif
