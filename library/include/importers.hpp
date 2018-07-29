/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef IMPORTERS
#define IMPORTERS

/////////////////////////////////////////////////////////////////////// DEPENDENCIES
#include <cassert>
#include <array>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <deque>
#include <sstream>
#include <GL/glew.h>

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace fs {
  struct mtl {
    std::string filename;
    std::vector<std::string> newmtl;
    std::vector<std::array<GLfloat, 3>> Ka;
    std::vector<std::array<GLfloat, 3>> Kd;
    std::vector<std::array<GLfloat, 3>> Ks;
    std::vector<std::array<GLfloat, 3>> Ke;
    std::vector<GLfloat> Ni;
    std::vector<GLfloat> Ns;
    std::vector<GLfloat> Tr;
    std::vector<GLfloat> d;
    std::vector<GLint> illum;
    std::vector<std::string> map_Kd;

    mtl(std::string);

    void save(std::string);
  };

  struct obj {
    std::string filename;
    std::string mtllib;
    std::vector<std::string> o;
    std::vector<std::array<GLfloat, 3>> v;
    std::vector<std::array<GLfloat, 3>> vn;
    std::vector<std::array<GLfloat, 2>> vt;
    std::vector<std::string> usemtl;
    std::vector<std::vector<std::array<GLint, 3>>> f;
    std::vector<std::string> s;

    obj(std::string);

    void save(std::string);
  };
};

#endif