#pragma once
#include "raylib.h"

class Projectile {
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
    Projectile(Vector3 pos, Vector3 vel, float radius, Vector3 forward, Color color, bool canBounce);
    virtual ~Projectile();

    virtual void update(float dt);
    virtual void draw(bool isDebug) const;

    bool isAlive() const;
    void setGravity(float g);
    void setIsAlive(bool alive);

    [[nodiscard]] BoundingBox GetBoundingBox() const { return m_boundingBox; }
    [[nodiscard]] Vector3 GetForward() const { return m_forward; } // so that the collisions on the walls know the line of the projectile

};
