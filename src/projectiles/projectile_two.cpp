#include "../../include/projectiles/projectile_two.h"
#include "raymath.h"

void ProjectileTwo::update(float dt) {
    if (!m_alive) return;
    m_position = Vector3Add(m_position, Vector3Scale(m_velocity, dt));
    if (Vector3Length(m_velocity) * dt > 200.0f) m_alive = false;
}