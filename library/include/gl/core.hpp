#pragma once

#include "extra.hpp"

namespace gl {
  namespace callback {
    struct close {
      void(*handler)(void);
    };
    struct display {
      void(*handler)(void);
    };
    struct idle {
      void(*handler)(void);
    };
    struct reshape {
      void(*handler)(int, int);
    };
    struct keyboard {
      void(*handler)(unsigned char, int, int);
    };
    struct motion {
      void(*handler)(int, int);
    };
    struct mouse {
      void(*handler)(int, int, int, int);
    };
    struct timer {
      const unsigned time;
      void(*handler)(int);
      const int value;
    };
  }
  enum shader_type {
    compute,
    vertex,
    geometry,
    tess_control,
    tess_evaluation,
    fragment
  };

  struct title {
    const char* name;
  };
  template<shader_type type> struct shader {
    const std::string filename;
  };
  //template struct shader<compute>;
  //template struct shader<vertex>;
  //template struct shader<geometry>;
  //template struct shader<tess_control>;
  //template struct shader<tess_evaluation>;
  //template struct shader<fragment>;

  class engine;
  class program;
  class model;
  class view;
  class projection;
  class window;

  class engine {
    private:
      std::vector<bool> window_ids;
      std::vector<bool> program_ids;
      std::vector<bool> model_ids;
      unsigned id_counter;

    public:
      engine();

      engine& operator+=(window);
      engine& operator+=(program);
      engine& operator+=(model);
      engine& operator+=(glm::mat4);
      engine& operator-=(model);
      engine& operator-=(program);
  };

  class program {
    private:
      unsigned id;
      std::vector<unsigned> shader_ids;

    public:
      program();

      template<shader_type type>
      program& operator+=(shader<type>);
      friend engine& engine::operator+=(program);
      friend engine& engine::operator-=(program);
  };

  class model {
  private:
    std::string filename;
    glm::mat4 scale;
    object object;

    unsigned id;
    std::vector<unsigned> buffer_ids;
    unsigned id_counter;

  public:
    glm::mat4 matrix;

    model(std::string, glm::vec3, glm::vec3, glm::quat, glm::vec3, glm::quat);
    model(std::string, glm::vec3, glm::vec3, glm::quat, glm::vec3);
    model(std::string, glm::vec3, glm::vec3, glm::quat);
    model(std::string, glm::vec3, glm::vec3);
    model(std::string, glm::vec3);
    model(std::string);

    template<mechanic m, behaviour b>
    model& operator+=(movement<m, b>);

    friend engine& engine::operator+=(model);
    friend engine& engine::operator-=(model);
  };
  class view {
  private:
    object object;

  public:
    glm::mat4 matrix;

    view(glm::vec3, glm::quat, glm::vec3, glm::quat);
    view(glm::vec3, glm::quat, glm::vec3);
    view(glm::vec3, glm::quat);
    view(glm::vec3);
    view();

    template<mechanic m, behaviour b>
    view& operator+=(movement<m, b>);
  };
  class projection {
  private:
    float fov;
    float aspect;
    float z_near;
    float z_far;

  public:
    glm::mat4 matrix;

    projection(float, float, float, float);
    projection(float, float, float, float, float, float);

    projection& operator+=(window);
  };

  class window {
  private:
    int id;

  public:
    int width, height;

    window();

    window& operator+=(callback::close);
    window& operator+=(callback::display);
    window& operator+=(callback::idle);
    window& operator+=(callback::reshape);
    window& operator+=(callback::keyboard);
    window& operator+=(callback::motion);
    window& operator+=(callback::mouse);
    window& operator+=(callback::timer);
    window& operator+=(title);

    friend engine& engine::operator+=(window);
    friend projection& projection::operator+=(window);
  };
}
