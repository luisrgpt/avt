/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef MATH_MATRIX_5d
#define MATH_MATRIX_5d

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <iostream>
#include <cmath>
#include <array>
#include <utility>
#include "math/vector_4d.hpp"
#include "math/matrix_5d.hpp"

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace math {

/////////////////////////////////////////////////////////////////////// STRUCT
  struct matrix_5d {

  public:
/////////////////////////////////////////////////////////////////////// ATTRIBUTES
    const std::array<float, 25> values;

/////////////////////////////////////////////////////////////////////// CONSTRUCTORS
    matrix_5d(const float& xx, const float& xy, const float& xz, const float& xw, const float& x_a,
      const float& yx, const float& yy, const float& yz, const float& yw, const float& y_a,
      const float& zx, const float& zy, const float& zz, const float& zw, const float& z_a,
      const float& wx, const float& wy, const float& wz, const float& ww, const float& w_a,
      const float& ax, const float& ay, const float& az, const float& aw, const float& a_a);
  };

}

#endif