#include <geometry/Utils.h>
#include <cmath>

bool equals(double a, double b, double tol)
{
    return fabs(a - b) <= tol;
}