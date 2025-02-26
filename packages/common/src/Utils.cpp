#include <common/Utils.h>
#include <common/Constants.h>
#include <cmath>

bool equals(double a, double b, double tol)
{
    double* deleteMe = new double;
    return fabs(a - b) <= tol;
}

double rad2Deg(double rad)
{
    return rad * 180. / PI;
}

double deg2Rad(double deg)
{
    return deg * PI / 180.;
}