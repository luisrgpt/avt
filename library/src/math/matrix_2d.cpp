#include "math/matrix_2d.hpp"

using namespace math;

matrix_2d::matrix_2d(const float& xx, const float& xy,
                          const float& yx, const float& yy)
  : values{xx, xy,
            yx, yy} {}

matrix_2d::matrix_2d(const int& xx, const int& xy,
                          const int& yx, const int& yy)
  : values{(float)xx, (float)xy,
            (float)yx, (float)yy} {}

matrix_2d::matrix_2d(const vector_2d& x, const vector_2d& y)
  : values{x.x, x.y,
            y.x, y.y} {}

matrix_2d::matrix_2d(const std::array<float, 4>& values)
  : values(values) {}

const bool matrix_2d::operator==(const matrix_2d& other_matrix) const {
  for (std::size_t index = 0; index < values.size(); index++) {
    if(values[index] != other_matrix.values[index]) {
      return false;
    }
  }
  
  return true;
}

const bool matrix_2d::operator!=(const matrix_2d& other_matrix) const {
  return !(*this == other_matrix);
}

const matrix_2d matrix_2d::operator+(const matrix_2d& other_matrix) const {
  std::array<float, 4> new_values;
  
  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] + other_matrix.values[index];
  }
  
  const matrix_2d new_matrix(new_values);

  return new_matrix;
}

const matrix_2d matrix_2d::operator-(const matrix_2d& other_matrix) const {
  std::array<float, 4> new_values;
  
  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] - other_matrix.values[index];
  }
  
  const matrix_2d new_matrix(new_values);

  return new_matrix;
}

const matrix_2d matrix_2d::operator*(const float& scalar) const {
  std::array<float, 4> new_values;

  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] * scalar;
  }
  
  const matrix_2d new_matrix(new_values);

  return new_matrix;
}

const matrix_2d matrix_2d::operator*(const int& scalar) const {
  return *this * (float)scalar;
}

const vector_2d matrix_2d::operator*(const vector_2d& vector) const {
  std::array<float, 2> new_values;
  
  for (std::size_t index = 0; index < 2; index++) {
    new_values[index] = values[index * 2] * vector[0] + values[index * 2 + 1] * vector[1];
  }
  
  const vector_2d new_vector(new_values);

  return new_vector;
}

const matrix_2d matrix_2d::operator*(const matrix_2d& other_matrix) const {
  std::array<float, 4> new_values;
  
  for (std::size_t index = 0; index < 2; index++) {
    new_values[index]     = values[index]     * other_matrix.values[0] +
                            values[index + 2] * other_matrix.values[1];

    new_values[index + 2] = values[index]     * other_matrix.values[2] +
                            values[index + 2] * other_matrix.values[3];
  }

  const matrix_2d new_matrix(new_values);

  return new_matrix;
}

const matrix_2d matrix_2d::operator/(const float& scalar) const {
  std::array<float, 4> new_values;

  for (std::size_t index = 0; index < values.size(); index++) {
    new_values[index] = values[index] / scalar;
  }
  
  const matrix_2d new_matrix(new_values);

  return new_matrix;
}

const matrix_2d matrix_2d::operator/(const int& scalar) const {
  return *this / (float)scalar;
}

const matrix_2d matrix_2d::operator/(const matrix_2d& other_matrix) const {
  return *this * other_matrix.Inverse();
}

const vector_2d matrix_2d::operator[](const int& index) const {
  if(index < 0 && index >= 2) {
      throw std::out_of_range("matrix_2d::operator[](): index is out of range");
  }
  
  const vector_2d new_vector(values[index * 2], values[index * 2 + 1]);
  
  return new_vector;
}

const matrix_2d matrix_2d::Transpose() const {
  std::array<float, 4> new_values;

  new_values[0] = values[0]; // | 0 1 |T  =  | 0 2 |
  new_values[1] = values[2]; // | 2 3 |      | 1 3 |
  new_values[2] = values[1];
  new_values[3] = values[3];
  
  const matrix_2d new_matrix(new_values);

  return new_matrix;
}

const float matrix_2d::Determinant() const {
  float calculated_determinant;

  calculated_determinant = values[0] * values[3] - values[1] * values[2];
  
  return calculated_determinant;
}

const matrix_2d matrix_2d::Inverse() const {
  return  matrix_2d( values[3], -values[1],
                   -values[2],  values[0]) / this->Determinant();
}

const std::array<float, 4> matrix_2d::ColumnMajor() const {
  std::array<float, 4> new_values;

  new_values[0] = values[0]; // | 00(0) 01(1) |T  =  | 0 2 |
  new_values[1] = values[2]; // | 10(2) 11(3) |      | 1 3 |
  new_values[2] = values[1];
  new_values[3] = values[3];

  return new_values;
}

const matrix_2d matrix_2d::Scaling(const float& x, const float& y) {
  return matrix_2d(x   , 0.0f,
                  0.0f, y   );
}

const matrix_2d matrix_2d::ScalingX(const float& x) {
  return matrix_2d(x   , 0.0f,
                  0.0f, 0.0f);
}

const matrix_2d matrix_2d::ScalingY(const float& y) {
  return matrix_2d(0.0f, 0.0f,
                  0.0f, y   );
}

const matrix_3d matrix_2d::Translation(const float& x, const float& y) {
  matrix_3d new_matrix(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    x, y, 1.0f
  );

  return new_matrix;
}

const matrix_3d matrix_2d::TranslationX(const float & x) {
  matrix_3d new_matrix(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    x, 0.0f, 1.0f
  );

  return new_matrix;
}

const matrix_3d matrix_2d::TranslationY(const float & y) {
  matrix_3d new_matrix(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, y, 1.0f
  );

  return new_matrix;
}

const matrix_2d matrix_2d::Rotation(const float& angle) {
  return matrix_2d(cos(angle), -sin(angle),
                  sin(angle),  cos(angle));
}

const matrix_2d math::operator*(const float& scalar, const matrix_2d& matrix) {
  return matrix * scalar;
}

const matrix_2d math::operator*(const int& scalar, const matrix_2d& matrix) {
  return matrix * scalar;
}

const vector_2d math::operator*(const vector_2d& vector, const matrix_2d& matrix) {
  return matrix * vector;
}

std::ostream& math::operator<<(std::ostream& os, const matrix_2d& matrix) {
  os << "(";
  for (auto i = 0u; i < matrix.values.size(); i++) {
    os << matrix[i];
  }
  os << ")";

  return os;
}
