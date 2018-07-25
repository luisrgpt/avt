#include <physics.hpp>
#include <iostream>

using namespace gl;

kinematic_movement::kinematic_movement(
  math::vector_3d linear,
  math::quaternion angular,
  float duration,
  float max_speed)
  : linear(linear)
  , angular(angular)
  , duration(duration)
  , max_speed(max_speed) {}

linear_movement::linear_movement(
  math::vector_3d linear,
  float duration)
  : kinematic_movement(linear, math::quaternion(0.0f, math::vector_3d(0.0f, 1.0f, 0.0f)), duration, 10000) {}
rotation::rotation(
  math::quaternion angular,
  float duration)
  : kinematic_movement(math::vector_3d(), angular, duration, 10000) {}
//kinematic_seek::kinematic_seek(
//  object target,
//  float duration,
//  float max_speed)
//  : angular(math::quaternion())
//  , duration(duration)
//  , max_speed(max_speed) {
//  this->linear = target.position - this->linear;
//
//  if (glm::length(this->linear) > 0)
//  {
//    this->linear = glm::normalize(this->linear);
//    this->linear *= max_speed;
//  }
//}
//movement<dynamic, seek>::movement(
//  object target,
//  float duration,
//  float max_speed,
//  float max_acceleration)
//  : angular(math::quaternion())
//  , duration(duration)
//  , max_speed(max_speed)
//  , max_acceleration(max_acceleration) {
//  this->linear = target.velocity - this->linear;
//
//  if (glm::length(this->linear) > 0)
//  {
//    this->linear = glm::normalize(this->linear);
//    this->linear *= max_acceleration;
//  }
//}
object::object(
  math::vector_3d position,
  math::quaternion orientation,
  math::vector_3d velocity,
  math::quaternion rotation)
  : position(position)
  , orientation(orientation)
  , velocity(velocity)
  , rotation(rotation) {}

void object::execute(kinematic_movement movement) {
  // Integrate with velocity
  this->position += movement.linear * movement.duration;
  for (auto count = 0u; count < movement.duration; count++) {
    //std::cout << (this->orientation).x << " " << (this->orientation).y << " " << (this->orientation).z << " " << (this->orientation).w;
    this->orientation = movement.angular * this->orientation;
    //std::cout << " -> " << (this->orientation).x << " " << (this->orientation).y << " " << (this->orientation).z << " " << (this->orientation).w << std::endl;
  }

  // Set orientation from velocity
  //auto length = glm::length(movement.linear);
  //if (length * length > 0.1) {
  //  // Convert vector to orientation
  //  this->orientation = std::atan2(movement.linear.x, movement.linear.z);
  //}
}
//void object::execute(dynamic_movement movement) {
//  // Integrate with velocity
//  this->position += this->velocity * movement.duration;
//  this->orientation *= std::pow(this->rotation, movement.duration);
//
//  // Integrate with acceleration
//  this->velocity += movement.linear * movement.duration;
//  this->rotation *= std::pow(movement.angular, movement.duration);
//
//  // Set orientation from velocity
//  auto length = glm::length(this->velocity);
//  if (length * length > 0.1) {
//    // Convert vector to orientation
//    this->orientation = std::atan2(this->velocity.x, this->velocity.z);
//  }
//
//  // Trim max speed
//  if (length > movement.max_speed) {
//    this->velocity = glm::normalize(this->velocity);
//    this->velocity *= movement.max_speed;
//  }
//}
