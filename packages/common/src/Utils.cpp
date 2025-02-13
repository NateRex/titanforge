#include <common/Utils.h>
#include <common/Constants.h>
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

double rad2Deg(double rad)
{
    return rad * 180. / PI;
}

double deg2Rad(double deg)
{
    return deg * PI / 180.;
}