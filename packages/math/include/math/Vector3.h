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
     * The zero vector
     */
    static const Vector3 ZERO;

    /**
     * Unit vector in the x direction
     */
    static const Vector3 XHAT;

    /**
     * Unit vector in the negative x direction
     */
    static const Vector3 MINUS_XHAT;

    /**
     * Unit vector in the y direction
     */
    static const Vector3 YHAT;

    /**
     * Unit vector in the negative y direction
     */
    static const Vector3 MINUS_YHAT;

    /**
     * Unit vector in the z direction
     */
    static const Vector3 ZHAT;

    /**
     * Unit vector in the negative z direction
     */
    static const Vector3 MINUS_ZHAT;

	/**
	 * X coordinate
	 */
	float x;

	/**
	 * Y coordinate
	 */
	float y;

	/**
	 * Z coordinate
	 */
	float z;

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
	Vector3(float x, float y, float z);

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
    bool isZero(float tol = 0.0) const;

    /**
     * Determines if this vector is equal to another vector (within tolerance).
     * @param other The vector to compare to
     * @param tol Tolerance value to use when comparing. Defaults to 0.
     * @return True if the two vectors are equal. Returns false otherwise.
     */
    bool equalTo(const Vector3& other, float tol = 0.0) const;

    /**
     * @return The magnitude of this vector
     */
    float getMagnitude() const;

    /**
     * Computes the dot product of this vector with another vector
     * @param other The other vector
     * @return The dot product of this vector and the given vector
     */
    float dot(const Vector3& other) const;

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
     * Uniformly scales this vector by the given value, returning the result as a new vector
     * @param value The value to scale by
     * @param result (Optional) Pointer to a vector in which to store the results
     * @return The resulting vector
     */
    Vector3 scale(float value, Vector3* result = nullptr) const;

    /**
     * Performs a non-uniform scaling of this vector using the given values, returning the result as a new vector
     * @param xScalar Scaling in the x direction
     * @param yScalar Scaling in the y direction
     * @param zScalar Scaling in the z direction
     * @param result (Optional) Pointer to a vector in which to store the results
     * @return The resulting vector
     */
    Vector3 scale(float xScalar, float yScalar, float zScalar, Vector3* result = nullptr) const;

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

    /**
     * Operator that compares this vector to another vector to determine if they are not equal.
     * @param other The vector to compare to
     * @return True if the two vectors are not equal. Returns false otherwise.
     */
    bool operator!=(const Vector3& other) const;
};