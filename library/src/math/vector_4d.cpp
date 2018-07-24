#include "math/vector_4d.hpp"

using namespace math;

vector_4d::vector_4d(const float& x, const float& y, const float& z, const float& w)
  // Set private attributes x, y, z and w with immutable references x, y, z and w
  // respectively
  : x(x), y(y), z(z), w(w) {}

vector_4d::vector_4d(const int& x, const int& y, const int& z, const int& w)
  // Set private attributes x, y, z and w with immutable references x, y, z and w,
  // respectively
  : x((float)x), y((float)y), z((float)z), w((float)w) {}

vector_4d::vector_4d(const std::array<float, 4>& values)
  : x(values[0]), y(values[1]), z(values[2]), w(values[3]) {}

const bool vector_4d::operator==(const vector_4d& other_vector) const {
  // Check if values from private attributes x and y matches values (other.x,
  // other.y and other.z, respectively) from immutable reference other vector
  return this->x == other_vector.x &&
    this->y == other_vector.y &&
    this->z == other_vector.z &&
    this->w == other_vector.w;
}

const bool vector_4d::operator!=(const vector_4d& other_vector) const {
  // Return the opposite of operator== result
  return !(*this == other_vector);
}

const vector_4d vector_4d::operator+ (const vector_4d& other_vector) const {
  // Create new x, y, z and w by summing each of vectors' attributes,
  // without receiving real-time updates from any operand
  // and join them into a new 2D vector
  const float newx = this->x + other_vector.x;
  const float newy = this->y + other_vector.y;
  const float newz = this->z + other_vector.z;
  const float neww = this->w + other_vector.w;
  vector_4d new_vector(newx, newy, newz, neww);

  // Return a reference of new vector
  return new_vector;
}

const vector_4d vector_4d::operator- (const vector_4d& other_vector) const {
  // Create new x, y, z and w by subtracting each of vectors' attributes,
  // without receiving real-time updates from any operand
  // and join them into a new 2D vector
  const float newx = this->x - other_vector.x;
  const float newy = this->y - other_vector.y;
  const float newz = this->z - other_vector.z;
  const float neww = this->w - other_vector.w;
  vector_4d new_vector(newx, newy, newz, neww);

  // Return a reference of new vector
  return new_vector;
}

const vector_4d vector_4d::operator* (const float& scalar) const {
  // Create new x, y, z and w by multiplying each of vector's attribute with scalar,
  // without receiving real-time updates from any operand
  // and join them into a new 2D vector
  const float newx = this->x * scalar;
  const float newy = this->y * scalar;
  const float newz = this->z * scalar;
  const float neww = this->w * scalar;
  vector_4d new_vector(newx, newy, newz, neww);

  // Return a reference of new vector
  return new_vector;
}

const vector_4d vector_4d::operator*(const int& scalar) const {
  // Operator * overload
  return *this * (float)scalar;
}

const vector_4d vector_4d::operator/ (const float& scalar) const {
  // Create new x, y, z and w by dividing each of vector's attribute with scalar,
  // without receiving real-time updates from any operand
  // and join them into a new 2D vector
  const float newx = this->x / scalar;
  const float newy = this->y / scalar;
  const float newz = this->z / scalar;
  const float neww = this->w / scalar;
  vector_4d new_vector(newx, newy, newz, neww);

  // Return a reference of new vector
  return new_vector;
}

const vector_4d vector_4d::operator/(const int& scalar) const {
  // Operator / overload
  return *this / (float)scalar;
}

const float vector_4d::operator[](const int& index) const {
  // Check a copy of index
  switch (index) {
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
    // Return a copy of private attribute w,
    // corresponding to the forth element of the vector
    case 3:
      return w;
      break;
    // Throw an out of range error if index
    // does not correspond with any element of the vector
    default:
      throw std::out_of_range("vector_2d::operator[](): index is out of range");
      break;
  }
}

const float vector_4d::Quadrance(const vector_4d& other_vector) const {
  // Calculate quadrance
  float calculated_quadrance = powf((other_vector.x - this->x), 2.0f) +
    powf((other_vector.y - this->y), 2.0f) +
    powf((other_vector.z - this->z), 2.0f) +
    powf((other_vector.w - this->w), 2.0f);

// Return a reference of calculated quadrance
  return calculated_quadrance;
}

const float vector_4d::Norm() const {
  // Calculate norm
  float calculated_norm = sqrtf(powf(this->x, 2.0f) +
                                powf(this->y, 2.0f) +
                                powf(this->z, 2.0f) +
                                powf(this->w, 2.0f));

  // Return a reference of calculated norm
  return calculated_norm;
}

const vector_4d vector_4d::Normalize() const {
  // Obtain calculated norm
  float calculated_norm = this->Norm();

  // Calculate normalized vector
  vector_4d calculated_normalized_vector = *this / calculated_norm;

  // Return a reference of calculated normalized vector
  return calculated_normalized_vector;
}

const float vector_4d::DotProduct(const vector_4d& other_vector) const {
  // Calculate dot product by multiplying each of vectors' attributes,
  // without receiving real-time updates from any operand
  // and sum all multiplications
  float calculated_dot_product = this->x * other_vector.x +
    this->y * other_vector.y +
    this->z * other_vector.z +
    this->w * other_vector.w;

// Return a reference of calculated dot product
  return calculated_dot_product;
}

const vector_4d math::operator* (const float& scalar, const vector_4d& vector) {
  // Operator * overload
  return vector * scalar;
}

const vector_4d math::operator* (const int& scalar, const vector_4d& vector) {
  // Operator * overload
  return vector * scalar;
}

std::ostream& math::operator<<(std::ostream& os, const vector_4d& vector) {
  return os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
}