#pragma once

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

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
  enum mechanic {
    kinematic,
    dynamic
  };
  enum behaviour {
    seek
  };

  template<mechanic m, behaviour b> struct movement {};
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

    template<behaviour type>
    object& operator+=(movement<kinematic, type>);
    template<behaviour type>
    object& operator+=(movement<dynamic, type>);
  };

  template<> class movement<kinematic, seek> {
  public:
    glm::vec3 linear;
    glm::quat angular;
    float duration;
    float max_speed;

    movement(object, float, float);

    template<behaviour type>
    friend object& object::operator+=(movement<kinematic, type>);
  };
  template<> class movement<dynamic, seek> {
  public:
    glm::vec3 linear;
    glm::quat angular;
    float duration;
    float max_speed;
    float max_acceleration;

    movement(object, float, float, float);

    template<behaviour type>
    friend object& object::operator+=(movement<dynamic, type>);
  };
}
