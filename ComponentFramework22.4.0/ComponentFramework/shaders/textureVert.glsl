#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

//layout(location = 0) uniform mat4 projectionMatrix; //old system
//layout(location = 1) uniform mat4 viewMatrix;
layout (std140, binding = 0) uniform CameraMatrices { //binding was = 0 & we are using std140
    mat4 projection;
    mat4 view;
};

layout (std140, binding = 1) uniform LightData {
    vec3 pos;
    vec4 colour;
};

layout(location = 2) uniform mat4 modelMatrix;

layout(location = 0) out vec3 vertNormal;
layout(location = 1) out vec3 lightDir;
layout(location = 2) out vec3 eyeDir; 
layout(location = 3) out vec2 texCoord; 


void main() {
    texCoord = uvCoord;
    texCoord.y *= -1.0;
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    vertNormal = normalize(normalMatrix * vNormal); /// Rotate the normal to the correct orientation 
    vec3 vertPos = vec3(view * modelMatrix * vVertex);
    vec3 vertDir = normalize(vertPos);
    eyeDir = -vertDir;
    lightDir = normalize(pos - vertPos); 
    gl_Position = projection * view * modelMatrix * vVertex;
}