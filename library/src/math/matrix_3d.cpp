#include "math/matrix_3d.hpp"

using namespace math;

#define DEGREES_TO_RADIANS 0.01745329251994329547

matrix_3d::matrix_3d(const float& xx, const float& xy, const float& xz,
                          const float& yx, const float& yy, const float& yz,
                          const float& zx, const float& zy, const float& zz)
  : values{xx, xy, xz,
            yx, yy, yz,
            zx, zy, zz} {}

matrix_3d::matrix_3d(const int& xx, const int& xy, const int& xz,
                          const int& yx, const int& yy, const int& yz,
                          const int& zx, const int& zy, const int& zz)
  : values{(float)xx, (float)xy, (float)xz,
            (float)yx, (float)yy, (float)yz,
            (float)zx, (float)zy, (float)zz} {}

matrix_3d::matrix_3d(const vector_3d& x, const vector_3d& y, const vector_3d& z)
  : values{x.x, x.y, x.z,
            y.x, y.y, y.z,
             z.x, z.y, z.z} {}

matrix_3d::matrix_3d(const std::array<float, 9>& values)
  : values(values) {}

const bool matrix_3d::operator==(const matrix_3d& other_matrix) const {
  for (std::size_t index = 0; index < values.size(); index++) {
    if(values[index] != other_matrix.values[index]) {
      return false;
    }
  }
  
  return true;
}

const bool matrix_3d::operator!=(const matrix_3d& other_matrix) const {
  return !(*this == other_matrix);
}

const matrix_3d matrix_3d::operator+(const matrix_3d& other_matrix) const {
  std::array<float, 9> new_values;
  
  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] + other_matrix.values[index];
  }
  
  return matrix_3d(new_values);
}

const matrix_3d matrix_3d::operator-(const matrix_3d& other_matrix) const {
  std::array<float, 9> new_values;
  
  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] - other_matrix.values[index];
  }
  
  return matrix_3d(new_values);
}

const matrix_3d matrix_3d::operator*(const float& scalar) const {
  std::array<float, 9> new_values;

  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] * scalar;
  }
  
  return matrix_3d(new_values);
}

const matrix_3d matrix_3d::operator*(const int& scalar) const {
  return *this * (float)scalar;
}

const vector_3d matrix_3d::operator*(const vector_3d& vector) const {
  std::array<float, 3> new_values;
  
  for (std::size_t index = 0; index < 3; index++) {
    new_values[index] = values[index * 3] * vector[0] + values[index * 3 + 1] * vector[1] + values[index * 3 + 2] * vector[2];
  }
  
  return vector_3d(new_values);
}

const matrix_3d matrix_3d::operator*(const matrix_3d& other_matrix) const {
  std::array<float, 9> new_values;
  
  for (std::size_t index = 0; index < 3; index++) {
    new_values[index]     = values[index]     * other_matrix.values[0] +
                            values[index + 3] * other_matrix.values[1] +
                            values[index + 6] * other_matrix.values[2];

    new_values[index + 3] = values[index]     * other_matrix.values[3] +
                            values[index + 3] * other_matrix.values[4] +
                            values[index + 6] * other_matrix.values[5];

    new_values[index + 6] = values[index]     * other_matrix.values[6] +
                            values[index + 3] * other_matrix.values[7] +
                            values[index + 6] * other_matrix.values[8];
  }

  return matrix_3d(new_values);
}

const matrix_3d matrix_3d::operator/(const float& scalar) const {
  std::array<float, 9> new_values;

  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] / scalar;
  }
  
  return matrix_3d(new_values);
}

const matrix_3d matrix_3d::operator/(const int& scalar) const {
  return *this / (float)scalar;
}

const matrix_3d matrix_3d::operator/(const matrix_3d& other_matrix) const {
  return *this * other_matrix.Inverse();
}

const vector_3d matrix_3d::operator[](const int& index) const {
  if(index < 0 && index >= 3) {
      throw std::out_of_range("matrix_3d::operator[](): index is out of range");
  }
  
  return vector_3d(values[index * 3], values[index * 3 + 1], values[index * 3 + 2]);
}

const matrix_3d matrix_3d::Transpose() const {
  std::array<float, 9> new_values;

  new_values[0] = values[0]; // | 0 1 2 |T  =  | 0 3 6 |
  new_values[1] = values[3]; // | 3 4 5 |      | 1 4 7 |
  new_values[2] = values[6]; // | 6 7 8 |      | 2 5 8 |
  new_values[3] = values[1];
  new_values[4] = values[4];
  new_values[5] = values[7];
  new_values[6] = values[2];
  new_values[7] = values[5];
  new_values[8] = values[8];
  
  return matrix_3d(new_values);
}

const float matrix_3d::Determinant() const {
  float calculated_determinant;

  calculated_determinant = values[0] * values[4] * values[8] +
                           values[1] * values[5] * values[6] +
                           values[2] * values[3] * values[7] -
                           values[2] * values[4] * values[6] -
                           values[1] * values[3] * values[8] -
                           values[0] * values[5] * values[7];
  
  return calculated_determinant;
}

