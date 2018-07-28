#include "math/matrix.hpp"

using namespace math;

#define DEGREES_TO_RADIANS 0.01745329251994329547
const float threshold = (float)1.0e-5;

matrix_4d::matrix_4d(const float& xx, const float& xy, const float& xz, const float& xw,
                          const float& yx, const float& yy, const float& yz, const float& yw,
                          const float& zx, const float& zy, const float& zz, const float& zw,
                          const float& wx, const float& wy, const float& wz, const float& ww)
  : values{xx, xy, xz, xw,
            yx, yy, yz, yw,
            zx, zy, zz, zw,
            wx, wy, wz, ww} {}

matrix_4d::matrix_4d(const int& xx, const int& xy, const int& xz, const int& xw,
                          const int& yx, const int& yy, const int& yz, const int& yw,
                          const int& zx, const int& zy, const int& zz, const int& zw,
                          const int& wx, const int& wy, const int& wz, const int& ww)
  : values{(float)xx, (float)xy, (float)xz, (float)xw,
            (float)yx, (float)yy, (float)yz, (float)yw,
            (float)zx, (float)zy, (float)zz, (float)zw,
            (float)wx, (float)wy, (float)wz, (float)ww} {}

matrix_4d::matrix_4d(const vector_4d& x, const vector_4d& y, const vector_4d& z, const vector_4d& w)
  : values{x.x, x.y, x.z, x.w,
            y.x, y.y, y.z, y.w,
            z.x, z.y, z.z, z.w,
            w.x, w.y, w.z, w.w} {}

matrix_4d::matrix_4d(const std::array<float, 16>& values)
  : values(values) {}

matrix_4d::matrix_4d(const quaternion& q) {
  //std::cout << "before normalize" << q.x << " " << q.y << " " << q.z << " " << q.w << std::endl;
  quaternion qn = q.Normalize();
  //std::cout << "after normalize" << qn.x << " " << qn.y << " " << qn.z << " " << qn.w << std::endl;
  float xx = qn.x * qn.x;
  float xy = qn.x * qn.y;
  float xz = qn.x * qn.z;
  float xw = qn.x * qn.w;
  float yy = qn.y * qn.y;
  float yz = qn.y * qn.z;
  float yw = qn.y * qn.w;
  float zz = qn.z * qn.z;
  float zw = qn.z * qn.w;

  this->values[0] = 1.0f - 2.0f * (yy + zz);
  this->values[1] = 2.0f * (xy + zw);
  this->values[2] = 2.0f * (xz - yw);
  this->values[3] = 0.0f;

  this->values[4] = 2.0f * (xy - zw);
  this->values[5] = 1.0f - 2.0f * (xx + zz);
  this->values[6] = 2.0f * (yz + xw);
  this->values[7] = 0.0f;

  this->values[8] = 2.0f * (xz + yw);
  this->values[9] = 2.0f * (yz - xw);
  this->values[10] = 1.0f - 2.0f * (xx + yy);
  this->values[11] = 0.0f;

  this->values[12] = 0.0f;
  this->values[13] = 0.0f;
  this->values[14] = 0.0f;
  this->values[15] = 1.0f;

  this->Clean();
}

const bool matrix_4d::operator==(const matrix_4d& other_matrix) const {
  for (std::size_t index = 0; index < values.size(); index++) {
    if(values[index] != other_matrix.values[index]) {
      return false;
    }
  }
  
  return true;
}

const bool matrix_4d::operator!=(const matrix_4d& other_matrix) const {
  return !(*this == other_matrix);
}

const matrix_4d matrix_4d::operator+(const matrix_4d& other_matrix) const {
  std::array<float, 16> new_values;
  
  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] + other_matrix.values[index];
  }
  
  return matrix_4d(new_values);
}

const matrix_4d matrix_4d::operator-(const matrix_4d& other_matrix) const {
  std::array<float, 16> new_values;
  
  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] - other_matrix.values[index];
  }
  
  return matrix_4d(new_values);
}

