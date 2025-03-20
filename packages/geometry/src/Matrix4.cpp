#include <geometry/Matrix4.h>
#include <geometry/Matrix3.h>
#include <geometry/Vector3.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <string>

const Matrix4 Matrix4::IDENTITY;

Matrix4::Matrix4()
{
	setValues(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

Matrix4::Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	setValues(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33);
}

Matrix4::Matrix4(const Matrix4& other)
{
	setValues(other);
}

Matrix4::~Matrix4()
{
	safeDelete(_inverse);
}

Matrix4 Matrix4::fromScaling(float scalar, Matrix4* result)
{
	return Matrix4::fromScaling(scalar, scalar, scalar, result);
}

Matrix4 Matrix4::fromScaling(float x, float y, float z, Matrix4* result)
{
	Matrix4& r = getOrDefault(result, Matrix4());
	r.setValues(
		x, 0., 0., 0.,
		0., y, 0., 0.,
		0., 0., z, 0.,
		0., 0., 0., 1.);
	return r;
}

Matrix4 Matrix4::fromRotation(const Matrix3& rot, Matrix4* result)
{
	Matrix4& r = getOrDefault(result, Matrix4());
	r.setValues(
		rot[0], rot[1], rot[2], 0.,
		rot[3], rot[4], rot[5], 0.,
		rot[6], rot[7], rot[8], 0.,
		0., 0., 0., 1.);
	return r;
}

Matrix4 Matrix4::fromTranslation(const Vector3& v, Matrix4* result)
{
	Matrix4& r = getOrDefault(result, Matrix4());
	r.setValues(
		0., 0., 0., v.x,
		0., 0., 0., v.y,
		0., 0., 0., v.z,
		0., 0., 0., 1.);
	return r;
}

bool Matrix4::isIdentity() const
{
	return equalTo(Matrix4::IDENTITY);
}

bool Matrix4::equalTo(const Matrix4& other, float tol) const
{
	for (int i = 0; i < 16; i++)
	{
		if (!equals(_m[i], other._m[i], tol))
		{
			return false;
		}
	}
	return true;
}

Matrix4 Matrix4::transpose(Matrix4* result) const
{
	Matrix4& r = getOrDefault(result, Matrix4());
	r.setValues(
		_m[0], _m[4], _m[8], _m[12],
		_m[1], _m[5], _m[9], _m[13],
		_m[2], _m[6], _m[10], _m[14],
		_m[3], _m[7], _m[11], _m[15]);
	return r;
}

bool Matrix4::inverse(Matrix4* result)
{
	if (_didComputeInverse)
	{
		if (_inverse == nullptr)
		{
			return false;
		}

		result->setValues(*_inverse);
		result->setInverse(new Matrix4(*this));

		return true;
	}

	_didComputeInverse = true;

	float t00 = _m[9] * _m[14] * _m[7] - _m[13] * _m[10] * _m[7] + _m[13] * _m[6] * _m[11] - _m[5] * _m[14] * _m[11] - _m[9] * _m[6] * _m[15] + _m[5] * _m[10] * _m[15];
	float t01 = _m[12] * _m[10] * _m[7] - _m[8] * _m[14] * _m[7] - _m[12] * _m[6] * _m[11] + _m[4] * _m[14] * _m[11] + _m[8] * _m[6] * _m[15] - _m[4] * _m[10] * _m[15];
	float t02 = _m[8] * _m[13] * _m[7] - _m[12] * _m[9] * _m[7] + _m[12] * _m[5] * _m[11] - _m[4] * _m[13] * _m[11] - _m[8] * _m[5] * _m[15] + _m[4] * _m[9] * _m[15];
	float t03 = _m[12] * _m[9] * _m[6] - _m[8] * _m[13] * _m[6] - _m[12] * _m[5] * _m[10] + _m[4] * _m[13] * _m[10] + _m[8] * _m[5] * _m[14] - _m[4] * _m[9] * _m[14];

	float det = _m[0] * t00 + _m[1] * t01 + _m[2] * t02 + _m[3] * t03;

	if (equals(det, 0, 1.0e-11)) {
		return false;
	}

	float detInv = 1. / det;

	float m00 = t00 * detInv;
	float m01 = (_m[13] * _m[10] * _m[3] - _m[9] * _m[14] * _m[3] - _m[13] * _m[2] * _m[11] + _m[1] * _m[14] * _m[11] + _m[9] * _m[2] * _m[15] - _m[1] * _m[10] * _m[15]) * detInv;
	float m02 = (_m[5] * _m[14] * _m[3] - _m[13] * _m[6] * _m[3] + _m[13] * _m[2] * _m[7] - _m[1] * _m[14] * _m[7] - _m[5] * _m[2] * _m[15] + _m[1] * _m[6] * _m[15]) * detInv;
	float m03 = (_m[9] * _m[6] * _m[3] - _m[5] * _m[10] * _m[3] - _m[9] * _m[2] * _m[7] + _m[1] * _m[10] * _m[7] + _m[5] * _m[2] * _m[11] - _m[1] * _m[6] * _m[11]) * detInv;
	float m10 = t01 * detInv;
	float m11 = (_m[8] * _m[14] * _m[3] - _m[12] * _m[10] * _m[3] + _m[12] * _m[2] * _m[11] - _m[0] * _m[14] * _m[11] - _m[8] * _m[2] * _m[15] + _m[0] * _m[10] * _m[15]) * detInv;
	float m12 = (_m[12] * _m[6] * _m[3] - _m[4] * _m[14] * _m[3] - _m[12] * _m[2] * _m[7] + _m[0] * _m[14] * _m[7] + _m[4] * _m[2] * _m[15] - _m[0] * _m[6] * _m[15]) * detInv;
	float m13 = (_m[4] * _m[10] * _m[3] - _m[8] * _m[6] * _m[3] + _m[8] * _m[2] * _m[7] - _m[0] * _m[10] * _m[7] - _m[4] * _m[2] * _m[11] + _m[0] * _m[6] * _m[11]) * detInv;
	float m20 = t02 * detInv;
	float m21 = (_m[12] * _m[9] * _m[3] - _m[8] * _m[13] * _m[3] - _m[12] * _m[1] * _m[11] + _m[0] * _m[13] * _m[11] + _m[8] * _m[1] * _m[15] - _m[0] * _m[9] * _m[15]) * detInv;
	float m22 = (_m[4] * _m[13] * _m[3] - _m[12] * _m[5] * _m[3] + _m[12] * _m[1] * _m[7] - _m[0] * _m[13] * _m[7] - _m[4] * _m[1] * _m[15] + _m[0] * _m[5] * _m[15]) * detInv;
	float m23 = (_m[8] * _m[5] * _m[3] - _m[4] * _m[9] * _m[3] - _m[8] * _m[1] * _m[7] + _m[0] * _m[9] * _m[7] + _m[4] * _m[1] * _m[11] - _m[0] * _m[5] * _m[11]) * detInv;
	float m30 = t03 * detInv;
	float m31 = (_m[8] * _m[13] * _m[2] - _m[12] * _m[9] * _m[2] + _m[12] * _m[1] * _m[10] - _m[0] * _m[13] * _m[10] - _m[8] * _m[1] * _m[14] + _m[0] * _m[9] * _m[14]) * detInv;
	float m32 = (_m[12] * _m[5] * _m[2] - _m[4] * _m[13] * _m[2] - _m[12] * _m[1] * _m[6] + _m[0] * _m[13] * _m[6] + _m[4] * _m[1] * _m[14] - _m[0] * _m[5] * _m[14]) * detInv;
	float m33 = (_m[4] * _m[9] * _m[2] - _m[8] * _m[5] * _m[2] + _m[8] * _m[1] * _m[6] - _m[0] * _m[9] * _m[6] - _m[4] * _m[1] * _m[10] + _m[0] * _m[5] * _m[10]) * detInv;
 
	result->setValues(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33);
	result->setInverse(new Matrix4(*this));

	_inverse = new Matrix4(*result);
	return this;
}

Vector3 Matrix4::transformPosition(const Vector3& p, Vector3* result) const
{
	float x = _m[0] * p.x + _m[1] * p.y + _m[2] * p.z + _m[3];
	float y = _m[4] * p.x + _m[5] * p.y + _m[6] * p.z + _m[7];
	float z = _m[8] * p.x + _m[9] * p.y + _m[10] * p.z + _m[11];
	float w = _m[12] * p.x + _m[13] * p.y + _m[14] * p.z + _m[15];

	Vector3& r = getOrDefault(result, Vector3());

	if (equals(w, 0., 1.0e-6))
	{
		r.x = 0.;
		r.y = 0.;
		r.z = 0.;
	}
	else
	{
		r.x = x / w;
		r.y = y / w;
		r.z = z / w;
	}

	return r;
}

Vector3 Matrix4::transformDirection(const Vector3& v, Vector3* result) const
{
	float x = _m[0] * v.x + _m[1] * v.y + _m[2] * v.z;
	float y = _m[4] * v.x + _m[5] * v.y + _m[6] * v.z;
	float z = _m[8] * v.x + _m[9] * v.y + _m[10] * v.z;
	float w = _m[12] * v.x + _m[13] * v.y + _m[14] * v.z;

	Vector3& r = getOrDefault(result, Vector3());

	if (equals(w, 0., 1.0e-6))
	{
		r.x = 0.;
		r.y = 0.;
		r.z = 0.;
	}
	else
	{
		r.x = x / w;
		r.y = y / w;
		r.z = z / w;
	}

	return r;
}

Matrix4 Matrix4::multiply(const Matrix4& m, Matrix4* result) const
{
	float data[16] = { 0 };

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; ++c)
		{
			data[r * 4 + c] = _m[r * 4] * m._m[c] + _m[r * 5] * m._m[4 + c] + _m[r * 6] * m._m[8 + c] + _m[r * 7] * m._m[12 + c];
		}
	}

	Matrix4& r = getOrDefault(result, Matrix4());
	r.setValues(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
		data[10], data[11], data[12], data[13], data[14], data[15]);

	return r;
}

