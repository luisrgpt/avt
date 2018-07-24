/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef MATH_VECTOR_4d
#define MATH_VECTOR_4d

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <iostream>
#include <cmath>
#include <array>

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace math {

/////////////////////////////////////////////////////////////////////// STRUCT
  struct vector_4d {
/////////////////////////////////////////////////////////////////////// ATTRIBUTES
    float x;
    float y;
    float z;
    float w;

/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
    static const vector_4d kZero;

/////////////////////////////////////////////////////////////////////// CONSTRUCTORS
    vector_4d(const float& x, const float& y, const float& z, const float& w);
    vector_4d(const int& x, const int& y, const int& z, const int& w);
    vector_4d(const std::array<float, 4>& values);

/////////////////////////////////////////////////////////////////////// OPERATORS
    const bool operator==(const vector_4d& other_vector) const;
    const bool operator!=(const vector_4d& other_vector) const;
    const vector_4d operator+ (const vector_4d& other_vector) const;
    const vector_4d operator- (const vector_4d& other_vector) const;
    const vector_4d operator* (const float& scalar) const;
    const vector_4d operator* (const int& scalar) const;
    const vector_4d operator/ (const float& scalar) const;
    const vector_4d operator/ (const int& scalar) const;
    const float operator[](const int& index) const;

/////////////////////////////////////////////////////////////////////// PUBLIC METHODS
    const float Quadrance(const vector_4d& other_vector) const;
    const float Norm() const;
    const vector_4d Normalize() const;
    const float DotProduct(const vector_4d& other_vector) const;

/////////////////////////////////////////////////////////////////////// FRIENDLY METHODS
    friend const vector_4d operator* (const float& scalar, const vector_4d& vector);
    friend const vector_4d operator* (const int& scalar, const vector_4d& vector);
    friend std::ostream& operator<< (std::ostream& os, const vector_4d& vector);

  };

/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
  const vector_4d vector_4d::kZero(0, 0, 0, 0);

}

#endif