const matrix_4d matrix_4d::operator*(const float& scalar) const {
  std::array<float, 16> new_values;

  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] * scalar;
  }
  
  return matrix_4d(new_values);
}

const matrix_4d matrix_4d::operator*(const int& scalar) const {
  return *this * (float)scalar;
}

const vector_4d matrix_4d::operator*(const vector_4d& vector) const {
  std::array<float, 4> new_values;
  
  for (std::size_t index = 0; index < 4; index++) {
    new_values[index] = values[index * 4] * vector[0] + values[index * 4 + 1] * vector[1] + values[index * 4 + 2] * vector[2] + values[index * 4 + 3] * vector[3];
  }
  
  return vector_4d(new_values);
}

matrix_4d matrix_4d::operator*(const matrix_4d& other_matrix) const {
  std::array<float, 16> new_values;
  
  for (std::size_t index = 0; index < 4; index++) {
    new_values[index]      = values[index]      * other_matrix.values[0] +
                             values[index + 4]  * other_matrix.values[1] +
                             values[index + 8]  * other_matrix.values[2] +
                             values[index + 12] * other_matrix.values[3];

    new_values[index + 4]  = values[index]      * other_matrix.values[4] +
                             values[index + 4]  * other_matrix.values[5] +
                             values[index + 8]  * other_matrix.values[6] +
                             values[index + 12] * other_matrix.values[7];

    new_values[index + 8]  = values[index]      * other_matrix.values[8] +
                             values[index + 4]  * other_matrix.values[9] +
                             values[index + 8]  * other_matrix.values[10] +
                             values[index + 12] * other_matrix.values[11];

    new_values[index + 12] = values[index]      * other_matrix.values[12] +
                             values[index + 4]  * other_matrix.values[13] +
                             values[index + 8]  * other_matrix.values[14] +
                             values[index + 12] * other_matrix.values[15];
  }

  return matrix_4d(new_values);
}

const matrix_4d matrix_4d::operator/(const float& scalar) const {
  std::array<float, 16> new_values;

  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] / scalar;
  }
  
  return matrix_4d(new_values);
}

const matrix_4d matrix_4d::operator/(const int& scalar) const {
  return *this / (float)scalar;
}

const vector_4d matrix_4d::operator[](const int& index) const {
  if(index < 0 && index >= 4) {
      throw std::out_of_range("matrix_4d::operator[](): index is out of range");
  }
  
  return vector_4d(values[index * 4], values[index * 4 + 1], values[index * 4 + 2], values[index * 4 + 3]);
}

const void matrix_4d::Clean() {
  //std::cout << "before clean: " << std::endl;
  //std::cout << this->values[0] << " " << this->values[1] << " " << this->values[2] << " " << this->values[3] << " " << std::endl;
  //std::cout << this->values[4] << " " << this->values[5] << " " << this->values[6] << " " << this->values[7] << " " << std::endl;
  //std::cout << this->values[8] << " " << this->values[9] << " " << this->values[10] << " " << this->values[11] << " " << std::endl;
  //std::cout << this->values[12] << " " << this->values[13] << " " << this->values[14] << " " << this->values[15] << " " << std::endl;
  //std::cout << std::endl;
  for (int i = 0; i < 16; i++) {
    if (fabs(this->values[i]) < threshold) this->values[i] = 0.0f;
  }
}

const matrix_4d matrix_4d::Transpose() const {
  std::array<float, 16> new_values;

  new_values[0]  = values[0];  // |  0  1  2  3 |T  =  |  0  4  8 12 |
  new_values[1]  = values[4];  // |  4  5  6  7 |      |  1  5  9 13 |
  new_values[2]  = values[8];  // |  8  9 10 11 |      |  2  6 10 14 |
  new_values[3]  = values[12]; // | 12 13 14 15 |      |  3  7 11 15 |
  new_values[4]  = values[1];
  new_values[5]  = values[5];
  new_values[6]  = values[9];
  new_values[7]  = values[13];
  new_values[8]  = values[2];
  new_values[9]  = values[6];
  new_values[10] = values[10];
  new_values[11] = values[14];
  new_values[12] = values[3];
  new_values[13] = values[7];
  new_values[14] = values[11];
  new_values[15] = values[15];
  
  return matrix_4d(new_values);
}

