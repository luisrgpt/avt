#pragma once

#include <boost/container/vector.hpp>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <map>

#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/detail/type_vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_transform_2d.hpp"
#include "glm/gtx/transform.hpp"

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
    glm::vec3 position;
    glm::quat orientation;

    // Kinematic data
    glm::vec3 velocity;
    glm::quat rotation;

    object(
      glm::vec3,
      glm::quat,
      glm::vec3,
      glm::quat
    );

    void execute(kinematic_movement);
    //void execute(dynamic_movement);
  };


  class kinematic_movement {
  private:
    glm::vec3 linear;
    glm::quat angular;
    float duration;
    float max_speed;

  public:
    kinematic_movement(glm::vec3, glm::quat, float, float);
    friend void object::execute(kinematic_movement);
  };

  //class dynamic_movement {
  //private:
  //  glm::vec3 linear;
  //  glm::quat angular;
  //  float duration;
  //  float max_speed;
  //  float max_acceleration;

  //public:
  //  dynamic_movement(glm::vec3, glm::quat, float, float, float);
  //  friend void object::execute(dynamic_movement);
  //};

  class linear_movement : public kinematic_movement {
  public:
    linear_movement(glm::vec3, float);
  };
  class rotation : public kinematic_movement {
  public:
    rotation(glm::quat, float);
  };
  //class kinematic_seek : public kinematic_movement {
  //public:
  //  kinematic_seek(object, float, float);
  //};
  //class dynamic_seek : public dynamic_movement {
  //  dynamic_seek(object, float, float, float);
  //};
}
