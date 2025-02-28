#include <common/Utils.h>
#include <common/Constants.h>
#include <cmath>

bool equals(float a, float b, float tol)
{
    return fabs(a - b) <= tol;
}

float rad2Deg(float rad)
{
    return rad * 180. / PI;
}

float deg2Rad(float deg)
{
    return deg * PI / 180.;
}