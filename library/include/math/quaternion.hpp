/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef MATH_QUATERNION
#define MATH_QUATERNION

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <iostream>
#include <cmath>
#include <array>

#include "vector_3d.hpp"

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace math {

  /////////////////////////////////////////////////////////////////////// STRUCT
  struct quaternion {
    /////////////////////////////////////////////////////////////////////// ATTRIBUTES
    float x;
    float y;
    float z;
    float w;

    /////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
    static const quaternion kZero;

    /////////////////////////////////////////////////////////////////////// CONSTRUCTORS
    quaternion(const float& x, const float& y, const float& z, const float& w);
    quaternion(const int& x, const int& y, const int& z, const int& w);
    quaternion(const std::array<float, 4>& values);
    quaternion(const float& theta, const vector_3d& axis);

    /////////////////////////////////////////////////////////////////////// OPERATORS
    const bool operator==(const quaternion& other_quaternion) const;
    const bool operator!=(const quaternion& other_quaternion) const;
    const quaternion operator+ (const quaternion& other_quaternion) const;
    const quaternion operator- (const quaternion& other_quaternion) const;
    const quaternion operator* (const float& scalar) const;
    const quaternion operator* (const int& scalar) const;
    const quaternion operator* (const quaternion& other_quaternion) const;
    const vector_3d operator* (const vector_3d& other_vector) const;
    const quaternion operator/ (const float& scalar) const;
    const quaternion operator/ (const int& scalar) const;
    const float operator[](const int& index) const;

    /////////////////////////////////////////////////////////////////////// PUBLIC METHODS
    const float Angle() const;
    const vector_3d Axis() const;
    const void Clean();

    const float Quadrance(const quaternion& other_quaternion) const;
    const float Quadrance() const;
    const float Norm() const;
    const quaternion Normalize() const;
    const quaternion Conjugate() const;

    const quaternion Inverse() const;

    const quaternion Lerp(const quaternion& other_quaternion, float k) const;
    const quaternion Slerp(const quaternion& other_quaternion, float k) const;

    /////////////////////////////////////////////////////////////////////// FRIENDLY METHODS
    friend const quaternion operator* (const float& scalar, const quaternion& vector);
    friend const quaternion operator* (const int& scalar, const quaternion& vector);
    friend std::ostream& operator<< (std::ostream& os, const quaternion& vector);

  };

  /////////////////////////////////////////////////////////////////////// STATIC CONSTANTS
  const quaternion quaternion::kZero(0, 0, 0, 0);

}

#endif
