#pragma once

#include <random>

namespace tests {

class Random {
  public:
    Random() = default;
    Random(Random&) = delete;
    Random(Random&&) = delete;

    ~Random() = default;

    Random& operator = (Random&) = delete;
    Random& operator = (Random&&) = delete;

    float operator () () {
      return distribution(generator);
    }

  private:
    std::random_device device;
    std::mt19937_64 generator = std::mt19937_64(device());
    std::uniform_real_distribution<float> distribution = std::uniform_real_distribution<float>(-50, 50);
};

} // namespace tests