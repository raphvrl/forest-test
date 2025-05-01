#include "game.hpp"

namespace core
{

Game::Game()
{
    m_window.init(800, 600, "My Game");

    m_device.init(m_window, "Vulkan Engine", {1, 0, 0});

    std::vector<gfx::Mesh::Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}}
    };

    std::vector<u32> indices = {0, 1, 2};

    m_mesh.init(m_device, vertices, indices);

    auto binding = gfx::Mesh::Vertex::getBindingDescription();
    auto attributes = gfx::Mesh::Vertex::getAttributeDescriptions();
    
    m_pipeline = gfx::Pipeline::Builder(m_device)
        .setShader(
            "assets/shaders/mesh.vert.spv",
            VK_SHADER_STAGE_VERTEX_BIT
        )
        .setShader(
            "assets/shaders/mesh.frag.spv",
            VK_SHADER_STAGE_FRAGMENT_BIT
        )
        .setPushConstant(
            sizeof(glm::mat4)
        )
        .setVertexInput({
            &binding,
            attributes.data(),
            static_cast<u32>(attributes.size())
        })
        .build();

    m_camera.setPosition({0.0f, 0.0f, 3.0f});

    m_gpuData.init(m_device);
}

Game::~Game()
{
    m_device.waitIdle();

    m_gpuData.destroy();

    m_pipeline.destroy();

    m_mesh.destroy();

    m_device.destroy();
    m_window.destroy();
}

void Game::run()
{
    while (m_running && !m_window.shouldClose()) {
        m_time = m_window.getTime();
        m_deltaTime = m_time - m_lastTime;
        m_lastTime = m_time;

        handleInput(m_deltaTime);
        update(m_deltaTime);
        render();
    }
}

void Game::handleInput(f32 dt)
{
    m_window.pollEvents();

    bool ctrl = m_window.isKeyPressed(GLFW_KEY_LEFT_CONTROL);

    if (ctrl) {
        if (m_window.isKeyJustPressed(GLFW_KEY_F4)) {
            m_running = false;
        }
    }

    glm::vec3 direciton(0.0f);

    if (m_window.isKeyPressed(GLFW_KEY_W)) direciton.z += 1.0f;
    if (m_window.isKeyPressed(GLFW_KEY_S)) direciton.z -= 1.0f;
    if (m_window.isKeyPressed(GLFW_KEY_A)) direciton.x -= 1.0f;
    if (m_window.isKeyPressed(GLFW_KEY_D)) direciton.x += 1.0f;
    if (m_window.isKeyPressed(GLFW_KEY_Q)) direciton.y -= 1.0f;
    if (m_window.isKeyPressed(GLFW_KEY_E)) direciton.y += 1.0f;

    m_camera.move(direciton, dt * 5.0f);

    glm::vec2 mouseRel = m_window.getMouseRel();

    if (m_window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        m_window.setCursorMode(GLFW_CURSOR_DISABLED);
        m_camera.rotate(mouseRel.x * 0.1f, -mouseRel.y * 0.1f);
    } else {
        m_window.setCursorMode(GLFW_CURSOR_NORMAL);
    }
}

void Game::update(f32 dt)
{
    UNUSED(dt);

    m_camera.setAspect(m_window.getAspect());

    m_camera.setOrthoLeft(0.0f);
    m_camera.setOrthoRight(m_window.getWidth());
    m_camera.setOrthoBottom(0.0f);
    m_camera.setOrthoTop(m_window.getHeight());

    m_camera.update();

    m_gpuData.updateCamera(m_camera);
}

void Game::render()
{
    u32 width = m_window.getWidth();
    u32 height = m_window.getHeight();

    if (width == 0 || height == 0) {
        return;
    }

    auto cmd = m_device.beginFrame();
    if (!cmd) return;

    m_device.beginRenderClear(cmd);

    m_pipeline.bind(cmd);

    glm::mat4 model = glm::mat4(1.0f);
    m_pipeline.push(cmd, model);

    m_mesh.bind(cmd);
    m_mesh.draw(cmd);

    m_device.endRender(cmd);

    m_device.endFrame(cmd);
}

} // namespace core