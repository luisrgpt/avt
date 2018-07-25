#include "math/quaternion.hpp"

using namespace math;

const float threshold = (float)1.0e-5;
#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

quaternion::quaternion(const float& x, const float& y, const float& z, const float& w)
// Set private attributes x, y, z and w with immutable references x, y, z and w
// respectively
  : x(x), y(y), z(z), w(w) {}

quaternion::quaternion(const int& x, const int& y, const int& z, const int& w)
// Set private attributes x, y, z and w with immutable references x, y, z and w,
// respectively
  : x((float)x), y((float)y), z((float)z), w((float)w) {}

quaternion::quaternion(const std::array<float, 4>& values)
  : x(values[0]), y(values[1]), z(values[2]), w(values[3]) {}

quaternion::quaternion(const float& theta, const vector_3d& axis) {
  vector_3d axisn = axis.Normalize();

  float a = theta * (float)DEGREES_TO_RADIANS;
  this->w = cosf(a / 2.0f);
  float s = sinf(a / 2.0f);
  this->x = axisn.x * s;
  this->y = axisn.y * s;
  this->z = axisn.z * s;
  //std::cout << "before gotcha " << this->x << " " << this->y << " " << this->z << " " << this->w << std::endl;
  this->Clean();
  //std::cout << "after gotcha " << this->x << " " << this->y << " " << this->z << " " << this->w << std::endl;
}

const bool quaternion::operator==(const quaternion& other_quaternion) const {
  // Check if values from private attributes x and y matches values (other.x,
  // other.y and other.z, respectively) from immutable reference other quaternion
  return
    fabs(this->x - other_quaternion.x) < threshold &&
    fabs(this->y - other_quaternion.y) < threshold &&
    fabs(this->z - other_quaternion.z) < threshold &&
    fabs(this->w - other_quaternion.w) < threshold;
}

const bool quaternion::operator!=(const quaternion& other_quaternion) const {
  // Return the opposite of operator== result
  return !(*this == other_quaternion);
}

const quaternion quaternion::operator+ (const quaternion& other_quaternion) const {
  // Create new x, y, z and w by summing each of vectors' attributes,
  // without receiving real-time updates from any operand
  // and join them into a new 2D quaternion
  const float newx = this->x + other_quaternion.x;
  const float newy = this->y + other_quaternion.y;
  const float newz = this->z + other_quaternion.z;
  const float neww = this->w + other_quaternion.w;
  quaternion new_vector(newx, newy, newz, neww);

  // Return a reference of new quaternion
  return new_vector;
}

const quaternion quaternion::operator- (const quaternion& other_quaternion) const {
  // Create new x, y, z and w by subtracting each of vectors' attributes,
  // without receiving real-time updates from any operand
  // and join them into a new 2D quaternion
  const float newx = this->x - other_quaternion.x;
  const float newy = this->y - other_quaternion.y;
  const float newz = this->z - other_quaternion.z;
  const float neww = this->w - other_quaternion.w;
  quaternion new_vector(newx, newy, newz, neww);

  // Return a reference of new quaternion
  return new_vector;
}

const quaternion quaternion::operator* (const float& scalar) const {
  // Create new x, y, z and w by multiplying each of quaternion's attribute with scalar,
  // without receiving real-time updates from any operand
  // and join them into a new 2D quaternion
  const float newx = this->x * scalar;
  const float newy = this->y * scalar;
  const float newz = this->z * scalar;
  const float neww = this->w * scalar;
  quaternion new_vector(newx, newy, newz, neww);

  // Return a reference of new quaternion
  return new_vector;
}

const quaternion quaternion::operator*(const int& scalar) const {
  // Operator * overload
  return *this * (float)scalar;
}

const quaternion quaternion::operator*(const quaternion& qo) const {
  // Operator * overload
  return quaternion(
    this->w * qo.x + this->x * qo.w + this->y * qo.z - this->z * qo.y,
    this->w * qo.y + this->y * qo.w + this->z * qo.x - this->x * qo.z,
    this->w * qo.z + this->z * qo.w + this->x * qo.y - this->y * qo.x,
    this->w * qo.w - this->x * qo.x - this->y * qo.y - this->z * qo.z
  );
}

const vector_3d quaternion::operator*(const vector_3d& other_vector) const {
  // Operator * overload
  vector_3d vector(this->x, this->y, this->z);
  vector_3d crossed = vector.CrossProduct(other_vector);
  vector_3d double_crossed = vector.CrossProduct(crossed);
  return other_vector + (crossed * this->w + double_crossed) * 2;
}

