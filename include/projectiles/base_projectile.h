#pragma once
#include "raylib.h"

class BaseProjectile {
protected:
    Vector3 m_position;
    Vector3 m_velocity;
    float m_radius;
    Vector3 m_forward;
    Color m_color;
    bool m_alive;
    bool m_canBounce;
    float m_gravity;
    BoundingBox m_boundingBox;

public:
    BaseProjectile(Vector3 pos, Vector3 vel, float radius, Vector3 forward, Color color, bool canBounce);
    virtual ~BaseProjectile();

    virtual void update(float dt);
    virtual void draw() const;
    // virtual void DrawDebug() const;

    bool isAlive() const;
    void setGravity(float g);

    [[nodiscard]] BoundingBox GetBoundingBox() const { return m_boundingBox; }
    [[nodiscard]] Vector3 GetForward() const { return m_forward; } // so that the collisions on the walls know the line of the projectile

    // [[nodiscard]] Vector3 GetPosition() const { return m_position; }
    // [[nodiscard]] Vector3 GetSize() const { return m_size; }
};
