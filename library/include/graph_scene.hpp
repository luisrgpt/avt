/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef SCENE
#define SCENE

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <vector>
#include <optional>

#include "physics.hpp"
#include "shaders.hpp"

namespace gl {
  struct mesh {
    bool is_light;
    vertex_type vertex_type;
    fragment_type fragment_type;

    std::string vertex_file;
    std::string fragment_file;
    std::string object_file;
    std::string material_file;

    unsigned id;
    size_t n_faces;
    unsigned program_id;
    unsigned texture_id;
    unsigned block_id;
    unsigned vertex;
    unsigned fragment;

    bool has_material;
    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;
    std::array<float, 3> emissive;
    float shininess;
    int n_textures;
  };
  typedef std::vector<mesh> scene;
  scene light = { mesh{ true } };

  class model {
  public:
    scene scene_info;
    object object;
    math::matrix_4d scale;

    model(scene, math::vector_3d, math::vector_3d, math::quaternion, math::vector_3d, math::quaternion);

    void execute(kinematic_movement);
    //void execute(dynamic_movement);
  };

  struct node {
    unsigned id;
  };

  template<class type>
  class graph {
  private:
    node root;

  public:
    unsigned counter;
    std::vector<std::optional<type>> nodes;
    std::vector<node> edges;
    graph();

    node* set_root(type);
    node* set_child(node, type);
    type& get(node);
  };

  template class graph<model>;

  class bltz {
  public:
    static graph<model>* load(std::string);
    static void save(graph<model>, std::string);
  };
};


#endif