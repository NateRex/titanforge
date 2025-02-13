#pragma once

class Vector3;

/**
 * A 3x3 matrix, whose values are indexable as a row-major order array
 * @author Nathaniel Rex
 */
class Matrix3
{
public:

	/**
	 * Constructs the identity matrix
	 */
	Matrix3();

	/**
	 * Constructs a matrix from values defined in row-major order.
	 * @param m00 Row 0, column 0 value
	 * @param m01 Row 0, column 1 value
	 * @param m02 Row 0, column 2 value
	 * @param m10 Row 1, column 0 value
	 * @param m11 Row 1, column 1 value
	 * @param m12 Row 1, column 2 value
	 * @param m20 Row 2, column 0 value
	 * @param m21 Row 2, column 1 value
	 * @param m22 Row 2, column 2 value
	 */
	Matrix3(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22);

	/**
	 * Constructs a matrix from another 3x3 matrix
	 * @param other Matrix to copy
	 */
	Matrix3(const Matrix3& other);

	/**
	 * Destructor
	 */
	~Matrix3();

	/**
	 * Constructs a matrix from three row vectors
	 * @param r0 Row 0 vector
	 * @param r1 Row 1 vector
	 * @param r2 Row 2 vector
	 * @param result (Optional) Matrix to store the result in
	 * @return The matrix containing the given row vectors
	 */
	static Matrix3 fromRows(const Vector3& r0, const Vector3& r1, const Vector3& r2, Matrix3* result = nullptr);

	/**
	 * Constructs a matrix from three column vectors
	 * @param c0 Column 0 vector
	 * @param c1 Column 1 vector
	 * @param c2 Column 2 vector
	 * @param result (Optional) Matrix to store the result in
	 * @return The matrix containing the given column vectors
	 */
	static Matrix3 fromColumns(const Vector3& c0, const Vector3& c1, const Vector3& c2, Matrix3* result = nullptr);

	/**
	 * Constructs a matrix representing a rotation about the X-axis
	 * @param radians Angle of rotation, in radians
	 * @param (Optional) Matrix in which to store the results
	 * @return The rotation matrix
	 */
	static Matrix3 fromXRotation(double radians, Matrix3* result = nullptr);

	/**
	 * Constructs a matrix representing a rotation about the Y-axis
	 * @param radians Angle of rotation, in radians
	 * @param (Optional) Matrix in which to store the results
	 * @return The rotation matrix
	 */
	static Matrix3 fromYRotation(double radians, Matrix3* result = nullptr);

	/**
	 * Constructs a matrix representing a rotation about the Z-axis
	 * @param radians Angle of rotation, in radians
	 * @param (Optional) Matrix in which to store the results
	 * @return The rotation matrix
	 */
	static Matrix3 fromZRotation(double radians, Matrix3* result = nullptr);

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
	bool equalTo(const Matrix3& other, double tol = 0.) const;

	/**
	 * Computes the transpose of this matrix
	 * @param result (Optional) Matrix in which to store the result
	 * @return The transpose matrix
	 */
	Matrix3 transpose(Matrix3* result = nullptr) const;

	/**
	 * Computes the inverse of this matrix
	 * @param Result matrix in which to store the result. This matrix will be unchanged if
	 * the inverse does not exist.
	 * @return An optional value containing the inverse, if it exists.
	 */
	bool inverse(Matrix3* result);

	/**
	 * Compute the product of this matrix and a column vector
	 * @param v The column vector
	 * @param result (Optional) Vector in which to store the result
	 * @return The product of this matrix and the column vector
	 */
	Vector3 multiply(const Vector3& v, Vector3* result = nullptr) const;

	/**
	 * Compute the product of this matrix and another matrix
	 * @param m The other matrix
	 * @param result (Optional) Matrix in which to store the result
	 * @return The product of this matrix and the other matrix
	 */
	Matrix3 multiply(const Matrix3& m, Matrix3* result = nullptr) const;

	/**
	 * Access a value of this matrix at the given index. Values of the matrix are stored and accessed in
	 * row-major order.
	 * @param i The index to access
	 * @return The value at the requested index
	 */
	double operator[](int i) const;

	/**
	 * Operator that compares this matrix to another matrix to determine if they are equal.
     * @param other The matrix to compare to
     * @return True if the two matrices are equal. Returns false otherwise.
	 */
	bool operator==(const Matrix3& other) const;

private:

	/**
	 * The values of this matrix
	 */
	double _m[9];

	/**
	 * Boolean value indicating whether the inverse of this matrix has been computed.
	 */
	bool _didComputeInverse = false;

	/**
	 * Cached inverse of this matrix. Will be null until the inverse is first computed, after which the value may either be
	 * null or a valid matrix.
	 */
	Matrix3* _inverse = nullptr;

	/**
	 * Sets the values of this matrix, specified in row-major order
	 * @param m00 Row 0, column 0 value
	 * @param m01 Row 0, column 1 value
	 * @param m02 Row 0, column 2 value
	 * @param m10 Row 1, column 0 value
	 * @param m11 Row 1, column 1 value
	 * @param m12 Row 1, column 2 value
	 * @param m20 Row 2, column 0 value
	 * @param m21 Row 2, column 1 value
	 * @param m22 Row 2, column 2 value
	 */
	void setValues(double m00, double m01, double m02, double m10, double m11, double m12, double m20, double m21, double m22);

	/**
	 * Set the values of this matrix from another matrix
	 * @param other The matrix to copy from
	 */
	void setValues(const Matrix3& other);
};

/**
 * The identity matrix
 */
const Matrix3 IDENTITY_MATRIX = Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);