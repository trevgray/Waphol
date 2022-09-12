#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUVCoord;

layout (std140, binding = 0) uniform CameraMatrices { //binding was = 0 & we are using std140
    mat4 projection;
    mat4 view;
};
layout(location = 2) uniform mat4 modelMatrix;

uniform vec4 PickingColor;

out vec4 PickingColorFrag;

void main() {
    PickingColorFrag = PickingColor;
    gl_Position = projection * view * modelMatrix * vec4(inVertex, 1.0);
}