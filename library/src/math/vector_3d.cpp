#include "math/vector_3d.hpp"

using namespace math;

vector_3d::vector_3d(const float& x, const float& y, const float& z)
  // Set private attributes x, y and z with immutable references x, y and z,
  // respectively
  : x(x), y(y), z(z) {}

vector_3d::vector_3d(const int& x, const int& y, const int& z)
  // Set private attributes x, y and z with immutable references x, y and z,
  // respectively
  : x((float)x), y((float)y), z((float)z) {}

vector_3d::vector_3d(const std::array<float, 3>& values)
  : x(values[0]), y(values[1]), z(values[2]) {}

const bool vector_3d::operator==(const vector_3d& other_vector) const {
  // Check if values from private attributes this->x and this->y matches values (other.x,
  // other.y and other.z, respectively) from immutable reference other vector
  return this->x == other_vector.x &&
         this->y == other_vector.y &&
         this->z == other_vector.z;
}

const bool vector_3d::operator!=(const vector_3d& other_vector) const {
  // Return the opposite of operator== result
  return !(*this == other_vector);
}

const vector_3d& vector_3d::operator+= (const vector_3d& other_vector) {
  this->x += other_vector.x;
  this->y += other_vector.y;
  this->z += other_vector.z;
  return (*this);
}

const vector_3d& vector_3d::operator-= (const vector_3d& other_vector) {
  this->x -= other_vector.x;
  this->y -= other_vector.y;
  this->z -= other_vector.z;
  return (*this);
}

const vector_3d& vector_3d::operator*= (const float& scalar) {
  this->x *= scalar;
  this->y *= scalar;
  this->z *= scalar;
  return (*this);
}

const vector_3d& vector_3d::operator*=(const int& scalar) {
  // Operator * overload
  return *this * (float)scalar;
}

const vector_3d& vector_3d::operator/= (const float& scalar) {
  this->x /= scalar;
  this->y /= scalar;
  this->z /= scalar;
  return (*this);
}

const vector_3d& vector_3d::operator/=(const int& scalar) {
  // Operator / overload
  return *this / (float)scalar;
}

const vector_3d vector_3d::operator+ (const vector_3d& other_vector) const {
  // Create new x, y and z by summing each of vectors' attributes,
  // without receiving real-time updates from any operand
  // and join them into a new 2D vector
  const float newx = this->x + other_vector.x;
  const float newy = this->y + other_vector.y;
  const float newz = this->z + other_vector.z;
  vector_3d new_vector(newx, newy, newz);

  // Return a reference of new vector
  return new_vector;
}

const vector_3d vector_3d::operator- (const vector_3d& other_vector) const {
  // Create new x, y and z by subtracting each of vectors' attributes,
  // without receiving real-time updates from any operand
  // and join them into a new 2D vector
  const float newx = this->x - other_vector.x;
  const float newy = this->y - other_vector.y;
  const float newz = this->z - other_vector.z;
  vector_3d new_vector(newx, newy, newz);

  // Return a reference of new vector
  return new_vector;
}

const vector_3d vector_3d::operator* (const float& scalar) const {
  // Create new x, y and z by multiplying each of vector's attribute with scalar,
  // without receiving real-time updates from any operand
  // and join them into a new 2D vector
  const float newx = this->x * scalar;
  const float newy = this->y * scalar;
  const float newz = this->z * scalar;
  vector_3d new_vector(newx, newy, newz);

  // Return a reference of new vector
  return new_vector;
}

const vector_3d vector_3d::operator*(const int& scalar) const {
  // Operator * overload
  return *this * (float)scalar;
}

const vector_3d vector_3d::operator/ (const float& scalar) const {
  // Create new x, y and z by dividing each of vector's attribute with scalar,
  // without receiving real-time updates from any operand
  // and join them into a new 2D vector
  const float newx = this->x / scalar;
  const float newy = this->y / scalar;
  const float newz = this->z / scalar;
  vector_3d new_vector(newx, newy, newz);

  // Return a reference of new vector
  return new_vector;
}

const vector_3d vector_3d::operator/(const int& scalar) const {
  // Operator / overload
  return *this / (float)scalar;
}

const float vector_3d::operator[](const int& index) const {
  // Check a copy of index
  switch(index) {
    // Return a copy of private attribute x,
    // corresponding to the first element of the vector
    case 0:
      return x;
      break;
    // Return a copy of private attribute y,
    // corresponding to the second element of the vector
    case 1:
      return y;
      break;
    // Return a copy of private attribute z,
    // corresponding to the third element of the vector
    case 2:
      return z;
      break;
    // Throw an out of range error if index
    // does not correspond with any element of the vector
    default:
      throw std::out_of_range("vector_2d::operator[](): index is out of range");
      break;
  }
}

const float vector_3d::Quadrance(const vector_3d& other_vector) const {
  // Calculate quadrance
  float calculated_quadrance = powf((other_vector.x - this->x), 2.0f) +
                               powf((other_vector.y - this->y), 2.0f) +
                               powf((other_vector.z - this->z), 2.0f);

  // Return a reference of calculated quadrance
  return calculated_quadrance;
}

const float vector_3d::Norm() const {
  // Calculate norm
  float calculated_norm = sqrtf(powf(this->x, 2.0f) +
                                powf(this->y, 2.0f) +
                                powf(this->z, 2.0f));

  // Return a reference of calculated norm
  return calculated_norm;
}

const vector_3d vector_3d::Normalize() const {
  // Obtain calculated norm
  float calculated_norm = this->Norm();

  // Calculate normalized vector
  vector_3d calculated_normalized_vector = *this / calculated_norm;

  // Return a reference of calculated normalized vector
  return calculated_normalized_vector;
}

const float vector_3d::DotProduct(const vector_3d& other_vector) const {
  // Calculate dot product by multiplying each of vectors' attributes,
  // without receiving real-time updates from any operand
  // and sum all multiplications
  float calculated_dot_product = this->x * other_vector.x +
                                 this->y * other_vector.y +
                                 this->z * other_vector.z;

  // Return a reference of calculated dot product
  return calculated_dot_product;
}

const vector_3d vector_3d::CrossProduct(const vector_3d& other_vector) const {
  // Calculate cross product
  const float newx = this->y * other_vector.z - this->z * other_vector.y;
  const float newy = this->z * other_vector.x - this->x * other_vector.z;
  const float newz = this->x * other_vector.y - this->y * other_vector.x;
  vector_3d calculated_cross_product(newx, newy, newz);

  // Return a reference of calculated cross product
  return calculated_cross_product;
}

const vector_3d math::vector_3d::Rotate(const vector_3d & other_vector, const float & angle) const {
  return (*this) * cosf(angle) +
         other_vector.CrossProduct(*this) * sinf(angle) +
         other_vector * other_vector.DotProduct(*this) * (1 - cosf(angle));
}

const vector_3d math::operator*(const float& scalar, const vector_3d& vector) {
  // Operator * overload
  return vector * scalar;
}

const vector_3d math::operator*(const int& scalar, const vector_3d& vector) {
  // Operator * overload
  return vector * scalar;
}

std::ostream& math::operator<<(std::ostream& os, const vector_3d& vector) {
  return os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
}