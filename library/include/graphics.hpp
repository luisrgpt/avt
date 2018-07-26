/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef GRAPHICS
#define GRAPHICS

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include "physics.hpp"
#include "loaders.hpp"

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace gl {
  enum shader_type {
    compute,
    vertex,
    geometry,
    tess_control,
    tess_evaluation,
    fragment
  };
  enum projection_type : bool {
    perspective,
    orthogonal
  };

  struct program {
    const unsigned id;
  };
  struct mesh {
    unsigned id;
    size_t n_faces;
    unsigned program_id;
    unsigned texture_id;
    unsigned block_id;
    unsigned mvp_id;
    unsigned material_id;

    bool has_material;
    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;
    std::array<float, 3> emissive;
    float shininess;
    int n_textures;
  };
  struct uniform {
    const int id;
  };
  struct uniform_block {
    const unsigned id;
  };
  struct node {
    const unsigned id;
  };
  typedef std::vector<mesh> scene;

  class model {
  public:
    scene scene_info;
    object object;
    math::matrix_4d scale;

    model(scene, math::vector_3d, math::vector_3d, math::quaternion, math::vector_3d, math::quaternion);

    void execute(kinematic_movement);
    //void execute(dynamic_movement);
  };
  class view {
  public:
    object object;
    math::matrix_4d matrix;

    view(math::vector_3d, math::quaternion, math::vector_3d, math::quaternion);

    void execute(kinematic_movement);
    //void execute(dynamic_movement);
  };
  class free_view {
  public:
    math::vector_3d position;
    math::vector_3d euler_angle;
    math::matrix_4d matrix;

    free_view(math::vector_3d, math::vector_3d);

    void apply_velocity(math::vector_3d);
    void apply_rotation(math::vector_3d);
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

  template<class type>
  class graph {
  private:
    node root;

  public:
    unsigned counter;
    std::vector<type> nodes;
    std::vector<node> edges;
    graph();

    node* set_root(type);
    node* set_child(node, type);
    type& get(node);
  };
  template class graph<model>;

  class engine {
  private:
    boost::container::vector<bool> window_ids;
    boost::container::vector<bool> program_ids;
    std::vector<std::map<shader_type, unsigned>> shader_ids;
    boost::container::vector<bool> mesh_ids;
    std::vector<std::vector<unsigned>> vbo_ids;
    unsigned current_mesh_id;
    unsigned current_program_id;
    float width, height;
    int window_id;

  public:
    engine(int*, char**, std::string, unsigned, unsigned);
    ~engine();

    bool get_left_mouse_state(int, int);

    void set_close_callback(void(*)(void));
    void set_display_callback(void(*)(void));
    void set_idle_callback(void(*)(void));
    void set_reshape_callback(void(*)(int, int));
    void set_keyboard_callback(void(*)(unsigned char, int, int));
    void set_motion_callback(void(*)(int, int));
    void set_mouse_callback(void(*)(int, int, int, int));
    void set_timer_callback(const unsigned, void(*)(int), const int);
    void set_title(std::string);

    void start();
    void display();

    program* create_program();
    template<shader_type type>
    void load_shader(program, std::string);
    uniform* get_uniform(program, std::string);
    uniform_block* get_uniform_block(program, std::string);
    void link(program);

    scene* load_scene(program, loader::obj, loader::mtl, uniform_block, uniform_block);

    void use(program);
    void bind(mesh);

    void set_uniform(uniform, math::matrix_4d);

    void draw(mesh);
    void draw(graph<model>, camera);
    void draw(graph<model>, free_camera);
    void draw(graph<model>, math::matrix_4d, math::matrix_4d);

    void unbind();
    void unuse();
  };
}

#endif