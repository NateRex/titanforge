#include <geometry/Matrix3.h>
#include <geometry/Vector3.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <math.h>
#include <string>
#include <optional>

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
	for (int i = 0; i < 9; i++)
	{
		_m[i] = other._m[i];
	}

	_didComputeInverse = other._didComputeInverse;
	if (other._inverse != nullptr)
	{
		_inverse = new Matrix3(*other._inverse);
	}
}

Matrix3::~Matrix3()
{
	if (_inverse != nullptr)
	{
		delete _inverse;
	}
}

Matrix3 Matrix3::fromRows(const Vector3& r0, const Vector3& r1, const Vector3& r2, Matrix3* result)
{
	Matrix3 r = getOrDefault(result, Matrix3());
	r.setValues(
		r0.x, r0.y, r0.z,
		r1.x, r1.y, r1.z,
		r2.x, r2.y, r2.z);
	return r;
}

Matrix3 Matrix3::fromColumns(const Vector3& c0, const Vector3& c1, const Vector3& c2, Matrix3* result)
{
	Matrix3 r = getOrDefault(result, Matrix3());
	r.setValues(
		c0.x, c1.x, c2.x,
		c0.y, c1.y, c2.y,
		c0.z, c1.z, c2.z);
	return r;
}

Matrix3 Matrix3::fromXRotation(double radians)
{
	double c = cos(radians);
	double s = sin(radians);
	return Matrix3(1., 0., 0., 0., c, -s, 0., s, c);
}

Matrix3 Matrix3::fromYRotation(double radians)
{
	double c = cos(radians);
	double s = sin(radians);
	return Matrix3(c, 0., s, 0., 1., 0., -s, 0., c);
}

Matrix3 Matrix3::fromZRotation(double radians)
{
	double c = cos(radians);
	double s = sin(radians);
	return Matrix3(c, -s, 0., s, c, 0., 0., 0., 1.);
}

Matrix3 Matrix3::transpose(Matrix3* result) const
{
	Matrix3 r = getOrDefault(result, Matrix3());
	r.setValues(
		_m[0], _m[3], _m[6],
		_m[1], _m[4], _m[7],
		_m[2], _m[5], _m[8]);
	return r;
}

std::optional<Matrix3> Matrix3::inverse(Matrix3* result)
{
	if (_didComputeInverse)
	{
		return _inverse == nullptr ? std::nullopt : std::optional<Matrix3>{ *_inverse };
	}
	_didComputeInverse = true;

	double t00 = _m[8] * _m[4] - _m[5] * _m[7];
	double t01 = _m[5] * _m[6] - _m[8] * _m[3];
	double t02 = _m[7] * _m[3] - _m[4] * _m[6];

	double det = _m[0] * t00 + _m[1] * t01 + _m[2] * t02;
	
	if (equals(det, 0, 1.0e-11))
	{
		return std::nullopt;
	}

	double detInv = 1. / det;

	double m00 = t01 * detInv;
	double m01 = (_m[2] * _m[7] - _m[8] * _m[1]) * detInv;
	double m02 = (_m[5] * _m[1] - _m[2] * _m[4]) * detInv;
	double m10 = t01 * detInv;
	double m11 = (_m[8] * _m[0] - _m[2] * _m[6]) * detInv;
	double m12 = (_m[2] * _m[3] - _m[5] * _m[0]) * detInv;
	double m20 = t02 * detInv;
	double m21 = (_m[1] * _m[6] - _m[7] * _m[0]) * detInv;
	double m22 = (_m[4] * _m[0] - _m[1] * _m[3]) * detInv;

	Matrix3 r = getOrDefault(result, Matrix3());
	r.setValues(m00, m01, m02, m10, m11, m12, m20, m21, m22);

	// Set this matrix's inverse, as well as the inverse of the new matrix
	safeDelete(r._inverse);
	r._didComputeInverse = true;
	r._inverse = new Matrix3(*this);
	_inverse = new Matrix3(r);

	return std::optional<Matrix3>{r};
}

Vector3 Matrix3::multiply(const Vector3& v, Vector3* result) const
{
	double x = v.x;
	double y = v.y;
	double z = v.z;

	Vector3 r = getOrDefault(result, Vector3());
	r.x = _m[0] * x + _m[1] * y + _m[2] * z;
	r.y = _m[3] * x + _m[4] * y + _m[5] * z;
	r.z = _m[6] * x + _m[7] * y + _m[8] * z;

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

	Matrix3 r = getOrDefault(result, Matrix3());
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

void Matrix3::setValues(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22)
{
	_m[0] = m00;	_m[1] = m10;	_m[2] = m20;
	_m[3] = m01;	_m[4] = m11;	_m[5] = m21;
	_m[6] = m02;	_m[7] = m12;	_m[8] = m22;
}