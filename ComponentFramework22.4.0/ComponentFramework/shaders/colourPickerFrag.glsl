#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;

in vec4 PickingColorFrag;

void main() {
    fragColor = PickingColorFrag;
}