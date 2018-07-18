#include <gl/physics.hpp>

using namespace gl;

movement<kinematic, linear>::movement(
  glm::vec3 linear,
  float duration)
  : linear(linear)
  , angular(glm::quat())
  , duration(duration)
  , max_speed(10000) {}
movement<kinematic, seek>::movement(
  object target,
  float duration,
  float max_speed)
  : angular(glm::quat())
  , duration(duration)
  , max_speed(max_speed) {
  this->linear = target.position - this->linear;

  if (glm::length(this->linear) > 0)
  {
    this->linear = glm::normalize(this->linear);
    this->linear *= max_speed;
  }
}
movement<dynamic, seek>::movement(
  object target,
  float duration,
  float max_speed,
  float max_acceleration)
  : angular(glm::quat())
  , duration(duration)
  , max_speed(max_speed)
  , max_acceleration(max_acceleration) {
  this->linear = target.velocity - this->linear;

  if (glm::length(this->linear) > 0)
  {
    this->linear = glm::normalize(this->linear);
    this->linear *= max_acceleration;
  }
}
object::object(
  glm::vec3 position,
  glm::quat orientation,
  glm::vec3 velocity,
  glm::quat rotation)
  : position(position)
  , orientation(orientation)
  , velocity(velocity)
  , rotation(rotation) {}

template<behaviour type>
object& object::operator+=(movement<kinematic, type> movement) {
  // Integrate with velocity
  this->position += movement.linear * movement.duration;
  this->orientation *= std::pow(movement.angular, movement.duration);

  // Set orientation from velocity
  auto length = glm::length(movement.linear);
  if (length * length > 0.1) {
    // Convert vector to orientation
    this->orientation = std::atan2(movement.linear.x, movement.linear.z);
  }

  return (*this);
}
template object& object::operator+=(movement<kinematic, linear>);
template<behaviour type>
object& object::operator+=(movement<dynamic, type> movement) {
  // Integrate with velocity
  this->position += this->velocity * movement.duration;
  this->orientation *= std::pow(this->rotation, movement.duration);

  // Integrate with acceleration
  this->velocity += movement.linear * movement.duration;
  this->rotation *= std::pow(movement.angular, movement.duration);

  // Set orientation from velocity
  auto length = glm::length(this->velocity);
  if (length * length > 0.1) {
    // Convert vector to orientation
    this->orientation = std::atan2(this->velocity.x, this->velocity.z);
  }

  // Trim max speed
  if (length > movement.max_speed) {
    this->velocity = glm::normalize(this->velocity);
    this->velocity *= movement.max_speed;
  }

  return (*this);
}
