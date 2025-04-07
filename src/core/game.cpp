#include "game.hpp"

namespace core
{

Game::Game()
{
    m_window.init(800, 600, "My Game");
    m_device.init(m_window, "Vulkan Engine", {1, 0, 0});

    m_pipeline = gfx::Pipeline::Builder(m_device)
        .setShader(
            "assets/shaders/triangle.vert.spv",
            VK_SHADER_STAGE_VERTEX_BIT
        )
        .setShader(
            "assets/shaders/triangle.frag.spv",
            VK_SHADER_STAGE_FRAGMENT_BIT
        )
        .build();
}

Game::~Game()
{
    m_device.waitIdle();

    m_pipeline.destroy();

    m_device.destroy();
    m_window.destroy();
}

void Game::run()
{
    while (m_running && !m_window.shouldClose()) {
        m_window.pollEvents();

        if (
            m_window.isKeyPressed(GLFW_KEY_LEFT_ALT) &&
            m_window.isKeyJustPressed(GLFW_KEY_F4)
        ) {
            m_running = false;
        }

        auto cmd = m_device.beginFrame();
        if (cmd) {
            m_pipeline.bind(cmd);

            vkCmdDraw(cmd, 3, 1, 0, 0);

            m_device.endFrame();
        }
    }
}

} // namespace core