const quaternion quaternion::operator/ (const float& scalar) const {
  // Create new x, y, z and w by dividing each of quaternion's attribute with scalar,
  // without receiving real-time updates from any operand
  // and join them into a new 2D quaternion
  const float newx = this->x / scalar;
  const float newy = this->y / scalar;
  const float newz = this->z / scalar;
  const float neww = this->w / scalar;
  quaternion new_vector(newx, newy, newz, neww);

  // Return a reference of new quaternion
  return new_vector;
}

const quaternion quaternion::operator/(const int& scalar) const {
  // Operator / overload
  return *this / (float)scalar;
}

const float quaternion::operator[](const int& index) const {
  // Check a copy of index
  switch (index) {
    // Return a copy of private attribute x,
    // corresponding to the first element of the quaternion
  case 0:
    return x;
    break;
    // Return a copy of private attribute y,
    // corresponding to the second element of the quaternion
  case 1:
    return y;
    break;
    // Return a copy of private attribute z,
    // corresponding to the third element of the quaternion
  case 2:
    return z;
    break;
    // Return a copy of private attribute w,
    // corresponding to the forth element of the quaternion
  case 3:
    return w;
    break;
    // Throw an out of range error if index
    // does not correspond with any element of the quaternion
  default:
    throw std::out_of_range("vector_2d::operator[](): index is out of range");
    break;
  }
}

const float quaternion::Angle() const {
  quaternion quaternion = this->Normalize();
  return 2.0f * acos(quaternion.w) * (float)RADIANS_TO_DEGREES;
}

const vector_3d quaternion::Axis() const {
  quaternion quaternion = this->Normalize();
  float s = sqrt(1.0f - quaternion.w * quaternion.w);
  if (s < threshold) {
    return vector_3d(
      1.0f,
      0.0f,
      0.0f
    );
  }
  else {
    return vector_3d(
      quaternion.x / s,
      quaternion.y / s,
      quaternion.z / s
    );
  }
}

const void quaternion::Clean() {
  if (fabs(this->x) < threshold) this->x = 0.0f;
  if (fabs(this->y) < threshold) this->y = 0.0f;
  if (fabs(this->z) < threshold) this->z = 0.0f;
  if (fabs(this->w) < threshold) this->w = 0.0f;
}

const float quaternion::Quadrance(const quaternion& other_quaternion) const {
  // Calculate quadrance
  float calculated_quadrance = powf((other_quaternion.x - this->x), 2.0f) +
    powf((other_quaternion.y - this->y), 2.0f) +
    powf((other_quaternion.z - this->z), 2.0f) +
    powf((other_quaternion.w - this->w), 2.0f);

  // Return a reference of calculated quadrance
  return calculated_quadrance;
}

const float quaternion::Quadrance() const {
  // Calculate quadrance
  float calculated_quadrance =
    powf(this->x, 2.0f) +
    powf(this->y, 2.0f) +
    powf(this->z, 2.0f) +
    powf(this->w, 2.0f);

  // Return a reference of calculated quadrance
  return calculated_quadrance;
}

const float quaternion::Norm() const {
  // Calculate norm
  float calculated_norm = sqrtf(this->Quadrance());

  // Return a reference of calculated norm
  return calculated_norm;
}

const quaternion quaternion::Normalize() const {
  // Obtain calculated norm
  float calculated_norm = this->Norm();

  // Calculate normalized quaternion
  quaternion calculated_normalized_vector = *this / calculated_norm;

  // Return a reference of calculated normalized quaternion
  return calculated_normalized_vector;
}

const quaternion quaternion::Conjugate() const {
  return quaternion(-this->x, -this->y, -this->z, this->w);
}

const quaternion quaternion::Inverse() const {
  return this->Conjugate() / this->Quadrance();
}

const quaternion quaternion::Lerp(const quaternion& qo, float k) const {
  float cos_angle = this->x*qo.x + this->y*qo.y + this->z*qo.z + this->w*qo.w;
  float k0 = 1.0f - k;
  float k1 = (cos_angle > 0) ? k : -k;
  quaternion qi = *this * k0 + qo * k1;
  return qi.Normalize();
}

const quaternion quaternion::Slerp(const quaternion& qo, float k) const {
  float angle = acos(this->x*qo.x + this->y*qo.y + this->z*qo.z + this->w*qo.w);
  float k0 = sin((1 - k)*angle) / sin(angle);
  float k1 = sin(k*angle) / sin(angle);
  quaternion qi = *this * k0 + qo * k1;
  return qi.Normalize();
}

const quaternion math::operator* (const float& scalar, const quaternion& quaternion) {
  // Operator * overload
  return quaternion * scalar;
}

const quaternion math::operator* (const int& scalar, const quaternion& quaternion) {
  // Operator * overload
  return quaternion * scalar;
}

std::ostream& math::operator<<(std::ostream& os, const quaternion& quaternion) {
  return os << "quat(" << quaternion.x << ", " << quaternion.y << ", " << quaternion.z << ", " << quaternion.w << ")";
}