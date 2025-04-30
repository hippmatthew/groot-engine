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

  std::vector<vec3> positions;
  std::vector<vec2> uvs;
  std::vector<vec3> normals;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  std::map<std::tuple<unsigned int, unsigned int, unsigned int>, unsigned int> indexMap;

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream ss(line);

    std::string key;
    ss >> key;

    if (key == "v") {
      vec3 position = vec3(0.0f);
      ss >> position.x >> position.y >> position.z;
      positions.emplace_back(position);
    }
    else if (key == "vt") {
      vec2 uv = vec2(0.0f);
      ss >> uv.x >> uv.y;
      uvs.emplace_back(uv);
    }
    else if (key == "vn") {
      vec3 normal = vec3(0.0f);
      ss >> normal.x >> normal.y >> normal.z;
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

SPVParser::Output SPVParser::parse(std::string path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) throw std::runtime_error("groot-engine: failed to open shader at " + path);

  std::vector<char> buffer;
  unsigned int size = file.tellg();

  if (size < 4)
    throw std::runtime_error("groot-engine: file too small - " + path);

  if (size % 4 != 0)
    throw std::runtime_error("groot-engine: corrupted shader - " + path);

  buffer.resize(4);
  file.seekg(0);
  file.read(buffer.data(), 4);

  if (*reinterpret_cast<unsigned int *>(buffer.data()) != 0x07230203)
    throw std::runtime_error("groot-engine: incorrect file format - " + path);

  buffer.resize(size);
  file.read(buffer.data() + 4, size - 4);

  return buffer;
}

} // namespace ge