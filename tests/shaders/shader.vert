#version 460

layout(set = 0, binding = 0) readonly buffer transforms {
  layout(row_major) mat4 ge_Models[];
};

layout(push_constant) uniform push_constants {
  layout(row_major) mat4 ge_View;
  layout(row_major) mat4 ge_Projection;
  uint ge_Frame;
  uint ge_Material;
  uint ge_Transform;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv_in;
layout(location = 2) in vec3 normal_in;

layout(location = 0) out vec2 uv_out;
layout(location = 1) out vec3 normal_out;

void main() {
  uint transform_index = ge_Transform + gl_BaseInstance;

  gl_Position = ge_Projection * ge_View * ge_Models[transform_index] * vec4(position, 1.0);
  uv_out = uv_in;
  normal_out = normal_in;
}