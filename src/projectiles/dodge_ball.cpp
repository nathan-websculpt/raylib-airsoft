#include "../../include/projectiles/dodge_ball.h"
#include "raymath.h"

void DodgeBall::update(float dt) {
    if (!m_alive) return;
    m_velocity.y -= m_gravity * dt;
    m_position = Vector3Add(m_position, Vector3Scale(m_velocity, dt));
    if (m_position.y < 0.0f) {
        m_position.y = 0.0f;
        m_velocity.y *= -0.5f;
        if (fabs(m_velocity.y) < 1.0f) m_alive = false;
    }
}