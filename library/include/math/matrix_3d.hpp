/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef MATH_MATRIX_3d
#define MATH_MATRIX_3d

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <iostream>
#include <cmath>
#include <array>
#include <utility>
#include "math/vector_3d.hpp"
#include "math/matrix_4d.hpp"

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace math {

/////////////////////////////////////////////////////////////////////// STRUCT
  struct matrix_3d {

  public:
/////////////////////////////////////////////////////////////////////// ATTRIBUTES
    const std::array<float, 9> values;
/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
    static const matrix_3d kZero;
    static const matrix_3d kIdentity;

/////////////////////////////////////////////////////////////////////// CONSTRUCTORS
    matrix_3d(const float& xx, const float& xy, const float& xz,
      const float& yx, const float& yy, const float& yz,
      const float& zx, const float& zy, const float& zz);
    matrix_3d(const int& xx, const int& xy, const int& xz,
      const int& yx, const int& yy, const int& yz,
      const int& zx, const int& zy, const int& zz);
    matrix_3d(const vector_3d& x, const vector_3d& y, const vector_3d& z);
    matrix_3d(const std::array<float, 9>& values);

/////////////////////////////////////////////////////////////////////// OPERATORS
    const bool operator==(const matrix_3d& other_matrix) const;
    const bool operator!=(const matrix_3d& other_matrix) const;
    const matrix_3d operator+ (const matrix_3d& other_matrix) const;
    const matrix_3d operator- (const matrix_3d& other_matrix) const;
    const matrix_3d operator* (const float& scalar) const;
    const matrix_3d operator* (const int& scalar) const;
    const vector_3d operator* (const vector_3d& vector) const;
    const matrix_3d operator* (const matrix_3d& other_matrix) const;
    const matrix_3d operator/ (const float& scalar) const;
    const matrix_3d operator/ (const int& scalar) const;
    const matrix_3d operator/ (const matrix_3d& other_matrix) const;
    const vector_3d operator[](const int& index) const;

/////////////////////////////////////////////////////////////////////// PUBLIC METHODS
    const matrix_3d Transpose() const;
    const float Determinant() const;
    const matrix_3d Inverse() const;
    const std::array<float, 9> ColumnMajor() const;

/////////////////////////////////////////////////////////////////////// STATIC METHODS
    static const matrix_3d Scaling(const float& x, const float& y, const float& z);
    static const matrix_3d ScalingX(const float& x);
    static const matrix_3d ScalingY(const float& y);
    static const matrix_3d ScalingZ(const float& z);
    static const matrix_4d Translation(const float& x, const float& y, const float& z);
    static const matrix_4d TranslationX(const float& x);
    static const matrix_4d TranslationY(const float& y);
    static const matrix_4d TranslationZ(const float& z);
    static const matrix_3d Rotation(const float& anglex, const float& angley, const float& anglez);
    static const matrix_3d RotationX(const float& angle);
    static const matrix_3d RotationY(const float& angle);
    static const matrix_3d RotationZ(const float& angle);
    static const matrix_3d Dual(const float& x, const float& y, const float& z);
    static const matrix_3d Dual(const vector_3d& vector);

/////////////////////////////////////////////////////////////////////// FRIENDLY METHODS
    friend const matrix_3d operator* (const float& scalar, const matrix_3d& matrix);
    friend const matrix_3d operator* (const int& scalar, const matrix_3d& matrix);
    friend const vector_3d operator* (const vector_3d& vector, const matrix_3d& matrix);
    friend std::ostream& operator<< (std::ostream& os, const matrix_3d& matrix);

  };

/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
  const matrix_3d matrix_3d::kZero(0, 0, 0,
    0, 0, 0,
    0, 0, 0);
  const matrix_3d matrix_3d::kIdentity(1, 0, 0,
    0, 1, 0,
    0, 0, 1);

}

#endif