const float matrix_4d::Determinant() const {
  return
    this->values[3] * this->values[6] * this->values[9] * this->values[12] - this->values[2] * this->values[7] * this->values[9] * this->values[12] - this->values[3] * this->values[5] * this->values[10] * this->values[12] + this->values[1] * this->values[7] * this->values[10] * this->values[12] +
    this->values[2] * this->values[5] * this->values[11] * this->values[12] - this->values[1] * this->values[6] * this->values[11] * this->values[12] - this->values[3] * this->values[6] * this->values[8] * this->values[13] + this->values[2] * this->values[7] * this->values[8] * this->values[13] +
    this->values[3] * this->values[4] * this->values[10] * this->values[13] - this->values[0] * this->values[7] * this->values[10] * this->values[13] - this->values[2] * this->values[4] * this->values[11] * this->values[13] + this->values[0] * this->values[6] * this->values[11] * this->values[13] +
    this->values[3] * this->values[5] * this->values[8] * this->values[14] - this->values[1] * this->values[7] * this->values[8] * this->values[14] - this->values[3] * this->values[4] * this->values[9] * this->values[14] + this->values[0] * this->values[7] * this->values[9] * this->values[14] +
    this->values[1] * this->values[4] * this->values[11] * this->values[14] - this->values[0] * this->values[5] * this->values[11] * this->values[14] - this->values[2] * this->values[5] * this->values[8] * this->values[15] + this->values[1] * this->values[6] * this->values[8] * this->values[15] +
    this->values[2] * this->values[4] * this->values[9] * this->values[15] - this->values[0] * this->values[6] * this->values[9] * this->values[15] - this->values[1] * this->values[4] * this->values[10] * this->values[15] + this->values[0] * this->values[5] * this->values[10] * this->values[15];
}

