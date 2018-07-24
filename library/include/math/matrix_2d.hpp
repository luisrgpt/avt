/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef MATH_MATRIX_2d
#define MATH_MATRIX_2d

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <iostream>
#include <cmath>
#include <array>
#include <utility>
#include "math/vector_2d.hpp"
#include "math/matrix_3d.hpp"

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace math {

/////////////////////////////////////////////////////////////////////// STRUCT
  struct matrix_2d {

  public:
/////////////////////////////////////////////////////////////////////// ATTRIBUTES
    const std::array<float, 4> values;
/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
    static const matrix_2d kZero;
    static const matrix_2d kIdentity;

/////////////////////////////////////////////////////////////////////// CONSTRUCTORS
    matrix_2d(const float& xx, const float& xy,
      const float& yx, const float& yy);
    matrix_2d(const int& xx, const int& xy,
      const int& yx, const int& yy);
    matrix_2d(const vector_2d& x, const vector_2d& y);
    matrix_2d(const std::array<float, 4>& values);

/////////////////////////////////////////////////////////////////////// OPERATORS
    const bool operator==(const matrix_2d& other_matrix) const;
    const bool operator!=(const matrix_2d& other_matrix) const;
    const matrix_2d operator+ (const matrix_2d& other_matrix) const;
    const matrix_2d operator- (const matrix_2d& other_matrix) const;
    const matrix_2d operator* (const float& scalar) const;
    const matrix_2d operator* (const int& scalar) const;
    const vector_2d operator* (const vector_2d& vector) const;
    const matrix_2d operator* (const matrix_2d& other_matrix) const;
    const matrix_2d operator/ (const float& scalar) const;
    const matrix_2d operator/ (const int& scalar) const;
    const matrix_2d operator/ (const matrix_2d& other_matrix) const;
    const vector_2d operator[](const int& index) const;

/////////////////////////////////////////////////////////////////////// PUBLIC METHODS
    const matrix_2d Transpose() const;
    const float Determinant() const;
    const matrix_2d Inverse() const;
    const std::array<float, 4> ColumnMajor() const;

/////////////////////////////////////////////////////////////////////// STATIC METHODS
    static const matrix_2d Scaling(const float& x, const float& y);
    static const matrix_2d ScalingX(const float& x);
    static const matrix_2d ScalingY(const float& y);
    static const matrix_3d Translation(const float& x, const float& y);
    static const matrix_3d TranslationX(const float& x);
    static const matrix_3d TranslationY(const float& y);
    static const matrix_2d Rotation(const float& anglex);

/////////////////////////////////////////////////////////////////////// FRIENDLY METHODS
    friend const matrix_2d operator* (const float& scalar, const matrix_2d& matrix);
    friend const matrix_2d operator* (const int& scalar, const matrix_2d& matrix);
    friend const vector_2d operator* (const vector_2d& vector, const matrix_2d& matrix);
    friend std::ostream& operator<< (std::ostream& os, const matrix_2d& matrix);

  };

/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
  const matrix_2d matrix_2d::kZero(0, 0,
    0, 0);
  const matrix_2d matrix_2d::kIdentity(1, 0,
    0, 1);

}

#endif