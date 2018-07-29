/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef SHADERS
#define SHADERS

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <array>
#include <GL/glew.h>

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace gl {
  enum vertex_type {
    v_none,
    v_flat,
    v_blinn_phong_vertex,
  };

  enum fragment_type {
    f_none,
    f_flat,
    f_blinn_phong_fragment
  };

  struct none {};
  struct flat_vertex {
    std::array<GLfloat, 16> model;
    std::array<GLfloat, 16> view;
    std::array<GLfloat, 16> projection;
    std::array<GLfloat, 9> m_normal;
    std::array<GLfloat, 3> light_position;
  };
  struct flat_fragment {
    std::array<GLfloat, 3> Ka;
    std::array<GLfloat, 3> Kd;
    std::array<GLfloat, 3> Ks;
    std::array<GLfloat, 3> Ke;
    float shininess;
    int tex_count;
  };
  struct blinn_phong_vertex {
    std::array<GLfloat, 16> model;
    std::array<GLfloat, 16> view;
    std::array<GLfloat, 16> projection;
    std::array<GLfloat, 9> m_normal;
    std::array<GLfloat, 3> light_position;
  };
  struct blinn_phong_fragment {
    std::array<GLfloat, 3> Ka;
    std::array<GLfloat, 3> Kd;
    std::array<GLfloat, 3> Ks;
    std::array<GLfloat, 3> Ke;
    GLfloat shininess;
    GLint tex_count;
  };
};

#endif