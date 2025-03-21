#pragma once

class Vector3;
class Matrix3;

/**
 * A 4x4 matrix, whose values are indexable as a row-major order array.
 * @author Nathaniel Rex
 */
class Matrix4 {
public:

	/**
	 * Identity matrix
	 */
	static const Matrix4 IDENTITY;

	/**
	 * Construct the identity matrix
	 */
	Matrix4();

	/**
	 * Constructs a matrix from values defined in row-major order
	 * @param m00 Row 0, column 0 value
	 * @param m01 Row 0, column 1 value
	 * @param m02 Row 0, column 2 value
	 * @param m03 Row 0, column 3 value
	 * @param m10 Row 1, column 0 value
	 * @param m11 Row 1, column 1 value
	 * @param m12 Row 1, column 2 value
	 * @param m13 Row 1, column 3 value
	 * @param m20 Row 2, column 0 value
	 * @param m21 Row 2, column 1 value
	 * @param m22 Row 2, column 2 value
	 * @param m23 Row 2, column 3 value
	 * @param m30 Row 3, column 0 value
	 * @param m31 Row 3, column 1 value
	 * @param m32 Row 3, column 2 value
	 * @param m33 Row 3, column 3 value
	 */
	Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);

	/**
	 * Constructs a matrix from another 4x4 matrix
	 * @param other Matrix to copy from
	 */
	Matrix4(const Matrix4& other);

	/**
	 * Destructor
	 */
	~Matrix4();

	/**
	 * Constructs a transformation matrix representing a translation
	 * @param v Vector representing the translation
	 * @param (Optional) Matrix in which to store the results
	 * @return The transformation matrix
	 */
	static Matrix4 fromTranslation(const Vector3& v, Matrix4* result = nullptr);

	/**
	 * Constructs a transformation matrix representing a rotation
	 * @param rot 3x3 rotation matrix
	 * @param (Optional) Matrix in which to store the results
	 * @return The transformation matrix
	 */
	static Matrix4 fromRotation(const Matrix3& rot, Matrix4* result = nullptr);

	/**
	 * Constructs a transformation matrix representing a uniform scaling
	 * @param scalar Value representing the scale magnitude
	 * @param (Optional) Matrix in which to store the results
	 * @return The transformation matrix
	 */
	static Matrix4 fromScaling(float scalar, Matrix4* result = nullptr);

	/**
	 * Constructs a transformation matrix representing a scaling
	 * @param x Scaling magnitude in the x direction
	 * @param y Scaling magnitude in the y direction
	 * @param z Scaling magnitude in the z direction
	 * @param (Optional) Matrix in which to store the results
	 * @return The transformation matrix
	 */
	static Matrix4 fromScaling(float x, float y, float z, Matrix4* result = nullptr);

	/**
	 * @return True if this matrix is the identity matrix. Returns false otherwise.
	 */
	bool isIdentity() const;

	/**
	 * Tests whether this matrix is equal to another matrix, within tolerance
	 * @param other Matrix to evaluate against
	 * @param (Optional) Tolerance value. Defaults to 0.
	 * @return True if the two matrices are equal (within tolerance). False otherwise.
	 */
	bool equalTo(const Matrix4& other, float tol = 0.) const;

	/**
	 * Computes the transpose of this matrix
	 * @param result (Optional) Matrix in which to store the result
	 * @return The transpose matrix
	 */
	Matrix4 transpose(Matrix4* result = nullptr) const;

	/**
	 * Computes the inverse of this matrix
	 * @param Result matrix in which to store the result. This matrix will be unchanged if
	 * the inverse does not exist.
	 * @return A boolean value indicating whether or not the inverse exists.
	 */
	bool inverse(Matrix4* result);

	/**
	 * Transforms the given 3-dimensional position vector using this matrix. This will result in a new
	 * position after having applied scaling, rotation, and translation.
	 * @param p Position vector to transform
	 * @param result (Optional) Vector in which to store the result.
	 * @return The resulting position vector
	 */
	Vector3 transformPosition(const Vector3& p, Vector3* result = nullptr) const;

	/**
	 * Transforms the given 3-dimensional direction vector using this matrix. This will result in a new
	 * direction after having applied scaling and rotation.
	 * @param v Direction vector to transform
	 * @param result (Optional) Vector in which to store the result
	 * @return The resulting direction vector
	 */
	Vector3 transformDirection(const Vector3& v, Vector3* result = nullptr) const;

	/**
	 * Compute the product of this matrix and another matrix
	 * @param m The other matrix
	 * @param result (Optional) Matrix in which to store the result
	 * @return The product of this matrix and the other matrix
	 */
	Matrix4 multiply(const Matrix4& m, Matrix4* result = nullptr) const;

	/**
	 * Access a value of this matrix at the given index. Values of the matrix are stored and accessed in
	 * row-major order.
	 * @param i The index to access
	 * @return The value at the requested index
	 */
	float operator[](int i) const;

	/**
	 * Operator that reassigns this matrix to a new value
	 * @param other Matrix to assign to
	 * @return The matrix instance after reassignment
	 */
	Matrix4& operator=(const Matrix4& other);

	/**
	 * Operator that compares this matrix to another matrix to determine if they are equal.
	 * @param other The matrix to compare to
	 * @return True if the two matrices are equal in value. Returns false otherwise.
	 */
	bool operator==(const Matrix4& other) const;

	/**
	 * Operator that compares this matrix to another matrix to determine if they are not equal.
	 * @param other The matrix to compare to
	 * @return True if the two matrices are equal in value. Returns false otherwise.
	 */
	bool operator!=(const Matrix4& other) const;

private:

	/**
	 * The values of this matrix
	 */
	float _m[16];

	/**
	 * Boolean value indicating whether the inverse of this matrix has been computed.
	 */
	bool _didComputeInverse = false;

	/**
	 * Cached inverse of this matrix. Will be null until the inverse is first computed, after which the value may either be
	 * null or a valid matrix.
	 */
	Matrix4* _inverse = nullptr;

	/**
	 * Sets the values of this matrix, specified in row-major order
	 * @param m00 Row 0, column 0 value
	 * @param m01 Row 0, column 1 value
	 * @param m02 Row 0, column 2 value
	 * @param m03 Row 0, column 3 value
	 * @param m10 Row 1, column 0 value
	 * @param m11 Row 1, column 1 value
	 * @param m12 Row 1, column 2 value
	 * @param m13 Row 1, column 3 value
	 * @param m20 Row 2, column 0 value
	 * @param m21 Row 2, column 1 value
	 * @param m22 Row 2, column 2 value
	 * @param m23 Row 2, column 3 value
	 * @param m30 Row 3, column 0 value
	 * @param m31 Row 3, column 1 value
	 * @param m32 Row 3, column 2 value
	 * @param m33 Row 3, column 3 value
	 */
	void setValues(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);

	/**
	 * Set the values of this matrix from another matrix
	 * @param other The matrix to copy from
	 */
	void setValues(const Matrix4& other);

	/**
	 * Clears the cached inverse of this matrix
	 */
	void clearInverse();

	/**
	 * Sets the cached inverse of this matrix
	 * @param inv Inverse matrix
	 */
	void setInverse(Matrix4* inv);
};