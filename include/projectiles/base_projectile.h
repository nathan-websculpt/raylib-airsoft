#pragma once
#include "raylib.h"

class BaseProjectile {
protected:
    Vector3 m_position;
    Vector3 m_velocity;
    float m_radius;
    Color m_color;
    bool m_alive;
    float m_gravity;
    BoundingBox m_boundingBox;

public:
    BaseProjectile(Vector3 pos, Vector3 vel, float radius, Color color);
    virtual ~BaseProjectile();

    virtual void update(float dt);
    virtual void draw() const;
    // virtual void DrawDebug() const;

    bool isAlive() const;
    void setGravity(float g);

    [[nodiscard]] BoundingBox GetBoundingBox() const { return m_boundingBox; }
    // [[nodiscard]] Vector3 GetPosition() const { return m_position; }
    // [[nodiscard]] Vector3 GetSize() const { return m_size; }
};
