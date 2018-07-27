/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef CAMERA
#define CAMERA

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include "physics.hpp"

namespace gl {
  enum projection_type : bool {
    perspective,
    orthogonal
  };

  class view {
  private:
    object initial_object;
    math::matrix_4d initial_matrix;

  public:
    object object;
    math::matrix_4d matrix;

    view(math::vector_3d, math::quaternion, math::vector_3d, math::quaternion);

    void execute(kinematic_movement);
    //void execute(dynamic_movement);

    void reset();
  };
  class free_view {
  private:
    math::vector_3d initial_position;
    math::vector_3d initial_euler_angle;
    math::matrix_4d initial_matrix;

  public:
    math::vector_3d position;
    math::vector_3d euler_angle;
    math::matrix_4d matrix;

    free_view(math::vector_3d, math::vector_3d);

    void apply_velocity(math::vector_3d);
    void apply_rotation(math::vector_3d);

    void reset();
  };

  class projection {
  private:
    projection_type type;
    float left;
    float right;
    float bottom;
    float top;
    float z_near;
    float z_far;

  protected:
    projection(projection_type, float, float, float, float, float, float, math::matrix_4d);

  public:
    math::matrix_4d matrix;

    void reshape(float, float);
  };
  class orthogonal_projection : public projection {
  public:
    orthogonal_projection(float, float, float, float, float, float, float, float);
  };
  class perspective_projection : public projection {
  public:
    perspective_projection(float, float, float, float, float);
  };

  struct camera {
    view view;
    projection projection;
  };
  struct free_camera {
    free_view view;
    projection projection;
  };
};

#endif