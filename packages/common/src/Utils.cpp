#include <common/Utils.h>
#include <cmath>

bool equals(double a, double b, double tol)
{
    return fabs(a - b) <= tol;
}

void safeDelete(void* ptr)
{
    if (ptr != nullptr)
    {
        delete ptr;
    }
}