#include <geometry/Vector2.h>
#include <geometry/Utils.h>
#include <math.h>

Vector2::Vector2(): x(0.0), y(0.0) {

}

Vector2::Vector2(double x, double y): x(x), y(y) {

}

Vector2::Vector2(const Vector2& other): x(other.x), y(other.y) {
    
}

bool Vector2::isZero(double tol) const {
    return equals(x, 0.0, tol) && equals(y, 0.0, tol);
}

bool Vector2::equalTo(const Vector2& other, double tol) const {
    return equals(x, other.x, tol) && equals(y, other.y, tol);
}

double Vector2::getMagnitude() const {
    return sqrt(x * x + y * y);
}

double Vector2::dot(const Vector2& other) const {
    return x * other.x + y * other.y;
}

double Vector2::cross(const Vector2& other) const {
    return x * other.y - y * other.x;
}

Vector2 Vector2::normalize(Vector2* result) const {
    double mag = getMagnitude();
    
    Vector2 res = getOrDefault(result, Vector2());
    if (isZero()) {
        res.x = 0;
        res.y = 0;
    }
    else {
        res.x = x / mag;
        res.y = y / mag;
    }
    
    return res;
}

Vector2 Vector2::scale(double value, Vector2* result) const {
    Vector2 res = getOrDefault(result, Vector2());
    res.x = x * value;
    res.y = y * value;
    return res;
}

Vector2 Vector2::minus(const Vector2& other, Vector2* result) const {
    Vector2 res = getOrDefault(result, Vector2());
    res.x = x - other.x;
    res.y = y - other.y;
    return res;
}

bool Vector2::operator==(const Vector2& other) const {
    return x == other.x && y == other.y;
}