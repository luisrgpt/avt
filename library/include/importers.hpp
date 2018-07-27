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

/////////////////////////////////////////////////////////////////////// NAMESPACE
namespace fs {
  struct mtl {
    std::string filename;
    std::vector<std::string> newmtl;
    std::vector<std::array<float, 3>> Ka;
    std::vector<std::array<float, 3>> Kd;
    std::vector<std::array<float, 3>> Ks;
    std::vector<std::array<float, 3>> Ke;
    std::vector<float> Ni;
    std::vector<float> Ns;
    std::vector<float> Tr;
    std::vector<float> d;
    std::vector<int> illum;
    std::vector<std::string> map_Kd;

    mtl(std::string);

    void save(std::string);
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
    std::vector<std::string> s;

    obj(std::string);

    void save(std::string);
  };
};

#endif