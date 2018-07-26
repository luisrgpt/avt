/////////////////////////////////////////////////////////////////////// HEADER GUARDS
#pragma once
#ifndef LOADERS
#define LOADERS

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

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace loader {
  struct mtl {
    std::string filename;
    std::vector<std::string> newmtl;
    std::vector<std::array<float, 3>> Ka;
    std::vector<std::array<float, 3>> Kd;
    std::vector<std::array<float, 3>> Ks;
    std::vector<std::array<float, 3>> Ke;
    std::vector<float> Ni;
    std::vector<float> d;
    std::vector<int> illum;
    mtl(std::string);
  };

  struct obj {
    std::string filename;
    std::string mtllib;
    std::vector<std::string> o;
    std::vector<std::array<float, 3>> v;
    std::vector<std::array<float, 3>> vn;
    std::vector<std::array<float, 2>> vt;
    std::vector<std::string> usemtl;
    std::vector<std::vector<std::array<int, 3>>> f;
    obj(std::string);
  };
};

#endif