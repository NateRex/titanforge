#include <math/Matrix4.h>
#include <math/Matrix3.h>
#include <math/Vector3.h>
#include <common/Utils.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <string>
#include <math.h>

const Matrix4 Matrix4::IDENTITY;

Matrix4::Matrix4()
{
	setValues(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);
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
	delete _inverse;
	_inverse = nullptr;
	_didComputeInverse = false;
}

Matrix4 Matrix4::fromTranslation(const Vector3& v, Matrix4* result)
{
	Matrix4& r = getOrDefault(result, Matrix4());
	r.setValues(
		1.f, 0.f, 0.f, v.x,
		0.f, 1.f, 0.f, v.y,
		0.f, 0.f, 1.f, v.z,
		0.f, 0.f, 0.f, 1.f);
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

Matrix4 Matrix4::fromRotation(const Vector3& axis, float radians, Matrix4* result)
{
	return Matrix4::fromRotation(Matrix3::fromRotation(axis, radians), result);
}

Matrix4 Matrix4::fromXRotation(float radians, Matrix4* result)
{
	return Matrix4::fromRotation(Matrix3::fromXRotation(radians), result);
}

Matrix4 Matrix4::fromYRotation(float radians, Matrix4* result)
{
	return Matrix4::fromRotation(Matrix3::fromYRotation(radians), result);
}

Matrix4 Matrix4::fromZRotation(float radians, Matrix4* result)
{
	return Matrix4::fromRotation(Matrix3::fromZRotation(radians), result);
}

Matrix4 Matrix4::fromScaling(float scalar, Matrix4* result)
{
	return Matrix4::fromScaling(scalar, scalar, scalar, result);
}

Matrix4 Matrix4::fromScaling(float x, float y, float z, Matrix4* result)
{
	Matrix4& r = getOrDefault(result, Matrix4());
	r.setValues(
		x, 0.f, 0.f, 0.f,
		0.f, y, 0.f, 0.f,
		0.f, 0.f, z, 0.f,
		0.f, 0.f, 0.f, 1.f);
	return r;
}

Matrix4 Matrix4::fromOrthographic(float width, float height, float near, float far, Matrix4* result)
{
	float r = width / 2.0f;
	float l = -r;
	float t = height / 2.0f;
	float b = -t;

	Matrix4& res = getOrDefault(result, Matrix4());
	res.setValues(
		2.0f / (r - l), 0.f, 0.f, -(r + l) / (r - l),
		0.f, 2.0f / (t - b), 0.f, -(t + b) / (t - b),
		0.f, 0.f, -2.0f / (far - near), -(far + near) / (far - near),
		0.f, 0.f, 0.f, 1.f);

	return res;
}

Matrix4 Matrix4::fromPerspective(float fov, float aspect, float near, float far, Matrix4* result)
{
	float tanHalfFov = tan(fov / 2.0f);

	Matrix4& r = getOrDefault(result, Matrix4());
	r.setValues(
		1.0f / (aspect * tanHalfFov), 0.f, 0.f, 0.f,
		0.f, 1.0f / tanHalfFov, 0.f, 0.f,
		0.f, 0.f, -(far + near) / (far - near), -2.f * far * near / (far - near),
		0.f, 0.f, -1.0f, 0.f);

	return r;
}

const float* Matrix4::getValues() const
{
	return _m;
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

	if (equals(det, 0, 1.0e-11))
	{
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

	Vector3& r = getOrDefault(result, Vector3());
	r.x = x;
	r.y = y;
	r.z = z;

	return r;
}

Matrix4 Matrix4::multiply(const Matrix4& m, Matrix4* result) const
{
	float m00 = _m[0] * m._m[0] + _m[1] * m._m[4] + _m[2] * m._m[8] + _m[3] * m._m[12];
	float m01 = _m[0] * m._m[1] + _m[1] * m._m[5] + _m[2] * m._m[9] + _m[3] * m._m[13];
	float m02 = _m[0] * m._m[2] + _m[1] * m._m[6] + _m[2] * m._m[10] + _m[3] * m._m[14];
	float m03 = _m[0] * m._m[3] + _m[1] * m._m[7] + _m[2] * m._m[11] + _m[3] * m._m[15];
	float m10 = _m[4] * m._m[0] + _m[5] * m._m[4] + _m[6] * m._m[8] + _m[7] * m._m[12];
	float m11 = _m[4] * m._m[1] + _m[5] * m._m[5] + _m[6] * m._m[9] + _m[7] * m._m[13];
	float m12 = _m[4] * m._m[2] + _m[5] * m._m[6] + _m[6] * m._m[10] + _m[7] * m._m[14];
	float m13 = _m[4] * m._m[3] + _m[5] * m._m[7] + _m[6] * m._m[11] + _m[7] * m._m[15];
	float m20 = _m[8] * m._m[0] + _m[9] * m._m[4] + _m[10] * m._m[8] + _m[11] * m._m[12];
	float m21 = _m[8] * m._m[1] + _m[9] * m._m[5] + _m[10] * m._m[9] + _m[11] * m._m[13];
	float m22 = _m[8] * m._m[2] + _m[9] * m._m[6] + _m[10] * m._m[10] + _m[11] * m._m[14];
	float m23 = _m[8] * m._m[3] + _m[9] * m._m[7] + _m[10] * m._m[11] + _m[11] * m._m[15];
	float m30 = _m[12] * m._m[0] + _m[13] * m._m[4] + _m[14] * m._m[8] + _m[15] * m._m[12];
	float m31 = _m[12] * m._m[1] + _m[13] * m._m[5] + _m[14] * m._m[9] + _m[15] * m._m[13];
	float m32 = _m[12] * m._m[2] + _m[13] * m._m[6] + _m[14] * m._m[10] + _m[15] * m._m[14];
	float m33 = _m[12] * m._m[3] + _m[13] * m._m[7] + _m[14] * m._m[11] + _m[15] * m._m[15];

	Matrix4& r = getOrDefault(result, Matrix4());
	r.setValues(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33);

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
	
	delete _inverse;
	_inverse = nullptr;

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

	for (int i = 0; i < 16; i++)
	{
		_m[i] = other._m[i];
	}
}

void Matrix4::clearInverse()
{
	_didComputeInverse = false;
	delete _inverse;
	_inverse = nullptr;
}

void Matrix4::setInverse(Matrix4* inv)
{
	_didComputeInverse = true;
	delete _inverse;
	_inverse = inv;
}