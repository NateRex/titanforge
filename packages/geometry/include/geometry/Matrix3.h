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
	static Matrix3 fromXRotation(double radians);

	/**
	 * Constructs a matrix representing a rotation about the Y-axis
	 * @param radians Angle of rotation, in radians
	 * @param (Optional) Matrix in which to store the results
	 * @return The rotation matrix
	 */
	static Matrix3 fromYRotation(double radians);

	/**
	 * Constructs a matrix representing a rotation about the Z-axis
	 * @param radians Angle of rotation, in radians
	 * @param (Optional) Matrix in which to store the results
	 * @return The rotation matrix
	 */
	static Matrix3 fromZRotation(double radians);

	/**
	 * Computes the transpose of this matrix
	 * @param result (Optional) Matrix in which to store the result
	 * @return The transpose matrix
	 */
	Matrix3 transpose(Matrix3* result = nullptr) const;

	/**
	 * Computes the inverse of this matrix
	 * @param result Matrix in which to store the result
	 */
	void inverse(Matrix3* result) const;

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

private:

	/**
	 * The values of this matrix
	 */
	double _m[9];

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
};