const matrix_4d matrix_4d::Inverse() const {
  matrix_4d invert(
    this->values[6] * this->values[11] * this->values[13] - this->values[7] * this->values[10] * this->values[13] + this->values[7] * this->values[9] * this->values[14] - this->values[5] * this->values[11] * this->values[14] - this->values[6] * this->values[9] * this->values[15] + this->values[5] * this->values[10] * this->values[15],
    this->values[3] * this->values[10] * this->values[13] - this->values[2] * this->values[11] * this->values[13] - this->values[3] * this->values[9] * this->values[14] + this->values[1] * this->values[11] * this->values[14] + this->values[2] * this->values[9] * this->values[15] - this->values[1] * this->values[10] * this->values[15],
    this->values[2] * this->values[7] * this->values[13] - this->values[3] * this->values[6] * this->values[13] + this->values[3] * this->values[5] * this->values[14] - this->values[1] * this->values[7] * this->values[14] - this->values[2] * this->values[5] * this->values[15] + this->values[1] * this->values[6] * this->values[15],
    this->values[3] * this->values[6] * this->values[9] - this->values[2] * this->values[7] * this->values[9] - this->values[3] * this->values[5] * this->values[10] + this->values[1] * this->values[7] * this->values[10] + this->values[2] * this->values[5] * this->values[11] - this->values[1] * this->values[6] * this->values[11],
    this->values[7] * this->values[10] * this->values[12] - this->values[6] * this->values[11] * this->values[12] - this->values[7] * this->values[8] * this->values[14] + this->values[4] * this->values[11] * this->values[14] + this->values[6] * this->values[8] * this->values[15] - this->values[4] * this->values[10] * this->values[15],
    this->values[2] * this->values[11] * this->values[12] - this->values[3] * this->values[10] * this->values[12] + this->values[3] * this->values[8] * this->values[14] - this->values[0] * this->values[11] * this->values[14] - this->values[2] * this->values[8] * this->values[15] + this->values[0] * this->values[10] * this->values[15],
    this->values[3] * this->values[6] * this->values[12] - this->values[2] * this->values[7] * this->values[12] - this->values[3] * this->values[4] * this->values[14] + this->values[0] * this->values[7] * this->values[14] + this->values[2] * this->values[4] * this->values[15] - this->values[0] * this->values[6] * this->values[15],
    this->values[2] * this->values[7] * this->values[8] - this->values[3] * this->values[6] * this->values[8] + this->values[3] * this->values[4] * this->values[10] - this->values[0] * this->values[7] * this->values[10] - this->values[2] * this->values[4] * this->values[11] + this->values[0] * this->values[6] * this->values[11],
    this->values[5] * this->values[11] * this->values[12] - this->values[7] * this->values[9] * this->values[12] + this->values[7] * this->values[8] * this->values[13] - this->values[4] * this->values[11] * this->values[13] - this->values[5] * this->values[8] * this->values[15] + this->values[4] * this->values[9] * this->values[15],
    this->values[3] * this->values[9] * this->values[12] - this->values[1] * this->values[11] * this->values[12] - this->values[3] * this->values[8] * this->values[13] + this->values[0] * this->values[11] * this->values[13] + this->values[1] * this->values[8] * this->values[15] - this->values[0] * this->values[9] * this->values[15],
    this->values[1] * this->values[7] * this->values[12] - this->values[3] * this->values[5] * this->values[12] + this->values[3] * this->values[4] * this->values[13] - this->values[0] * this->values[7] * this->values[13] - this->values[1] * this->values[4] * this->values[15] + this->values[0] * this->values[5] * this->values[15],
    this->values[3] * this->values[5] * this->values[8] - this->values[1] * this->values[7] * this->values[8] - this->values[3] * this->values[4] * this->values[9] + this->values[0] * this->values[7] * this->values[9] + this->values[1] * this->values[4] * this->values[11] - this->values[0] * this->values[5] * this->values[11],
    this->values[6] * this->values[9] * this->values[12] - this->values[5] * this->values[10] * this->values[12] - this->values[6] * this->values[8] * this->values[13] + this->values[4] * this->values[10] * this->values[13] + this->values[5] * this->values[8] * this->values[14] - this->values[4] * this->values[9] * this->values[14],
    this->values[1] * this->values[10] * this->values[12] - this->values[2] * this->values[9] * this->values[12] + this->values[2] * this->values[8] * this->values[13] - this->values[0] * this->values[10] * this->values[13] - this->values[1] * this->values[8] * this->values[14] + this->values[0] * this->values[9] * this->values[14],
    this->values[2] * this->values[5] * this->values[12] - this->values[1] * this->values[6] * this->values[12] - this->values[2] * this->values[4] * this->values[13] + this->values[0] * this->values[6] * this->values[13] + this->values[1] * this->values[4] * this->values[14] - this->values[0] * this->values[5] * this->values[14],
    this->values[1] * this->values[6] * this->values[8] - this->values[2] * this->values[5] * this->values[8] + this->values[2] * this->values[4] * this->values[9] - this->values[0] * this->values[6] * this->values[9] - this->values[1] * this->values[4] * this->values[10] + this->values[0] * this->values[5] * this->values[10]
  );
  float inversed_determinant = 1 / invert.Determinant();
  return matrix_4d::Scaling(
    inversed_determinant,
    inversed_determinant,
    inversed_determinant,
    inversed_determinant
  );
}

const std::array<float, 16> matrix_4d::ColumnMajor() const {
  std::array<float, 16> new_values;

  new_values[0]  = values[0];  // |  0  1  2  3 |T  =  |  0  4  8 12 |
  new_values[1]  = values[4];  // |  4  5  6  7 |      |  1  5  9 13 |
  new_values[2]  = values[8];  // |  8  9 10 11 |      |  2  6 10 14 |
  new_values[3]  = values[12]; // | 12 13 14 15 |      |  3  7 11 15 |
  new_values[4]  = values[1];
  new_values[5]  = values[5];
  new_values[6]  = values[9];
  new_values[7]  = values[13];
  new_values[8]  = values[2];
  new_values[9]  = values[6];
  new_values[10] = values[10];
  new_values[11] = values[14];
  new_values[12] = values[3];
  new_values[13] = values[7];
  new_values[14] = values[11];
  new_values[15] = values[15];
  
  return new_values;
}