float Matrix4::operator[](int i) const
{
	if (i < 0 || i > 15)
	{
		std::string errMsg = "Index " + std::to_string(i) + " out of range for 4x4 matrix";
		throw IllegalArgumentException(errMsg);
	}

	return _m[i];
}

Matrix4& Matrix4::operator=(const Matrix4& other)
{
	setValues(other);
	_didComputeInverse = false;
	_inverse = safeDelete(_inverse);
	if (other._inverse != nullptr)
	{
		setInverse(new Matrix4(*other._inverse));
	}

	return *this;
}

bool Matrix4::operator==(const Matrix4& other) const
{
	return equalTo(other);
}

bool Matrix4::operator!=(const Matrix4& other) const
{
	return !equalTo(other);
}

void Matrix4::setValues(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	clearInverse();

	_m[0] = m00;	_m[1] = m01;	_m[2] = m02;	_m[3] = m03;
	_m[4] = m10;	_m[5] = m11;	_m[6] = m12;	_m[7] = m13;
	_m[8] = m20;	_m[9] = m21;	_m[10] = m22;	_m[11] = m23;
	_m[12] = m30;	_m[13] = m31;	_m[14] = m32;	_m[15] = m33;
}

void Matrix4::setValues(const Matrix4& other)
{
	clearInverse();

	for (int i = 0; i < 15; i++)
	{
		_m[i] = other._m[i];
	}
}

void Matrix4::clearInverse()
{
	_didComputeInverse = false;
	_inverse = safeDelete(_inverse);
}

void Matrix4::setInverse(Matrix4* inv)
{
	_didComputeInverse = true;
	safeDelete(_inverse);
	_inverse = inv;
}