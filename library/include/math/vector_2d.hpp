/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef MATH_VECTOR_2d
#define MATH_VECTOR_2d

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <iostream>
#include <cmath>
#include <array>

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace math {

/////////////////////////////////////////////////////////////////////// STRUCT
  struct vector_2d {
/////////////////////////////////////////////////////////////////////// ATTRIBUTES
    float x;
    float y;

/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
    static const vector_2d kZero;

/////////////////////////////////////////////////////////////////////// CONSTRUCTORS
    vector_2d(const float& x, const float& y);
    vector_2d(const int& x, const int& y);
    vector_2d(const std::array<float, 2>& values);

/////////////////////////////////////////////////////////////////////// OPERATORS
    const bool operator==(const vector_2d& other_vector) const;
    const bool operator!=(const vector_2d& other_vector) const;
    const vector_2d operator+ (const vector_2d& other_vector) const;
    const vector_2d operator- (const vector_2d& other_vector) const;
    const vector_2d operator* (const float& scalar) const;
    const vector_2d operator* (const int& scalar) const;
    const vector_2d operator/ (const float& scalar) const;
    const vector_2d operator/ (const int& scalar) const;
    const float operator[](const int& index) const;

/////////////////////////////////////////////////////////////////////// PUBLIC METHODS
    const float Quadrance(const vector_2d& other_vector) const;
    const float Norm() const;
    const vector_2d Normalize() const;
    const float DotProduct(const vector_2d& other_vector) const;
    const float CrossProduct(const vector_2d& other_vector) const;

/////////////////////////////////////////////////////////////////////// FRIENDLY METHODS
    friend const vector_2d operator* (const float& scalar, const vector_2d& vector);
    friend const vector_2d operator* (const int& scalar, const vector_2d& vector);
    friend std::ostream& operator<< (std::ostream& os, const vector_2d& vector);

  };

/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
  const vector_2d vector_2d::kZero(0, 0);

}

#endif