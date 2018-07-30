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
    v_particles
  };

  enum fragment_type {
    f_none,
    f_flat,
    f_blinn_phong_fragment,
    f_particles
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
    GLfloat shininess;
    GLint tex_count;
  };

  struct point_light_vertex {
    std::array<GLfloat, 16> model;
    std::array<GLfloat, 16> view;
    std::array<GLfloat, 16> projection;
    std::array<GLfloat, 16> m_normal;
    std::array<GLfloat, 3> light_position;
    GLint mode;
  };
  struct point_light_fragment {
    std::array<GLfloat, 3> Ka;
    std::array<GLfloat, 3> Kd;
    std::array<GLfloat, 3> Ks;
    std::array<GLfloat, 3> Ke;
    GLfloat shininess;
    GLint tex_count;
    GLint mode;
  };

  struct blinn_phong_vertex {
  };
  struct blinn_phong_fragment {
  };

  struct particles_vertex {
  };
  struct particles_fragment {};
};

#endif