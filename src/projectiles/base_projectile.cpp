#include "../../include/projectiles/base_projectile.h"
#include "raymath.h"

BaseProjectile::BaseProjectile(Vector3 pos, Vector3 vel, float radius, Vector3 forward, Color color, bool canBounce)
    : m_position(pos), m_velocity(vel), m_radius(radius), m_forward(forward), m_color(color), m_canBounce(canBounce), m_alive(true), m_gravity(2.0f) {}

BaseProjectile::~BaseProjectile() = default;

void BaseProjectile::update(float dt) {
    if (!m_alive) return;
    m_velocity.y -= m_gravity * dt;
    m_position = Vector3Add(m_position, Vector3Scale(m_velocity, dt));

    Vector3 min = {m_position.x - m_radius, m_position.y - m_radius, m_position.z - m_radius};
    Vector3 max = {m_position.x + m_radius, m_position.y + m_radius, m_position.z + m_radius};
    m_boundingBox = { min, max };

    if(m_canBounce) {
        if (m_position.y < 0.0f) {
            m_position.y = 0.0f;
            m_velocity.y *= -0.5f;
            if (fabs(m_velocity.y) < 1.0f) m_alive = false;
        }
    }

    if (m_position.y < 0.0f) m_alive = false; // TODO: use configs
}

void BaseProjectile::draw(bool isDebug) const {
    if (m_alive) {
        DrawSphere(m_position, m_radius, m_color);
        if (isDebug) DrawBoundingBox(m_boundingBox, RED);
    }
}

bool BaseProjectile::isAlive() const { return m_alive; }
void BaseProjectile::setGravity(float g) { m_gravity = g; }
void BaseProjectile::setIsAlive(bool alive) { m_alive = alive; }