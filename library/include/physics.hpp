/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef PHYSICS
#define PHYSICS

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <boost/container/vector.hpp>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <sstream>

#include "math/matrix_3d.hpp"
#include "math/matrix_4d.hpp"
#include "math/vector_3d.hpp"
#include "math/quaternion.hpp"

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace gl {
  enum behaviour {
    linear,
    seek
  };

  class kinematic_movement;
  class dynamic_movement;
  class object {
  public:
    // Static data
    math::vector_3d position;
    math::quaternion orientation;

    // Kinematic data
    math::vector_3d velocity;
    math::quaternion rotation;

    object(
      math::vector_3d,
      math::quaternion,
      math::vector_3d,
      math::quaternion
    );

    void execute(kinematic_movement);
    //void execute(dynamic_movement);
  };


  class kinematic_movement {
  private:
    math::vector_3d linear;
    math::quaternion angular;
    float duration;
    float max_speed;

  public:
    kinematic_movement(math::vector_3d, math::quaternion, float, float);
    friend void object::execute(kinematic_movement);
  };

  //class dynamic_movement {
  //private:
  //  math::vector_3d linear;
  //  math::quaternion angular;
  //  float duration;
  //  float max_speed;
  //  float max_acceleration;

  //public:
  //  dynamic_movement(math::vector_3d, math::quaternion, float, float, float);
  //  friend void object::execute(dynamic_movement);
  //};

  class linear_movement : public kinematic_movement {
  public:
    linear_movement(math::vector_3d, float);
  };
  class rotation : public kinematic_movement {
  public:
    rotation(math::quaternion, float);
  };
  //class kinematic_seek : public kinematic_movement {
  //public:
  //  kinematic_seek(object, float, float);
  //};
  //class dynamic_seek : public dynamic_movement {
  //  dynamic_seek(object, float, float, float);
  //};
}

#endif