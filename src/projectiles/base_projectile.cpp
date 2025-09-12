#include "../../include/projectiles/base_projectile.h"
#include "raymath.h"

BaseProjectile::BaseProjectile(Vector3 pos, Vector3 vel, float radius, Color color)
    : m_position(pos), m_velocity(vel), m_radius(radius), m_color(color), m_alive(true), m_gravity(2.0f) {}

BaseProjectile::~BaseProjectile() = default;

void BaseProjectile::update(float dt) {
    if (!m_alive) return;
    m_velocity.y -= m_gravity * dt;
    m_position = Vector3Add(m_position, Vector3Scale(m_velocity, dt));
    if (m_position.y < 0.0f) m_alive = false;
}

void BaseProjectile::draw() const {
    if (m_alive) DrawSphere(m_position, m_radius, m_color);
}

bool BaseProjectile::isAlive() const { return m_alive; }
void BaseProjectile::setGravity(float g) { m_gravity = g; }