#include "camera.hpp"

using namespace gl;

view::view(
  math::vector_3d position,
  math::quaternion orientation,
  math::vector_3d velocity,
  math::quaternion rotation)
  : initial_object(position, orientation, velocity, rotation)
  , initial_matrix(math::matrix_4d::View(position, position + orientation * math::vector_3d(0.0f, 0.0f, -1.0f), orientation * math::vector_3d(0.0f, 1.0f, 0.0f)))
  , object(position, orientation, velocity, rotation)
  , matrix(math::matrix_4d::View(position, position + orientation * math::vector_3d(0.0f, 0.0f, -1.0f), orientation * math::vector_3d(0.0f, 1.0f, 0.0f))) {
  //std::cout << "lookat: " << std::endl;
  //std::cout << this->matrix[0][0] << " " << this->matrix[0][1] << " " << this->matrix[0][2] << " " << this->matrix[0][3] << " " << std::endl;
  //std::cout << this->matrix[1][0] << " " << this->matrix[1][1] << " " << this->matrix[1][2] << " " << this->matrix[1][3] << " " << std::endl;
  //std::cout << this->matrix[2][0] << " " << this->matrix[2][1] << " " << this->matrix[2][2] << " " << this->matrix[2][3] << " " << std::endl;
  //std::cout << this->matrix[3][0] << " " << this->matrix[3][1] << " " << this->matrix[3][2] << " " << this->matrix[3][3] << " " << std::endl;
  //std::cout << std::endl;
}
free_view::free_view(math::vector_3d position, math::vector_3d euler_angle)
  : initial_position(position)
  , initial_euler_angle(euler_angle)
  , initial_matrix(math::matrix_4d::kZero)
  , position(position)
  , euler_angle(euler_angle)
  , matrix(math::matrix_4d::kZero) {
  math::vector_3d eye = math::vector_3d(0.0f, 0.0f, -1.0f) * math::matrix_3d::Rotation(euler_angle.x, euler_angle.y, euler_angle.z);
  math::vector_3d up = math::vector_3d(0.0f, 1.0f, 0.0f) * math::matrix_3d::Rotation(euler_angle.x, euler_angle.y, euler_angle.z);

  this->initial_matrix = math::matrix_4d::View(position, position + eye, up);
  this->matrix = this->initial_matrix;
}
projection::projection(
  projection_type type,
  float left,
  float right,
  float bottom,
  float top,
  float z_near,
  float z_far,
  math::matrix_4d matrix)
  : type(type)
  , left(left)
  , right(right)
  , bottom(bottom)
  , top(top)
  , z_near(z_near)
  , z_far(z_far)
  , matrix(matrix) {}
orthogonal_projection::orthogonal_projection(
  float width, float height,
  float left, float right,
  float bottom, float top,
  float z_near, float z_far)
  : projection(orthogonal, left, right, bottom, top, z_near, z_far, math::matrix_4d::OrthographicProjection(left * width / height, right * width / height, bottom, top, z_near, z_far)) {
  //std::cout << "orthogonal: " << std::endl;
  //std::cout << this->matrix[0][0] << " " << this->matrix[0][1] << " " << this->matrix[0][2] << " " << this->matrix[0][3] << " " << std::endl;
  //std::cout << this->matrix[1][0] << " " << this->matrix[1][1] << " " << this->matrix[1][2] << " " << this->matrix[1][3] << " " << std::endl;
  //std::cout << this->matrix[2][0] << " " << this->matrix[2][1] << " " << this->matrix[2][2] << " " << this->matrix[2][3] << " " << std::endl;
  //std::cout << this->matrix[3][0] << " " << this->matrix[3][1] << " " << this->matrix[3][2] << " " << this->matrix[3][3] << " " << std::endl;
  //std::cout << std::endl;
}
perspective_projection::perspective_projection(
  float width, float height,
  float fovy,
  float z_near, float z_far)
  : projection(perspective, fovy, 0.0f, 0.0f, 0.0f, z_near, z_far, math::matrix_4d::PerspectiveProjection(fovy, width / height, z_near, z_far)) {
  //std::cout << "frustum: " << std::endl;
  //std::cout << this->matrix[0][0] << " " << this->matrix[0][1] << " " << this->matrix[0][2] << " " << this->matrix[0][3] << " " << std::endl;
  //std::cout << this->matrix[1][0] << " " << this->matrix[1][1] << " " << this->matrix[1][2] << " " << this->matrix[1][3] << " " << std::endl;
  //std::cout << this->matrix[2][0] << " " << this->matrix[2][1] << " " << this->matrix[2][2] << " " << this->matrix[2][3] << " " << std::endl;
  //std::cout << this->matrix[3][0] << " " << this->matrix[3][1] << " " << this->matrix[3][2] << " " << this->matrix[3][3] << " " << std::endl;
  //std::cout << std::endl;
}

