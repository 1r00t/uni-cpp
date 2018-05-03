#include "vec3f.h"

using namespace my;

// constructor
Vec3f::Vec3f(float x, float y, float z)
    : a_({x, y, z})
{}


// operator overloading
float& Vec3f::operator[](const int i)
{
    return a_[i];
}
const float& Vec3f::operator[](const int i) const
{
    return a_[i];
}
bool Vec3f::operator==(const Vec3f rhs) const
{
    return a_[0] == rhs[0] && a_[1] == rhs[1] && a_[2] == rhs[2];
}
bool Vec3f::operator!=(const Vec3f rhs) const
{
    return !(*this == rhs);
}
Vec3f& Vec3f::operator+=(const Vec3f& rhs)
{
    a_[0] += rhs[0];
    a_[1] += rhs[1];
    a_[2] += rhs[2];
    return *this;
}
Vec3f Vec3f::operator+(const Vec3f& rhs) const
{
    Vec3f temp(*this);
    temp += rhs;
    return temp;
}
Vec3f Vec3f::operator-() const
{
    return Vec3f(-a_[0], -a_[1], -a_[2]);
}

// free function
float my::dot(const Vec3f lhs, const Vec3f rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}
