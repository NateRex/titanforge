#include <geometry/Matrix3.h>
#include <geometry/Vector3.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <math.h>
#include <string>

Matrix3::Matrix3()
{
	setValues(1, 0, 0, 0, 1, 0, 0, 0, 1);
}

Matrix3::Matrix3(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22)
{
	setValues(m00, m01, m02, m10, m11, m12, m20, m21, m22);
}

Matrix3::Matrix3(const Matrix3& other)
{
	setValues(other);
}

Matrix3::~Matrix3()
{
	safeDelete(_inverse);
}

Matrix3 Matrix3::fromRows(const Vector3& r0, const Vector3& r1, const Vector3& r2, Matrix3* result)
{
	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(
		r0.x, r0.y, r0.z,
		r1.x, r1.y, r1.z,
		r2.x, r2.y, r2.z);
	return r;
}

Matrix3 Matrix3::fromColumns(const Vector3& c0, const Vector3& c1, const Vector3& c2, Matrix3* result)
{
	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(
		c0.x, c1.x, c2.x,
		c0.y, c1.y, c2.y,
		c0.z, c1.z, c2.z);
	return r;
}

Matrix3 Matrix3::fromXRotation(double radians, Matrix3* result)
{
	double c = cos(radians);
	double s = sin(radians);

	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(1., 0., 0., 0., c, -s, 0., s, c);
	return r;
}

Matrix3 Matrix3::fromYRotation(double radians, Matrix3* result)
{
	double c = cos(radians);
	double s = sin(radians);

	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(c, 0., s, 0., 1., 0., -s, 0., c);
	return r;
}

Matrix3 Matrix3::fromZRotation(double radians, Matrix3* result)
{
	double c = cos(radians);
	double s = sin(radians);

	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(c, -s, 0., s, c, 0., 0., 0., 1.);
	return r;
}

bool Matrix3::isIdentity() const
{
	return equalTo(IDENTITY_MATRIX);
}

bool Matrix3::equalTo(const Matrix3& other, double tol) const
{
	for (int i = 0; i < 9; i++)
	{
		if (!equals(_m[i], other._m[i], tol))
		{
			return false;
		}
	}
	
	return true;
}

Matrix3 Matrix3::transpose(Matrix3* result) const
{
	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(
		_m[0], _m[3], _m[6],
		_m[1], _m[4], _m[7],
		_m[2], _m[5], _m[8]);
	return r;
}

bool Matrix3::inverse(Matrix3* result)
{
	if (_didComputeInverse)
	{
		if (_inverse == nullptr)
		{
			return false;
		}

		result->setValues(*_inverse);
		return true;
	}
	_didComputeInverse = true;

	double t00 = _m[8] * _m[4] - _m[5] * _m[7];
	double t01 = _m[5] * _m[6] - _m[8] * _m[3];
	double t02 = _m[7] * _m[3] - _m[4] * _m[6];

	double det = _m[0] * t00 + _m[1] * t01 + _m[2] * t02;
	
	if (equals(det, 0, 1.0e-11))
	{
		return false;
	}

	double detInv = 1. / det;

	double m00 = t00 * detInv;
	double m01 = (_m[2] * _m[7] - _m[8] * _m[1]) * detInv;
	double m02 = (_m[5] * _m[1] - _m[2] * _m[4]) * detInv;
	double m10 = t01 * detInv;
	double m11 = (_m[8] * _m[0] - _m[2] * _m[6]) * detInv;
	double m12 = (_m[2] * _m[3] - _m[5] * _m[0]) * detInv;
	double m20 = t02 * detInv;
	double m21 = (_m[1] * _m[6] - _m[7] * _m[0]) * detInv;
	double m22 = (_m[4] * _m[0] - _m[1] * _m[3]) * detInv;

	result->setValues(m00, m01, m02, m10, m11, m12, m20, m21, m22);

	// Set this matrix's inverse, as well as the inverse of the new matrix
	safeDelete(result->_inverse);
	result->_didComputeInverse = true;
	result->_inverse = new Matrix3(*this);
	_inverse = new Matrix3(*result);

	return true;
}

Vector3 Matrix3::multiply(const Vector3& v, Vector3* result) const
{
	double x = _m[0] * v.x + _m[1] * v.y + _m[2] * v.z;
	double y = _m[3] * v.x + _m[4] * v.y + _m[5] * v.z;
	double z = _m[6] * v.x + _m[7] * v.y + _m[8] * v.z;

	Vector3& r = getOrDefault(result, Vector3());
	r.x = x;
	r.y = y;
	r.z = z;

	return r;
}

Matrix3 Matrix3::multiply(const Matrix3& m, Matrix3* result) const
{
	double m00 = _m[0] * m._m[0] + _m[1] * m._m[3] + _m[2] * m._m[6];
	double m01 = _m[0] * m._m[1] + _m[1] * m._m[4] + _m[2] * m._m[7];
	double m02 = _m[0] * m._m[2] + _m[1] * m._m[5] + _m[2] * m._m[8];
	double m10 = _m[3] * m._m[0] + _m[4] * m._m[3] + _m[5] * m._m[6];
	double m11 = _m[3] * m._m[1] + _m[4] * m._m[4] + _m[5] * m._m[7];
	double m12 = _m[3] * m._m[2] + _m[4] * m._m[5] + _m[5] * m._m[8];
	double m20 = _m[6] * m._m[0] + _m[7] * m._m[3] + _m[8] * m._m[6];
	double m21 = _m[6] * m._m[1] + _m[7] * m._m[4] + _m[8] * m._m[7];
	double m22 = _m[6] * m._m[2] + _m[7] * m._m[5] + _m[8] * m._m[8];

	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(m00, m01, m02, m10, m11, m12, m20, m21, m22);

	return r;
}

double Matrix3::operator[](int i) const
{
	if (i < 0 || i > 8)
	{
		std::string errMsg = "Index " + std::to_string(i) + " out of range for 3x3 matrix";
		throw IllegalArgumentException(errMsg);
	}
	return _m[i];
}

bool Matrix3::operator==(const Matrix3& other) const
{
	return equalTo(other);
}

void Matrix3::setValues(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22)
{
	_m[0] = m00;	_m[1] = m01;	_m[2] = m02;
	_m[3] = m10;	_m[4] = m11;	_m[5] = m12;
	_m[6] = m20;	_m[7] = m21;	_m[8] = m22;
}

void Matrix3::setValues(const Matrix3& other)
{
	for (int i = 0; i < 9; i++)
	{
		_m[i] = other._m[i];
	}

	_didComputeInverse = other._didComputeInverse;
	if (other._inverse != nullptr)
	{
		safeDelete(_inverse);
		_inverse = new Matrix3(*other._inverse);
	}
}