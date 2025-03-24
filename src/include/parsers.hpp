#pragma once

#include "src/include/vertex.hpp"

#include <string>
#include <vector>

namespace ge {

class ObjParser {

  using Output = std::pair<std::vector<Vertex>, std::vector<unsigned int>>;

  public:
    ObjParser() = delete;
    ObjParser(ObjParser&) = delete;
    ObjParser(ObjParser&&) = delete;

    ~ObjParser() = default;

    ObjParser& operator = (ObjParser&) = delete;
    ObjParser& operator = (ObjParser&&) = delete;

    static Output parse(std::string);
};

} // namespace ge