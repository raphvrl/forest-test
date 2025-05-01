#pragma once

#include "core/window/window.hpp"

#include "graphics/device.hpp"
#include "graphics/pipeline.hpp"
#include "graphics/mesh.hpp"
#include "graphics/camera.hpp"
#include "graphics/gpu_data.hpp"

namespace core
{   

class Game
{

public:
    Game();
    ~Game();

    void run();

private:
    bool m_running = true;

    Window m_window;

    gfx::Device m_device;
    gfx::Mesh m_mesh;
    gfx::Pipeline m_pipeline;
    gfx::Camera m_camera;
    gfx::GPUData m_gpuData;

private:
    f32 m_time = 0.0f;
    f32 m_deltaTime = 0.0f;
    f32 m_lastTime = 0.0f;

private:
    void handleInput(f32 dt);
    void update(f32 dt);
    void render();
};

} // namespace core