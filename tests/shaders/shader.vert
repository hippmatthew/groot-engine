#version 460

layout(push_constant) uniform push_constants {
  layout(row_major) mat4 camera;
  layout(row_major) mat4 transforms[1];
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec3 normal_in;

layout(location = 0) out vec2 uv_out;
layout(location = 1) out vec3 normal_out;

void main() {
  gl_Position = camera * transforms[gl_BaseInstance] * vec4(position, 1.0);
  uv_out = uv_in;
  normal_out = normal_in;
}