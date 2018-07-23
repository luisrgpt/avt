#pragma once

#include "physics.hpp"

namespace gl {
  enum shader_type {
    compute,
    vertex,
    geometry,
    tess_control,
    tess_evaluation,
    fragment
  };
  enum scene_type {
    gltf,
    obj
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
    unsigned n_faces;
    unsigned program_id;
    unsigned texture_id;
    unsigned block_id;
  };
  struct material {
    float diffuse[4];
    float ambient[4];
    float specular[4];
    float emissive[4];
    float shininess;
    int n_textures;
  };
  struct uniform {
    const int id;
  };
  struct uniform_block {
    const int id;
  };
  struct node {
    const unsigned id;
  };
  typedef std::vector<mesh> scene;

  //template struct shader<compute>;
  //template struct shader<vertex>;
  //template struct shader<geometry>;
  //template struct shader<tess_control>;
  //template struct shader<tess_evaluation>;
  //template struct shader<fragment>;

  class model {
  public:
    scene scene_info;
    object object;
    glm::mat4 scale;

    model(scene, glm::vec3, glm::vec3, glm::quat, glm::vec3, glm::quat);

    void execute(kinematic_movement);
    //void execute(dynamic_movement);
  };
  class view {
  public:
    object object;
    glm::mat4 matrix;

    view(glm::vec3, glm::quat, glm::vec3, glm::quat);

    void execute(kinematic_movement);
    //void execute(dynamic_movement);
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
    projection(projection_type, float, float, float, float, float, float, glm::mat4);

  public:
    glm::mat4 matrix;

    void reshape(float, float);
  };
  class orthogonal_projection : public projection {
  public:
    orthogonal_projection(float, float, float, float, float, float, float, float);
  };
  class perspective_projection : public projection {
  public:
    perspective_projection(float, float, float, float, float, float, float, float);
  };

  struct camera {
    view view;
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
    unsigned current_mesh_id;
    unsigned current_program_id;
    float width, height;
    int window_id;

  public:
    engine(int*, char**, std::string, unsigned, unsigned);

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
    //uniform_block* get_uniform_block(program, std::string);
    void link(program);

    template<scene_type type>
    scene* load_scene(program, std::string);

    void use(program);
    void bind(mesh);

    void set_uniform(uniform, glm::mat4);

    void draw(mesh);
    void draw(uniform, graph<model>, camera);

    void unbind();
    void unuse();

    //void unload_shader(program, std::string);
    //void destroy_program();
  };
}