void view::execute(kinematic_movement movement) {
  this->object.execute(movement);
  //std::cout << "before lookat: " << std::endl;
  //std::cout << this->matrix[0][0] << " " << this->matrix[0][1] << " " << this->matrix[0][2] << " " << this->matrix[0][3] << " " << std::endl;
  //std::cout << this->matrix[1][0] << " " << this->matrix[1][1] << " " << this->matrix[1][2] << " " << this->matrix[1][3] << " " << std::endl;
  //std::cout << this->matrix[2][0] << " " << this->matrix[2][1] << " " << this->matrix[2][2] << " " << this->matrix[2][3] << " " << std::endl;
  //std::cout << this->matrix[3][0] << " " << this->matrix[3][1] << " " << this->matrix[3][2] << " " << this->matrix[3][3] << " " << std::endl;
  //std::cout << std::endl;
  this->matrix = math::matrix_4d::View(this->object.position, this->object.position + this->object.orientation * math::vector_3d(0.0f, 0.0f, -1.0f), this->object.orientation * math::vector_3d(0.0f, 1.0f, 0.0f));
  //std::cout << "after lookat: " << std::endl;
  //std::cout << this->matrix[0][0] << " " << this->matrix[0][1] << " " << this->matrix[0][2] << " " << this->matrix[0][3] << " " << std::endl;
  //std::cout << this->matrix[1][0] << " " << this->matrix[1][1] << " " << this->matrix[1][2] << " " << this->matrix[1][3] << " " << std::endl;
  //std::cout << this->matrix[2][0] << " " << this->matrix[2][1] << " " << this->matrix[2][2] << " " << this->matrix[2][3] << " " << std::endl;
  //std::cout << this->matrix[3][0] << " " << this->matrix[3][1] << " " << this->matrix[3][2] << " " << this->matrix[3][3] << " " << std::endl;
  //std::cout << std::endl;
}
void gl::view::reset()
{
  this->object = this->initial_object;
  this->matrix = this->initial_matrix;
}
void free_view::apply_velocity(math::vector_3d velocity) {
  this->position += velocity;

  math::vector_3d eye = math::vector_3d(0.0f, 0.0f, -1.0f) * math::matrix_3d::Rotation(this->euler_angle.x, this->euler_angle.y, this->euler_angle.z);
  math::vector_3d up = math::vector_3d(0.0f, 1.0f, 0.0f) * math::matrix_3d::Rotation(this->euler_angle.x, this->euler_angle.y, this->euler_angle.z);

  this->matrix = math::matrix_4d::View(this->position, this->position + eye, up);
}
void free_view::apply_rotation(math::vector_3d euler_angle) {
  this->euler_angle += euler_angle;

  math::vector_3d eye = math::vector_3d(0.0f, 0.0f, -1.0f) * math::matrix_3d::Rotation(this->euler_angle.x, this->euler_angle.y, this->euler_angle.z);
  math::vector_3d up = math::vector_3d(0.0f, 1.0f, 0.0f) * math::matrix_3d::Rotation(this->euler_angle.x, this->euler_angle.y, this->euler_angle.z);

  this->matrix = math::matrix_4d::View(this->position, this->position + eye, up);
}
void free_view::reset()
{
  this->position = this->initial_position;
  this->euler_angle = this->initial_euler_angle;
  this->matrix = this->initial_matrix;
}

void projection::reshape(float width, float height) {
  float ratio = width / height;
  if (this->type == perspective) {
    this->matrix = math::matrix_4d::PerspectiveProjection(
      this->left, //fovy
      ratio,
      this->z_near, this->z_far
    );
  }
  else {
    this->matrix = math::matrix_4d::OrthographicProjection(
      this->left * ratio, this->right * ratio,
      this->bottom, this->top,
      this->z_near, this->z_far
    );
  }
}