const matrix_3d matrix_3d::Inverse() const {
  return matrix_3d(values[4] * values[8] - values[5] * values[7],
                  values[2] * values[7] - values[1] * values[8],
                  values[1] * values[5] - values[2] * values[4],

                  values[5] * values[6] - values[3] * values[8],
                  values[0] * values[8] - values[2] * values[6],
                  values[2] * values[3] - values[0] * values[5],

                  values[3] * values[7] - values[4] * values[6],
                  values[1] * values[6] - values[0] * values[7],
                  values[0] * values[4] - values[1] * values[3]) / this->Determinant();
}

const std::array<float, 9> matrix_3d::ColumnMajor() const {
  std::array<float, 9> new_values;

  new_values[0] = values[0]; // | 0 1 2 |T  =  | 0 3 6 |
  new_values[1] = values[3]; // | 3 4 5 |      | 1 4 7 |
  new_values[2] = values[6]; // | 6 7 8 |      | 2 5 8 |
  new_values[3] = values[1];
  new_values[4] = values[4];
  new_values[5] = values[7];
  new_values[6] = values[2];
  new_values[7] = values[5];
  new_values[8] = values[8];
  
  return new_values;
}

const matrix_3d matrix_3d::Scaling(const float& x, const float& y, const float& z) {
  return matrix_3d(x   , 0.0f, 0.0f,
                  0.0f, y   , 0.0f,
                  0.0f, 0.0f, z   );
}

const matrix_3d matrix_3d::ScalingX(const float& x) {
  return matrix_3d(x   , 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f);
}

const matrix_3d matrix_3d::ScalingY(const float& y) {
  return matrix_3d(0.0f, 0.0f, 0.0f,
                  0.0f, y   , 0.0f,
                  0.0f, 0.0f, 0.0f);
}

const matrix_3d matrix_3d::ScalingZ(const float& z) {
  return matrix_3d(0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, z   );
}

const matrix_4d matrix_3d::Translation(const float& x, const float& y, const float& z) {
  matrix_4d new_matrix(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    x, y, z, 1.0f
  );

  return new_matrix;
}

const matrix_4d matrix_3d::TranslationX(const float & x) {
  matrix_4d new_matrix(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    x, 0.0f, 0.0f, 1.0f
  );

  return new_matrix;
}

const matrix_4d matrix_3d::TranslationY(const float & y) {
  matrix_4d new_matrix(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, y, 0.0f, 1.0f
  );

  return new_matrix;
}

const matrix_4d matrix_3d::TranslationZ(const float & z) {
  matrix_4d new_matrix(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, z, 1.0f
  );

  return new_matrix;
}

const matrix_3d matrix_3d::Rotation(const float& anglex, const float& angley, const float& anglez) {
  return matrix_3d::RotationX(anglex) * matrix_3d::RotationY(angley) * matrix_3d::RotationZ(anglez);
}

const matrix_3d matrix_3d::RotationX(const float& degree) {
  float angle = degree * DEGREES_TO_RADIANS;
  return matrix_3d(1.0f, 0.0f      , 0.0f,
                  0.0f, cosf(angle), -sinf(angle),
                  0.0f, sinf(angle),  cosf(angle));
}

const matrix_3d matrix_3d::RotationY(const float& degree) {
  float angle = degree * DEGREES_TO_RADIANS;
  return matrix_3d( cosf(angle), 0.0f, sinf(angle),
                   0.0f      , 1.0f, 0.0f,
                  -sinf(angle), 0.0f, cosf(angle));
}

const matrix_3d matrix_3d::RotationZ(const float& degree) {
  float angle = degree * DEGREES_TO_RADIANS;
  return matrix_3d(cosf(angle), -sinf(angle), 0.0f,
                  sinf(angle),  cosf(angle), 0.0f,
                  0.0f      ,  0.0f      , 1.0f);
}

const matrix_3d matrix_3d::Dual(const float& x, const float& y, const float& z) {
  return matrix_3d( 0.0f, -z   ,  y   ,
                   z   ,  0.0f, -x   ,
                  -y   ,  x   , 0.0f);
}

const matrix_3d matrix_3d::Dual(const vector_3d& vector) {
  return matrix_3d( 0.0f      , -vector.z,  vector.y,
                   vector.z,  0.0f      , -vector.x,
                  -vector.y,  vector.x,  0.0f      );
}

const matrix_3d math::operator*(const float& scalar, const matrix_3d& matrix) {
  return matrix * scalar;
}

const matrix_3d math::operator*(const int& scalar, const matrix_3d& matrix) {
  return matrix * scalar;
}

const vector_3d math::operator*(const vector_3d& vector, const matrix_3d& matrix) {
  return matrix * vector;
}

std::ostream& math::operator<<(std::ostream& os, const matrix_3d& matrix) {
  os << "(";
  for (auto i = 0u; i < matrix.values.size(); i++) {
    os << matrix[i];
  }
  os << ")";

  return os;
}