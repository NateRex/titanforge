#pragma once

class Vector3;

/**
 * A two-dimensional vector.
 * @author Nathaniel Rex
 */
class Vector2
{
public:

    /**
     * Zero vector
     */
    static const Vector2 ZERO;

    /**
     * X coordinate
     */
    float x;

    /**
     * Y coordinate
     */
    float y;

    /**
     * Constructs a vector where all coordinate values are zero
     */
    Vector2();

    /**
     * Constructs a vector from x and y values
     * @param x X coordinate value
     * @param y Y coordinate value
     */
    Vector2(float x, float y);

    /**
     * Constructs a vector from another vector
     * @param other The vector to copy
     */
    Vector2(const Vector2& other);

    /**
     * Constructs a vector from the x and y components of a three-dimensional vector
     * @param other The vector to copy from
     */
    Vector2(const Vector3& other);

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
    bool equalTo(const Vector2& other, float tol = 0.0) const;

    /**
     * @return The magnitude of this vector
     */
    float getMagnitude() const;

    /**
     * Computes the dot product of this vector with another vector
     * @param other The other vector
     * @return The dot product of this vector and the given vector
     */
    float dot(const Vector2& other) const;

    /**
     * Computes the cross product of this vector with another vector.
     * @param other The other vector
     * @return The cross product of this vector and the given vector.
     */
    float cross(const Vector2& other) const;

    /**
     * Computes the unsigned angle between this vector and another vector
     * @param other The other vector
     * @return The unsigned angle between the two vectors (in radians)
     */
    float angleBetween(const Vector2& other) const;

    /**
     * Normalizes this vector to have a magnitude of 1. If this vector is the zero vector, this method has no effect.
     * @param result (Optional) Pointer to a vector in which to store the results
     * @return The normalized vector
     */
    Vector2 normalize(Vector2* result = nullptr) const;

    /**
     * Uniformly scales this vector by the given value, returning the result as a new vector
     * @param value The value to scale by
     * @param result (Optional) Pointer to a vector in which to store the results
     * @return The resulting vector
     */
    Vector2 scale(float value, Vector2* result = nullptr) const;

    /**
     * Performs a non-uniform scaling of this vector using the given values, returning the result as a new vector
     * @param xScalar Scaling in the x direction
     * @param yScalar Scaling in the y direction
     * @param result (Optional) Pointer to a vector in which to store the results
     * @return The resulting vector
     */
    Vector2 scale(float xScalar, float yScalar, Vector2* result = nullptr) const;

    /**
     * Subtracts another vector from this vector.
     * @param other The vector to subtract by
     * @param result (Optional) Pointer to a vector in which to store the results
     * @return The new vector obtained by subtracting the given vector from this vector
     */
    Vector2 minus(const Vector2& other, Vector2* result = nullptr) const;

    /**
     * Operator that compares this vector to another vector to determine if they are equal.
     * @param other The vector to compare to
     * @return True if the two vectors are equal. Returns false otherwise.
     */
    bool operator==(const Vector2& other) const;

    /**
     * Operator that compares this vector to another vector to determine if they are not equal.
     * @param other The vector to compare to
     * @return True if the two vectors are not equal. Returns false otherwise.
     */
    bool operator!=(const Vector2& other) const;
};