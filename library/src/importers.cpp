#include "importers.hpp"

using namespace fs;

mtl::mtl(std::string filename)
  : filename(filename)
  , newmtl(std::vector<std::string>())
  , Ka(std::vector<std::array<float, 3>>())
  , Kd(std::vector<std::array<float, 3>>())
  , Ks(std::vector<std::array<float, 3>>())
  , Ke(std::vector<std::array<float, 3>>())
  , Ni(std::vector<float>())
  , Ns(std::vector<float>())
  , Tr(std::vector<float>())
  , d(std::vector<float>())
  , illum(std::vector<int>())
  , map_Kd(std::vector<std::string>())
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
    else if (prefix.compare("Ns") == 0) {
      this->Ns.push_back(
        std::stof(tokens[0])
      );
    }
    else if (prefix.compare("Tr") == 0) {
      this->Tr.push_back(
        std::stof(tokens[0])
      );
    }
    else if (prefix.compare("d") == 0) {
      this->d.push_back(
        std::stof(tokens[0])
      );
    }
    else if (prefix.compare("illum") == 0) {
      this->illum.push_back(
        std::stoi(tokens[0])
      );
    }
    else if (prefix.compare("map_Kd") == 0) {
      this->map_Kd.push_back(
        tokens[0]
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
    if (prefix.compare("s") == 0) {
      this->s.push_back(
        tokens[0]
      );
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

void mtl::save(std::string filename)
{
  std::ofstream file(filename, std::ios::binary);

  for (auto i = 0u; i < this->newmtl.size(); i++) {
    file << "newmtl " << newmtl[i] << std::endl;
    if (i < Ns.size()) {
      file << "Ns " << Ns[i] << std::endl;
    }
    if (i < Ka.size()) {
      file << "Ka " << Ka[i][0] << " " << Ka[i][1] << " " << Ka[i][2] << std::endl;
    }
    if (i < Kd.size()) {
      file << "Kd " << Kd[i][0] << " " << Kd[i][1] << " " << Kd[i][2] << std::endl;
    }
    if (i < Ks.size()) {
      file << "Ks " << Ks[i][0] << " " << Ks[i][1] << " " << Ks[i][2] << std::endl;
    }
    if (i < Ke.size()) {
      file << "Ke " << Ke[i][0] << " " << Ke[i][1] << " " << Ke[i][2] << std::endl;
    }
    if (i < Ni.size()) {
      file << "Ni " << Ni[i] << std::endl;
    }
    if (i < Tr.size()) {
      file << "Tr " << Tr[i] << std::endl;
    }
    if (i < d.size()) {
      file << "d " << d[i] << std::endl;
    }
    if (i < illum.size()) {
      file << "illum " << illum[i] << std::endl;
    }
    if (i < map_Kd.size()) {
      file << "map_Kd " << map_Kd[i] << std::endl;
    }
    file << std::endl;
  }

  file.close();
}

std::string to_string(const std::array<int, 3>& x) {
  return (x[0] == 0 ? "" : std::to_string(x[0])) + "/"
       + (x[1] == 0 ? "" : std::to_string(x[1])) + "/"
       + (x[2] == 0 ? "" : std::to_string(x[2]));
}

void obj::save(std::string filename)
{
  std::ofstream file(filename, std::ios::binary);

  file << "mtllib " << mtllib << std::endl;
  for (auto i = 0u; i < this->o.size(); i++) {
    file << "o " << o[i] << std::endl;
    for (auto x : v) {
      file << "v " << x[0] << " " << x[1] << " " << x[2] << std::endl;
    }
    for (auto x : vn) {
      file << "vn " << x[0] << " " << x[1] << " " << x[2] << std::endl;
    }
    for (auto x : vt) {
      file << "vt " << x[0] << " " << x[1] << std::endl;
    }
    file << "usemtl " << usemtl[i] << std::endl;
    file << "s " << s[i] << std::endl;
    for (auto x = 0u; x < f[i].size() / 3; x++) {
      file << "f " << to_string(f[i][x * 3]) << " " << to_string(f[i][x * 3 + 1]) << " " << to_string(f[i][x * 3 + 2]) << std::endl;
    }
  }

  file.close();
}
