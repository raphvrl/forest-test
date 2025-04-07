#pragma once

#include "window.hpp"
#include "graphics/device.hpp"
#include "graphics/pipeline.hpp"

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

    gfx::Pipeline m_pipeline;

};

} // namespace core