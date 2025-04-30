#version 460

layout(push_constant) uniform push_constants {
  layout(row_major) mat4 ge_View;
  layout(row_major) mat4 ge_Projection;
  uint ge_Frame;
  uint ge_Material;
  uint ge_Transform;
};

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 normal;

layout(location = 0) out vec4 frag_color;

void main() {
  frag_color = vec4(uv, 0.0, 1.0);
}