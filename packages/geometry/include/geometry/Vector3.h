#pragma once

class Vector2;

/**
 * A three-dimensional vector.
 * @author Nathaniel Rex
 */
class Vector3
{
public:
	
	/**
	 * X coordinate
	 */
	double x;

	/**
	 * Y coordinate
	 */
	double y;

	/**
	 * Z coordinate
	 */
	double z;

	/**
	 * Constructs a vector where all coordinate values are zero
	 */
	Vector3();

	/**
	 * Constructs a vector from a set of values
	 * @param x X coordinate value
     * @param y Y coordinate value
	 * @param z Z coordinate value
	 */
	Vector3(double x, double y, double z);

	/**
	 * Constructs a vector using the x and y coordinates from a two-dimensional vector
	 * @param other The vector to copy from
	 */
	Vector3(const Vector2& other);

	/**
	 * Constructs a vector from another three-dimensional vector
	 * @param other The vector to copy from
	 */
	Vector3(const Vector3& other);

    /**
     * Determines if this vector is the zero vector
     * @param tol Tolerance to use when comparing. Defaults to 0.
     * @return True if this vector is the zero vector. Returns false otherwise.
     */
    bool isZero(double tol = 0.0) const;

    /**
     * Determines if this vector is equal to another vector (within tolerance).
     * @param other The vector to compare to
     * @param tol Tolerance value to use when comparing. Defaults to 0.
     * @return True if the two vectors are equal. Returns false otherwise.
     */
    bool equalTo(const Vector3& other, double tol = 0.0) const;

    /**
     * @return The magnitude of this vector
     */
    double getMagnitude() const;

    /**
     * Computes the dot product of this vector with another vector
     * @param other The other vector
     * @return The dot product of this vector and the given vector
     */
    double dot(const Vector3& other) const;

    /**
     * Computes the cross product of this vector with another vector.
     * @param other The other vector
     * @param result (Optional) Pointer to a vector in which to store the result
     * @return The cross product of this vector and the given vector.
     */
    Vector3 cross(const Vector3& other, Vector3* result = nullptr) const;

    /**
     * Normalizes this vector to have a magnitude of 1. If this vector is the zero vector, this method has no effect.
     * @param result (Optional) Pointer to a vector in which to store the results
     * @return The normalized vector
     */
    Vector3 normalize(Vector3* result = nullptr) const;

    /**
     * Scales this vector by the given value, returning the result as a new vector
     * @param value The value to scale by
     * @param result (Optional) Pointer to a vector in which to store the results
     * @return The resulting vector
     */
    Vector3 scale(double value, Vector3* result = nullptr) const;

    /**
     * Subtracts another vector from this vector.
     * @param other The vector to subtract by
     * @param result (Optional) Pointer to a vector in which to store the results
     * @return The new vector obtained by subtracting the given vector from this vector
     */
    Vector3 minus(const Vector3& other, Vector3* result = nullptr) const;

    /**
     * Operator that compares this vector to another vector to determine if they are equal.
     * @param other The vector to compare to
     * @return True if the two vectors are equal. Returns false otherwise.
     */
    bool operator==(const Vector3& other) const;
};