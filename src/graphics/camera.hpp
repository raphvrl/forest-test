#pragma once

#include <glm/ext.hpp>

#include "core/types.hpp"

namespace gfx
{

class Camera
{

public:
    Camera();
    ~Camera() = default;

    void update();

    void move(const glm::vec3 &direction, f32 speed);

    void rotate(f32 xOffset, f32 yOffset);
    void rotate(const glm::vec2 &offset) { rotate(offset.x, offset.y); }

public:
    void setPosition(const glm::vec3 &pos) { m_pos = pos; }
    void setFront(const glm::vec3 &front) { m_front = front; }
    void setUp(const glm::vec3 &up) { m_up = up; }
    void setRight(const glm::vec3 &right) { m_right = right; }

    void setYaw(f32 yaw) { m_yaw = yaw; }
    void setPitch(f32 pitch) { m_pitch = pitch; }

    void setFov(f32 fov) { m_fov = fov; }
    void setAspect(f32 aspect) { m_aspect = aspect; }
    void setNearPlane(f32 value) { m_near = value; }
    void setFarPlane(f32 value) { m_far = value; }

    void setOrthoLeft(f32 value) { m_orthoLeft = value; }
    void setOrthoRight(f32 value) { m_orthoRight = value; }
    void setOrthoBottom(f32 value) { m_orthoBottom = value; }
    void setOrthoTop(f32 value) { m_orthoTop = value; }

public:
    const glm::vec3 &getPosition() const { return m_pos; }
    const glm::vec3 &getFront() const { return m_front; }
    const glm::vec3 &getUp() const { return m_up; }
    const glm::vec3 &getRight() const { return m_right; }

    f32 getYaw() const { return m_yaw; }
    f32 getPitch() const { return m_pitch; }

    const glm::mat4 &getView() const { return m_view; }

    f32 getFov() const { return m_fov; }
    f32 getAspect() const { return m_aspect; }
    f32 getNear() const { return m_near; }
    f32 getFar() const { return m_far; }

    const glm::mat4 &getProjection() const { return m_proj; }

    f32 getOrthoLeft() const { return m_orthoLeft; }
    f32 getOrthoRight() const { return m_orthoRight; }
    f32 getOrthoBottom() const { return m_orthoBottom; }
    f32 getOrthoTop() const { return m_orthoTop; }

    const glm::mat4 &getOrtho() const { return m_ortho; }

private:
    glm::vec3 m_pos;

    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;

    glm::vec3 m_worldUp;

    f32 m_yaw;
    f32 m_pitch;

    glm::mat4 m_view;

    f32 m_fov;
    f32 m_aspect;
    f32 m_near;
    f32 m_far;

    glm::mat4 m_proj;

    f32 m_orthoLeft;
    f32 m_orthoRight;
    f32 m_orthoBottom; 
    f32 m_orthoTop;

    glm::mat4 m_ortho;
};

} // namespace gfx