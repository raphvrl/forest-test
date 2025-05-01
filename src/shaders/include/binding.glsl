#ifndef BINDING_GLSL
#define BINDING_GLSL

#define CAMEA_UBO 0
#define TIME_UBO 1

struct CameraUBO
{
    mat4 view;
    mat4 proj;
    mat4 ortho;
    vec3 position;
};

struct TimeUBO
{
    float time;
    float deltaTime;
};

layout(binding = 0) uniform UniformBufferObjects {
    CameraUBO camera;
    TimeUBO time;
} ubos[];

CameraUBO getCamera()
{
    return ubos[CAMEA_UBO].camera;
}

TimeUBO getTime()
{
    return ubos[TIME_UBO].time;
}

#endif