#include "loaders.hpp"

using namespace loader;

mtl::mtl(std::string filename)
  : filename(filename)
  , newmtl(std::vector<std::string>())
  , Ka(std::vector<std::array<float, 3>>())
  , Kd(std::vector<std::array<float, 3>>())
  , Ks(std::vector<std::array<float, 3>>())
  , Ke(std::vector<std::array<float, 3>>())
  , Ni(std::vector<float>())
  , d(std::vector<float>())
  , illum(std::vector<int>())
{
  std::ifstream file(filename);
  std::string line;
  std::string token;
  std::deque<std::string> tokens;
  std::string prefix;
  while (std::getline(file, line)) {
    auto sin = std::stringstream(line);
    tokens.clear();
    while (std::getline(sin, token, ' ')) {
      tokens.push_back(token);
    }
    if (tokens.size() == 0) {
      continue;
    }
    prefix = tokens.front();
    tokens.pop_front();
    if (prefix.compare("newmtl") == 0) {
      this->newmtl.push_back(
        tokens[0]
      );
    }
    else if (prefix.compare("Ka") == 0) {
      this->Ka.push_back({
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
      });
    }
    else if(prefix.compare("Kd") == 0) {
      this->Kd.push_back({
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
        });
    }
    else if(prefix.compare("Ks") == 0) {
      this->Ks.push_back({
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
      });
    }
    else if (prefix.compare("Ke") == 0) {
      this->Ke.push_back({
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
      });
    }
    else if (prefix.compare("Ni") == 0) {
      this->Ni.push_back(
        std::stof(tokens[0])
      );
    }
    else if (prefix.compare("d") == 0) {
      this->Ni.push_back(
        std::stof(tokens[0])
      );
    }
    else if (prefix.compare("illum") == 0) {
      this->Ni.push_back(
        std::stoi(tokens[0])
      );
    }
  }
}

std::array<int, 3> parse_obj_f(std::string token)
{
  std::stringstream ss = std::stringstream(token);
  std::array<int, 3> f = std::array<int, 3>();
  std::getline(ss, token, '/');
  if (token.size() > 0) {
    f[0] = std::stoi(token);
  }
  else {
    f[0] = 0;
  }
  std::getline(ss, token, '/');
  if (token.size() > 0) {
    f[1] = std::stoi(token);
  }
  else {
    f[1] = 0;
  }
  std::getline(ss, token, ' ');
  if (token.size() > 0) {
    f[2] = std::stoi(token);
  }
  else {
    f[2] = 0;
  }
  return f;
}

obj::obj(std::string filename)
  : filename(filename)
  , o(std::vector<std::string>())
  , v(std::vector<std::array<float, 3>>())
  , vn(std::vector<std::array<float, 3>>())
  , vt(std::vector<std::array<float, 2>>())
  , usemtl(std::vector<std::string>())
  , f(std::vector<std::vector<std::array<int, 3>>>())
{
  std::ifstream file(filename);
  std::string line;
  std::string token;
  std::deque<std::string> tokens;
  std::string prefix;
  int n = -1;
  while (std::getline(file, line)) {
    auto sin = std::stringstream(line);
    tokens.clear();
    while (std::getline(sin, token, ' ')) {
      tokens.push_back(token);
    }
    if (tokens.size() == 0) {
      continue;
    }
    prefix = tokens.front();
    tokens.pop_front();
    if (prefix.compare("mtllib") == 0) {
      this->mtllib = tokens[0];
    }
    else if (prefix.compare("o") == 0) {
      this->o.push_back(
        tokens[0]
      );
      this->f.push_back(std::vector<std::array<int, 3>>());
      n += 1;
    }
    else if (prefix.compare("usemtl") == 0) {
      this->usemtl.push_back(
        tokens[0]
      );
    }
    else if (prefix.compare("v") == 0) {
      this->v.push_back({
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
      });
    }
    else if (prefix.compare("vn") == 0) {
      this->vn.push_back({
        std::stof(tokens[0]),
        std::stof(tokens[1]),
        std::stof(tokens[2])
      });
    }
    else if (prefix.compare("vt") == 0) {
      this->vt.push_back({
        std::stof(tokens[0]),
        std::stof(tokens[1])
      });
    }
    else if (prefix.compare("f") == 0) {
      for (auto i = 0u; i < 3u; i++) {
        this->f[n].push_back(parse_obj_f(tokens[i]));
      }
      if (tokens.size() == 4) {
        this->f[n].push_back(parse_obj_f(tokens[2]));
        this->f[n].push_back(parse_obj_f(tokens[3]));
        this->f[n].push_back(parse_obj_f(tokens[0]));
      }
    }
  }
}
