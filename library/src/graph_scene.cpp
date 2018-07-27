#include "graph_scene.hpp"

using namespace gl;

model::model(
  scene scene,
  math::vector_3d scale,
  math::vector_3d position,
  math::quaternion orientation,
  math::vector_3d velocity,
  math::quaternion rotation)
  : scene_info(scene)
  , object(position, orientation, velocity, rotation)
  , scale(math::matrix_4d::Scaling(scale.x, scale.y, scale.z, 1.0f)) {
  //std::cout << "quat: " << std::endl;
  //std::cout << this->object.orientation.x << " " << this->object.orientation.y << " " << this->object.orientation.z << " " << this->object.orientation.w << " " << std::endl;
  //std::cout << std::endl;
  //std::cout << "scale: " << std::endl;
  //std::cout << this->scale[0][0] << " " << this->scale[0][1] << " " << this->scale[0][2] << " " << this->scale[0][3] << " " << std::endl;
  //std::cout << this->scale[1][0] << " " << this->scale[1][1] << " " << this->scale[1][2] << " " << this->scale[1][3] << " " << std::endl;
  //std::cout << this->scale[2][0] << " " << this->scale[2][1] << " " << this->scale[2][2] << " " << this->scale[2][3] << " " << std::endl;
  //std::cout << this->scale[3][0] << " " << this->scale[3][1] << " " << this->scale[3][2] << " " << this->scale[3][3] << " " << std::endl;
  //std::cout << std::endl;
}

void model::execute(kinematic_movement movement) {
  this->object.execute(movement);
}

template<class type>
graph<type>::graph()
  : root(node{ 0u })
  , counter(1u)
  , nodes({ std::optional<type>() })
  , edges({ this->root }) {}

template<class type>
node* graph<type>::set_root(type content) {
  node* root = new node{ this->counter++ };
  this->nodes.push_back(std::optional(content));
  this->edges.push_back(this->root);

  return root;
}
template<class type>
node* graph<type>::set_child(node parent, type content) {
  node* child = new node{ this->counter++ };
  this->nodes.push_back(content);
  this->edges.push_back(parent);

  return child;
}
template<class type>
type& graph<type>::get(node node) {
  return nodes[node.id].value();
}

template gl::graph<gl::model>::graph();
template gl::node* gl::graph<gl::model>::set_root(gl::model);
template gl::node* gl::graph<gl::model>::set_child(gl::node, gl::model);
template gl::model& gl::graph<gl::model>::get(gl::node);