const matrix_3d matrix_4d::Normal() const {
  matrix_3d this_3d(
    this->values[0], this->values[1], this->values[2],
    this->values[4], this->values[5], this->values[6],
    this->values[8], this->values[9], this->values[10]
  );
  matrix_3d normal = matrix_3d(
    this_3d.values[4] * this_3d.values[8] - this_3d.values[5] * this_3d.values[7],
    this_3d.values[5] * this_3d.values[6] - this_3d.values[8] * this_3d.values[3],
    this_3d.values[3] * this_3d.values[7] - this_3d.values[4] * this_3d.values[6],
    this_3d.values[2] * this_3d.values[7] - this_3d.values[1] * this_3d.values[8],
    this_3d.values[0] * this_3d.values[8] - this_3d.values[2] * this_3d.values[6],
    this_3d.values[1] * this_3d.values[6] - this_3d.values[7] * this_3d.values[0],
    this_3d.values[1] * this_3d.values[5] - this_3d.values[4] * this_3d.values[2],
    this_3d.values[2] * this_3d.values[3] - this_3d.values[0] * this_3d.values[5],
    this_3d.values[0] * this_3d.values[4] - this_3d.values[3] * this_3d.values[1]
  ) / this_3d.Determinant();

  return normal;
}

const matrix_4d matrix_4d::Scaling(const float& x, const float& y, const float& z, const float& w) {
  return matrix_4d(x   , 0.0f, 0.0f, 0.0f,
                  0.0f, y   , 0.0f, 0.0f,
                  0.0f, 0.0f, z   , 0.0f,
                  0.0f, 0.0f, 0.0f, w   );
}

const matrix_4d matrix_4d::ScalingX(const float& x) {
  return matrix_4d(x   , 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 0.0f);
}

const matrix_4d matrix_4d::ScalingY(const float& y) {
  return matrix_4d(0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, y   , 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 0.0f);
}

const matrix_4d matrix_4d::ScalingZ(const float& z) {
  return matrix_4d(0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, z   , 0.0f,
                  0.0f, 0.0f, 0.0f, 0.0f);
}

const matrix_4d math::matrix_4d::ScalingW(const float & w) {
  return matrix_4d(0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, w   );
}

const matrix_5d matrix_4d::Translation(const float& x, const float& y, const float& z, const float& w) {
  return matrix_5d(1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                  x, y, z, w, 1.0f);
}

const matrix_5d matrix_4d::TranslationX(const float & x) {
  return matrix_5d(1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                  x, 0.0f, 0.0f, 0.0f, 1.0f);
}

const matrix_5d matrix_4d::TranslationY(const float & y) {
  return matrix_5d(1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                  0.0f, y, 0.0f, 0.0f, 1.0f);
}

const matrix_5d matrix_4d::TranslationZ(const float & z) {
  return matrix_5d(1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, z, 0.0f, 1.0f);
}

const matrix_5d matrix_4d::TranslationW(const float & w) {
  return matrix_5d(1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, w, 1.0f);
}

const matrix_4d matrix_4d::Rotation(const float& anglex, const float& angley, const float& anglez) {
  return matrix_4d::RotationX(anglex) * matrix_4d::RotationY(angley) * matrix_4d::RotationZ(anglez);
}

const matrix_4d matrix_4d::RotationX(const float& degree) {
  float angle = degree * DEGREES_TO_RADIANS;
  return matrix_4d(1.0f, 0.0f      ,  0.0f      , 0.0f,
                  0.0f, cos(angle), -sin(angle), 0.0f,
                  0.0f, sin(angle),  cos(angle), 0.0f,
                  0.0f, 0.0f      ,  0.0f      , 1.0f);
}

