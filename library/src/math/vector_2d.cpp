#include "math/vector_2d.hpp"

using namespace math;

vector_2d::vector_2d(const float& x, const float& y)
  // Set private attributes x and y with copys x and y,
  // respectively 
  : x(x), y(y) {}

vector_2d::vector_2d(const int & x, const int & y)
  // Set private attributes x and y with copys x and y,
  // respectively 
  : x((float)x), y((float)y) {}

vector_2d::vector_2d(const std::array<float, 2>& values)
  : x(values[0]), y(values[1]) {}

const bool vector_2d::operator==(const vector_2d& other_vector) const {
  // Check if values from private attributes x and y matches values (other.x
  // and other.y, respectively) from reference other vector
  return this->x == other_vector.x &&
         this->y == other_vector.y;
}

const bool vector_2d::operator!=(const vector_2d& other_vector) const {
  // Return the opposite of operator== result
  return !(*this == other_vector);
}

const vector_2d vector_2d::operator+ (const vector_2d& other_vector) const {
  // Create new x and y by summing each of vectors' attributes
  // and join them into a new 2D vector
  const float newx = this->x + other_vector.x;
  const float newy = this->y + other_vector.y;
  vector_2d new_vector(newx, newy);

  // Return a copy of new vector
  return new_vector;
}

const vector_2d vector_2d::operator- (const vector_2d& other_vector) const {
  // Create new x and y by subtracting each of vectors' attributes
  // and join them into a new 2D vector
  const float newx = this->x - other_vector.x;
  const float newy = this->y - other_vector.y;
  vector_2d new_vector(newx, newy);

  // Return a copy of new vector
  return new_vector;
}

const vector_2d vector_2d::operator* (const float& scalar) const {
  // Create new x and y by multiplying each of vector's attribute with scalar
  // and join them into a new 2D vector
  const float newx = this->x * scalar;
  const float newy = this->y * scalar;
  vector_2d new_vector(newx, newy);

  // Return a copy of new vector
  return new_vector;
}

const vector_2d vector_2d::operator*(const int& scalar) const {
  // Operator * overload
  return *this * (float)scalar;
}

const vector_2d vector_2d::operator/ (const float& scalar) const {
  // Create new x and y by dividing each of vector's attribute with scalar
  // and join them into a new 2D vector
  const float newx = this->x / scalar;
  const float newy = this->y / scalar;
  vector_2d new_vector(newx, newy);

  // Return a copy of new vector
  return new_vector;
}

const vector_2d vector_2d::operator/(const int& scalar) const {
  // Operator / overload
  return *this / (float)scalar;
}

const float vector_2d::operator[](const int& index) const {
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
    // Throw an out of range error if index
    // does not correspond with any element of the vector
    default:
      throw std::out_of_range("vector_2d::operator[](): index is out of range");
      break;
  }
}

const float vector_2d::Quadrance(const vector_2d& other_vector) const {
  // Calculate quadrance
  const float calculated_quadrance = powf((other_vector.x - this->x), 2.0f) +
                                     powf((other_vector.y - this->y), 2.0f);

  // Return a copy of calculated quadrance
  return calculated_quadrance;
}

const float vector_2d::Norm() const {
  // Calculate norm
  const float calculated_norm = sqrtf(powf(this->x, 2.0f) +
                                      powf(this->y, 2.0f));

  // Return a reference of calculated norm
  return calculated_norm;
}

const vector_2d vector_2d::Normalize() const {
  // Obtain calculated norm
  const float calculated_norm = this->Norm();

  // Calculate normalized vector
  vector_2d calculated_normalized_vector = *this / calculated_norm;

  // Return a reference of calculated normalized vector
  return calculated_normalized_vector;
}

const float vector_2d::DotProduct(const vector_2d& other_vector) const {
  // Calculate dot product by multiplying each of vectors' attributes
  // and sum all multiplications
  const float calculated_dot_product = this->x * other_vector.x +
                                       this->y * other_vector.y;

  // Return a reference of calculated dot product
  return calculated_dot_product;
}

const float vector_2d::CrossProduct(const vector_2d& other_vector) const {
  // Calculate cross product
  const float calculated_cross_product = this->x * other_vector.y - this->y * other_vector.x;

  // Return a copy of calculated cross product
  return calculated_cross_product;
}

const vector_2d math::operator* (const float& scalar, const vector_2d& vector) {
  // Operator * overload
  return vector * scalar;
}

const vector_2d math::operator* (const int& scalar, const vector_2d& vector) {
  // Operator * overload
  return vector * scalar;
}

std::ostream& math::operator<<(std::ostream& os, const vector_2d& vector) {
  return os << "(" << vector.x << ", " << vector.y << ")";
}