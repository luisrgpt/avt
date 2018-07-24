/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef MATH_VECTOR_3d
#define MATH_VECTOR_3d

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <iostream>
#include <cmath>
#include <array>

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace math {

/////////////////////////////////////////////////////////////////////// STRUCT
  struct vector_3d {
/////////////////////////////////////////////////////////////////////// ATTRIBUTES
    float x;
    float y;
    float z;

/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
    static const vector_3d kZero;

/////////////////////////////////////////////////////////////////////// CONSTRUCTORS
    vector_3d() = default;
    vector_3d(const float& x, const float& y, const float& z);
    vector_3d(const int& x, const int& y, const int& z);
    vector_3d(const std::array<float, 3>& values);

/////////////////////////////////////////////////////////////////////// OPERATORS
    const bool operator==(const vector_3d& other_vector) const;
    const bool operator!=(const vector_3d& other_vector) const;

    const vector_3d& operator+= (const vector_3d& other_vector);
    const vector_3d& operator-= (const vector_3d& other_vector);
    const vector_3d& operator*= (const float& scalar);
    const vector_3d& operator*= (const int& scalar);
    const vector_3d& operator/= (const float& scalar);
    const vector_3d& operator/= (const int& scalar);

    const vector_3d operator+ (const vector_3d& other_vector) const;
    const vector_3d operator- (const vector_3d& other_vector) const;
    const vector_3d operator* (const float& scalar) const;
    const vector_3d operator* (const int& scalar) const;
    const vector_3d operator/ (const float& scalar) const;
    const vector_3d operator/ (const int& scalar) const;
    const float operator[](const int& index) const;

/////////////////////////////////////////////////////////////////////// PUBLIC METHODS
    const float Quadrance(const vector_3d& other_vector) const;
    const float Norm() const;
    const vector_3d Normalize() const;
    const float DotProduct(const vector_3d& other_vector) const;
    const vector_3d CrossProduct(const vector_3d& other_vector) const;
    const vector_3d Rotate(const vector_3d& other_vector, const float& angle) const;

/////////////////////////////////////////////////////////////////////// FRIENDLY METHODS
    friend const vector_3d operator* (const float& scalar, const vector_3d& Vector);
    friend const vector_3d operator* (const int& scalar, const vector_3d& Vector);
    friend std::ostream& operator<< (std::ostream& os, const vector_3d& Vector);

  };

/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
  const vector_3d vector_3d::kZero(0, 0, 0);

}

#endif