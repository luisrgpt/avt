/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef GRAPHICS
#define GRAPHICS

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include "importers.hpp"
#include "shaders.hpp"
#include "camera.hpp"
#include "graph_scene.hpp"

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

  struct program {
    const unsigned id;
  };
  struct uniform {
    const int id;
  };
  struct uniform_block {
    const unsigned id;
  };
  typedef std::vector<math::vector_3d> illumination;

  class engine {
  private:
    boost::container::vector<bool> window_ids;
    boost::container::vector<bool> program_ids;
    std::vector<std::map<shader_type, std::string>> program_filenames;
    std::vector<std::map<shader_type, unsigned>> shader_ids;
    boost::container::vector<bool> mesh_ids;
    std::vector<std::vector<unsigned>> vbo_ids;
    unsigned current_mesh_id;
    unsigned current_program_id;
    int window_id;

    void set_blinn_phong(
      mesh,
      illumination,
      math::matrix_4d,  //projection
      math::matrix_4d,  //view
      math::matrix_4d,  //translation
      math::matrix_4d,  //rotation
      math::matrix_4d   //scalation
    );
    float width, height;

  public:

    engine(int*, char**, std::string, unsigned, unsigned);
    ~engine();

    static scene light;

    bool left_button_is_down(int, int);
    bool left_button_is_up(int, int);
    bool right_button_is_down(int, int);
    bool right_button_is_up(int, int);

    int get_width();
    int get_height();

    void set_close_callback(void(*)(void));
    void set_display_callback(void(*)(void));
    void set_idle_callback(void(*)(void));
    void set_reshape_callback(void(*)(int, int));
    void set_keyboard_callback(void(*)(unsigned char, int, int));
    void set_motion_callback(void(*)(int, int));
    void set_mouse_callback(void(*)(int, int, int, int));
    void set_timer_callback(const unsigned, void(*)(int), const int);
    void set_title(std::string);

    void reshape(float, float);

    void start();
    void display();

    program* create_program();
    template<shader_type type>
    void load_shader(program, std::string);
    uniform* get_uniform(program, std::string);
    uniform_block* get_uniform_block(program, std::string);
    void link(program);

    template<class vertex, class fragment>
    scene* load_scene(program, fs::obj, fs::mtl);

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