#include "src/include/engine.hpp"

namespace ge {

Engine::Engine() {
  initialize();
}

Engine::Engine(const Settings& settings) : m_settings(settings) {
  initialize();
}

Engine::Engine(Settings&& settings) : m_settings(settings) {
  initialize();
}

Engine::~Engine() {
  glfwDestroyWindow(p_window);
  glfwTerminate();
}

void Engine::run() {
  run([](){});
}

bool Engine::shouldClose() const {
  return glfwWindowShouldClose(p_window);
}

void Engine::pollEvents() const {
  glfwPollEvents();
}

void Engine::initialize() {
  createWindow();
  m_context.initialize(*this);
  createSurface();
  m_context.initialize(*this);
}

void Engine::createWindow() {
  if (glfwInit() != GLFW_TRUE)
    throw std::runtime_error("groot-engine: failed to initialize glfw");

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  p_window = glfwCreateWindow(
    m_settings.window_width,
    m_settings.window_height,
    m_settings.window_title.c_str(),
    nullptr,
    nullptr
  );

  if (p_window == nullptr)
    throw std::runtime_error("groot-engine: failed to create window");
}

void Engine::createSurface() {
  VkSurfaceKHR surface;
  if (glfwCreateWindowSurface(*m_context.instance(), p_window, nullptr, &surface) != VK_SUCCESS)
    throw std::runtime_error("groot-engine: failed to create window surface");

  m_surface = vk::raii::SurfaceKHR(m_context.instance(), surface);
  if (*m_surface == nullptr)
    throw std::runtime_error("groot-engine: failed to create window surface");
}

} // namespace ge