const matrix_4d matrix_4d::RotationY(const float& degree) {
  float angle = degree * DEGREES_TO_RADIANS;
  return matrix_4d( cos(angle), 0.0f, sin(angle), 0.0f,
                   0.0f      , 1.0f, 0.0f      , 0.0f,
                  -sin(angle), 0.0f, cos(angle), 0.0f,
                  0.0f       , 0.0f, 0.0f      , 1.0f);
}

const matrix_4d matrix_4d::RotationZ(const float& degree) {
  float angle = degree * DEGREES_TO_RADIANS;
  return matrix_4d(cos(angle), -sin(angle), 0.0f, 0.0f,
                  sin(angle),  cos(angle), 0.0f, 0.0f,
                  0.0f      ,  0.0f      , 1.0f, 0.0f,
                  0.0f      ,  0.0f      , 0.0f, 1.0f);
}

const matrix_4d math::matrix_4d::View(const vector_3d& eye, const vector_3d& center, const vector_3d& up) {
  vector_3d direction = (center - eye).Normalize();
  vector_3d right = direction.CrossProduct(up).Normalize();
  vector_3d calculated_up = right.CrossProduct(direction);

  return matrix_4d(right.x, calculated_up.x, -direction.x, 0.0f,
                   right.y, calculated_up.y, -direction.y, 0.0f,
                   right.z, calculated_up.z, -direction.z, 0.0f,
    -right.DotProduct(eye), -calculated_up.DotProduct(eye), direction.DotProduct(eye),  1.0f);
}

const matrix_4d math::matrix_4d::OrthographicProjection(const float& left, const float& right, const float& bottom, const float& top, const float& near_, const float& far_) {
  return matrix_4d(2 / (right - left), 0.0f              ,  0.0f              , 0.0f,
                   0.0f              , 2 / (top - bottom),  0.0f              , 0.0f,
                   0.0f              , 0.0f              , -2 / (far_ - near_), 0.0f,
                  -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far_ + near_) / (far_ - near_),  1.0f);
}

const matrix_4d math::matrix_4d::PerspectiveProjection(const float& degree, const float& aspect, const float& near_, const float& far_) {
  float fovy = degree * DEGREES_TO_RADIANS;
  return matrix_4d(1 / tanf(fovy / 2) / aspect,  0.0f              ,  0.0f                             , 0.0f,
                   0.0f                       ,  1 / tanf(fovy / 2),  0.0f                             , 0.0f,
                   0.0f                       ,  0.0f              , -(far_ + near_) / (far_ - near_)  , -1.0f,
                   0.0f                       ,  0.0f              , -2 * far_ * near_ / (far_ - near_), 0.0f);
}

const matrix_4d math::matrix_4d::Frustum(const float& left, const float& right, const float& bottom, const float& top, const float& near_, const float& far_) {
  return matrix_4d(2 * near_ / (right - left),     0.0f,                             0.0f,                                 0.0f,
                   0.0f,                           2 * near_ / (top - bottom),       0.0f,                                 0.0f,
                  (right + left) / (right - left), (top + bottom) / (top - bottom), -(far_ + near_) / (far_ - near_),     -1.0f,
                   0.0f,                           0.0f,                            -(2 * far_ * near_) / (far_ - near_),  0.0f);
}

const matrix_4d math::operator*(const float& scalar, const matrix_4d& matrix) {
  return matrix * scalar;
}

const matrix_4d math::operator*(const int& scalar, const matrix_4d& matrix) {
  return matrix * scalar;
}

const vector_4d math::operator*(const vector_4d& vector, const matrix_4d& matrix) {
  return matrix * vector;
}

std::ostream& math::operator<<(std::ostream& os, const matrix_4d& matrix) {
  os << "(";
  for (auto i = 0u; i < matrix.values.size(); i++) {
    os << matrix[i];
  }
  os << ")";

  return os;
}