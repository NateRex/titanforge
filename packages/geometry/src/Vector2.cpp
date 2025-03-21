#include <geometry/Vector2.h>
#include <geometry/Vector3.h>
#include <common/Utils.h>
#include <math.h>

Vector2::Vector2(): x(0.0), y(0.0)
{

}

Vector2::Vector2(float x, float y): x(x), y(y)
{

}

Vector2::Vector2(const Vector2& other): x(other.x), y(other.y)
{
    
}

Vector2::Vector2(const Vector3& other) : x(other.x), y(other.y)
{

}

bool Vector2::isZero(float tol) const
{
    return equals(x, 0.0, tol) && equals(y, 0.0, tol);
}

bool Vector2::equalTo(const Vector2& other, float tol) const
{
    return equals(x, other.x, tol) && equals(y, other.y, tol);
}

float Vector2::getMagnitude() const
{
    return sqrt(x * x + y * y);
}

float Vector2::dot(const Vector2& other) const
{
    return x * other.x + y * other.y;
}

float Vector2::cross(const Vector2& other) const
{
    return x * other.y - y * other.x;
}

Vector2 Vector2::normalize(Vector2* result) const
{
    float mag = getMagnitude();
    
    Vector2& r = getOrDefault(result, Vector2());

    if (isZero())
    {
        r.x = 0;
        r.y = 0;
    }
    else
    {
        r.x = x / mag;
        r.y = y / mag;
    }
    
    return r;
}

Vector2 Vector2::scale(float value, Vector2* result) const
{
    Vector2& r = getOrDefault(result, Vector2());

    r.x = x * value;
    r.y = y * value;
    return r;
}

Vector2 Vector2::minus(const Vector2& other, Vector2* result) const
{
    Vector2& r = getOrDefault(result, Vector2());

    r.x = x - other.x;
    r.y = y - other.y;
    return r;
}

bool Vector2::operator==(const Vector2& other) const
{
    return equalTo(other);
}

bool Vector2::operator!=(const Vector2& other) const
{
    return !equalTo(other);
}