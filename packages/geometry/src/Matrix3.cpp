#include <geometry/Matrix3.h>
#include <geometry/Vector3.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <math.h>
#include <string>

const Matrix3 Matrix3::IDENTITY;

Matrix3::Matrix3()
{
	setValues(1, 0, 0, 0, 1, 0, 0, 0, 1);
}

Matrix3::Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
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
	_inverse = nullptr;
	_didComputeInverse = false;
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

Matrix3 Matrix3::fromRotation(const Vector3& axis, float radians, Matrix3* result)
{
	float c = cos(radians);
	float s = sin(radians);
	float omc = 1 - c;
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	float xy = axis.x * axis.y;
	float xz = axis.x * axis.z;
	float yz = axis.y * axis.z;

	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(
		c + x * x * omc,
		xy * omc - z * s,
		xz * omc + y * s,
		xy * omc + z * s,
		c + y * y * omc,
		yz * omc - x * s,
		xz * omc - y * s,
		yz * omc + x * s,
		c + z * z * omc);
	return r;
}

Matrix3 Matrix3::fromXRotation(float radians, Matrix3* result)
{
	float c = cos(radians);
	float s = sin(radians);

	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(1., 0., 0., 0., c, -s, 0., s, c);
	return r;
}

Matrix3 Matrix3::fromYRotation(float radians, Matrix3* result)
{
	float c = cos(radians);
	float s = sin(radians);

	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(c, 0., s, 0., 1., 0., -s, 0., c);
	return r;
}

Matrix3 Matrix3::fromZRotation(float radians, Matrix3* result)
{
	float c = cos(radians);
	float s = sin(radians);

	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(c, -s, 0., s, c, 0., 0., 0., 1.);
	return r;
}

bool Matrix3::isIdentity() const
{
	return equalTo(Matrix3::IDENTITY);
}

bool Matrix3::equalTo(const Matrix3& other, float tol) const
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
		result->setInverse(new Matrix3(*this));

		return true;
	}

	_didComputeInverse = true;

	float t00 = _m[8] * _m[4] - _m[5] * _m[7];
	float t01 = _m[5] * _m[6] - _m[8] * _m[3];
	float t02 = _m[7] * _m[3] - _m[4] * _m[6];

	float det = _m[0] * t00 + _m[1] * t01 + _m[2] * t02;
	
	if (equals(det, 0, 1.0e-11))
	{
		return false;
	}

	float detInv = 1. / det;

	float m00 = t00 * detInv;
	float m01 = (_m[2] * _m[7] - _m[8] * _m[1]) * detInv;
	float m02 = (_m[5] * _m[1] - _m[2] * _m[4]) * detInv;
	float m10 = t01 * detInv;
	float m11 = (_m[8] * _m[0] - _m[2] * _m[6]) * detInv;
	float m12 = (_m[2] * _m[3] - _m[5] * _m[0]) * detInv;
	float m20 = t02 * detInv;
	float m21 = (_m[1] * _m[6] - _m[7] * _m[0]) * detInv;
	float m22 = (_m[4] * _m[0] - _m[1] * _m[3]) * detInv;

	result->setValues(m00, m01, m02, m10, m11, m12, m20, m21, m22);
	result->setInverse(new Matrix3(*this));

	_inverse = new Matrix3(*result);
	return true;
}

Vector3 Matrix3::multiply(const Vector3& v, Vector3* result) const
{
	float x = _m[0] * v.x + _m[1] * v.y + _m[2] * v.z;
	float y = _m[3] * v.x + _m[4] * v.y + _m[5] * v.z;
	float z = _m[6] * v.x + _m[7] * v.y + _m[8] * v.z;

	Vector3& r = getOrDefault(result, Vector3());
	r.x = x;
	r.y = y;
	r.z = z;

	return r;
}

Matrix3 Matrix3::multiply(const Matrix3& m, Matrix3* result) const
{
	float m00 = _m[0] * m._m[0] + _m[1] * m._m[3] + _m[2] * m._m[6];
	float m01 = _m[0] * m._m[1] + _m[1] * m._m[4] + _m[2] * m._m[7];
	float m02 = _m[0] * m._m[2] + _m[1] * m._m[5] + _m[2] * m._m[8];
	float m10 = _m[3] * m._m[0] + _m[4] * m._m[3] + _m[5] * m._m[6];
	float m11 = _m[3] * m._m[1] + _m[4] * m._m[4] + _m[5] * m._m[7];
	float m12 = _m[3] * m._m[2] + _m[4] * m._m[5] + _m[5] * m._m[8];
	float m20 = _m[6] * m._m[0] + _m[7] * m._m[3] + _m[8] * m._m[6];
	float m21 = _m[6] * m._m[1] + _m[7] * m._m[4] + _m[8] * m._m[7];
	float m22 = _m[6] * m._m[2] + _m[7] * m._m[5] + _m[8] * m._m[8];

	Matrix3& r = getOrDefault(result, Matrix3());
	r.setValues(m00, m01, m02, m10, m11, m12, m20, m21, m22);

	return r;
}

float Matrix3::operator[](int i) const
{
	if (i < 0 || i > 8)
	{
		std::string errMsg = "Index " + std::to_string(i) + " out of range for 3x3 matrix";
		throw IllegalArgumentException(errMsg);
	}
	return _m[i];
}

Matrix3& Matrix3::operator=(const Matrix3& other)
{
	setValues(other);
	_didComputeInverse = other._didComputeInverse;
	_inverse = safeDelete(_inverse);
	if (other._inverse != nullptr)
	{
		setInverse(new Matrix3(*other._inverse));
	}

	return *this;
}

bool Matrix3::operator==(const Matrix3& other) const
{
	return equalTo(other);
}

bool Matrix3::operator!=(const Matrix3& other) const
{
	return !equalTo(other);
}

void Matrix3::setValues(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	clearInverse();

	_m[0] = m00;	_m[1] = m01;	_m[2] = m02;
	_m[3] = m10;	_m[4] = m11;	_m[5] = m12;
	_m[6] = m20;	_m[7] = m21;	_m[8] = m22;
}

void Matrix3::setValues(const Matrix3& other)
{
	clearInverse();

	for (int i = 0; i < 9; i++)
	{
		_m[i] = other._m[i];
	}
}

void Matrix3::clearInverse()
{
	_didComputeInverse = false;
	_inverse = safeDelete(_inverse);
}

void Matrix3::setInverse(Matrix3* inv)
{
	_didComputeInverse = true;
	safeDelete(_inverse);
	_inverse = inv;
}