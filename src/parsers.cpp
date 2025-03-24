#include "src/include/parsers.hpp"

#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <tuple>

namespace ge {

ObjParser::Output ObjParser::parse(std::string path) {
  std::ifstream file(path);
  if (!file) throw std::runtime_error("groot-engine: failed to open " + path);

  std::vector<vec<3>> positions;
  std::vector<vec<2>> uvs;
  std::vector<vec<3>> normals;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  std::map<std::tuple<unsigned int, unsigned int, unsigned int>, unsigned int> indexMap;

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream ss(line);

    std::string key;
    ss >> key;

    if (key == "v") {
      vec<3> position = vec<3>::zero();
      ss >> position[0] >> position[1] >> position[2];
      positions.emplace_back(position);
    }
    else if (key == "vt") {
      vec<2> uv = vec<2>::zero();
      ss >> uv[0] >> uv[1];
      uvs.emplace_back(uv);
    }
    else if (key == "vn") {
      vec<3> normal = vec<3>::zero();
      ss >> normal[0] >> normal[1] >> normal[2];
      normals.emplace_back(normal);
    }
    else if (key == "f") {
      std::string token;
      while (ss >> token) {
        std::replace(token.begin(), token.end(), '/', ' ');
        std::istringstream ts(token);

        unsigned int vertIndex, uvIndex, normIndex;
        ts >> vertIndex >> uvIndex >> normIndex;

        if (indexMap.contains({ vertIndex, uvIndex, normIndex })) {
          indices.emplace_back(indexMap.at({ vertIndex, uvIndex, normIndex }));
          continue;
        }

        indexMap.emplace(std::make_pair(std::tuple{ vertIndex, uvIndex, normIndex }, vertices.size()));
        indices.emplace_back(vertices.size());
        vertices.emplace_back(Vertex{ positions[vertIndex - 1], uvs[uvIndex - 1], normals[normIndex - 1] });
      }
    }
  }

  return { std::move(vertices), std::move(indices) };
}

} // namespace ge