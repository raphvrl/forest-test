#version 450
#extension GL_GOOGLE_include_directive : require

#include "binding.glsl"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(push_constant) uniform PushConstants {
    mat4 model;
} pc;

void main()
{
    CameraUBO camera = getCamera();

    mat4 view = camera.view;
    mat4 proj = camera.proj;
    mat4 model = pc.model;

    gl_Position = proj * view * model * vec4(inPosition, 1.0);
}