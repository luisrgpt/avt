#include "math/matrix_5d.hpp"

using namespace math;

matrix_5d::matrix_5d(const float& xx, const float& xy, const float& xz, const float& xw, const float& x_a,
                  const float& yx, const float& yy, const float& yz, const float& yw, const float& y_a,
                  const float& zx, const float& zy, const float& zz, const float& zw, const float& z_a,
                  const float& wx, const float& wy, const float& wz, const float& ww, const float& w_a,
                  const float& ax, const float& ay, const float& az, const float& aw, const float& a_a)
  : values{xx, xy, xz, xw, x_a,
           yx, yy, yz, yw, y_a,
           zx, zy, zz, zw, z_a,
           wx, wy, wz, ww, w_a,
           ax, ay, az, aw, a_a} {}