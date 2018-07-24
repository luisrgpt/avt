/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef MATH_MATRIX_4d
#define MATH_MATRIX_4d

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <iostream>
#include <cmath>
#include <array>
#include <utility>
#include "math/vector_3d.hpp"
#include "math/matrix_5d.hpp"
#include "math/quaternion.hpp"

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace math {

/////////////////////////////////////////////////////////////////////// STRUCT
  struct matrix_4d {

  public:
/////////////////////////////////////////////////////////////////////// ATTRIBUTES
    std::array<float, 16> values;
/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
    static const matrix_4d kZero;
    static const matrix_4d kIdentity;

/////////////////////////////////////////////////////////////////////// CONSTRUCTORS
    matrix_4d(const float& xx, const float& xy, const float& xz, const float& xw,
      const float& yx, const float& yy, const float& yz, const float& yw,
      const float& zx, const float& zy, const float& zz, const float& zw,
      const float& wx, const float& wy, const float& wz, const float& ww);
    matrix_4d(const int& xx, const int& xy, const int& xz, const int& xw,
      const int& yx, const int& yy, const int& yz, const int& yw,
      const int& zx, const int& zy, const int& zz, const int& zw,
      const int& wx, const int& wy, const int& wz, const int& ww);
    matrix_4d(const vector_4d& x, const vector_4d& y, const vector_4d& z, const vector_4d& w);
    matrix_4d(const std::array<float, 16>& values);
    matrix_4d(const quaternion& values);

/////////////////////////////////////////////////////////////////////// OPERATORS
    const bool operator==(const matrix_4d& other_matrix) const;
    const bool operator!=(const matrix_4d& other_matrix) const;
    const matrix_4d operator+ (const matrix_4d& other_matrix) const;
    const matrix_4d operator- (const matrix_4d& other_matrix) const;
    const matrix_4d operator* (const float& scalar) const;
    const matrix_4d operator* (const int& scalar) const;
    const vector_4d operator* (const vector_4d& vector) const;
    const matrix_4d operator* (const matrix_4d& other_matrix) const;
    const matrix_4d operator/ (const float& scalar) const;
    const matrix_4d operator/ (const int& scalar) const;
    const vector_4d operator[](const int& index) const;

/////////////////////////////////////////////////////////////////////// PUBLIC METHODS
    const void Clean();
    const matrix_4d Transpose() const;
    const float Determinant() const;
    const std::array<float, 16> ColumnMajor() const;

/////////////////////////////////////////////////////////////////////// STATIC METHODS
    static const matrix_4d Scaling(const float& x, const float& y, const float& z, const float& w);
    static const matrix_4d ScalingX(const float& x);
    static const matrix_4d ScalingY(const float& y);
    static const matrix_4d ScalingZ(const float& z);
    static const matrix_4d ScalingW(const float& w);
    static const matrix_5d Translation(const float& x, const float& y, const float& z, const float& w);
    static const matrix_5d TranslationX(const float& x);
    static const matrix_5d TranslationY(const float& y);
    static const matrix_5d TranslationZ(const float& z);
    static const matrix_5d TranslationW(const float& w);
    static const matrix_4d Rotation(const float& anglex, const float& angley, const float& anglez);
    static const matrix_4d RotationX(const float& angle);
    static const matrix_4d RotationY(const float& angle);
    static const matrix_4d RotationZ(const float& angle);
    static const matrix_4d View(const vector_3d& eye, const vector_3d& center, const vector_3d& up);
    static const matrix_4d OrthographicProjection(const float& left, const float& right, const float& bottom, const float& top, const float& near_, const float& far_);
    static const matrix_4d PerspectiveProjection(const float& fovy, const float& aspect, const float& nearz, const float& farz);
    static const matrix_4d Frustum(const float& left, const float& right, const float& bottom, const float& top, const float& near_, const float& far_);

/////////////////////////////////////////////////////////////////////// FRIENDLY METHODS
    friend const matrix_4d operator* (const float& scalar, const matrix_4d& matrix);
    friend const matrix_4d operator* (const int& scalar, const matrix_4d& matrix);
    friend const vector_4d operator* (const vector_4d& vector, const matrix_4d& matrix);
    friend std::ostream& operator<< (std::ostream& os, const matrix_4d& matrix);

  };

/////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
  const matrix_4d matrix_4d::kZero(0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0);
  const matrix_4d matrix_4d::kIdentity(1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1);

}

#endif