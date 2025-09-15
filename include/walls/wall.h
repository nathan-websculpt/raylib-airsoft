// Wall is an abstract base class (pure virtual Draw()) for all wall types.

#pragma once
#include "raylib.h"
#include <vector>

class Wall {
private:
    // prevents accidental modification and allows validation
    Vector3 m_position;
    Vector3 m_size;
    BoundingBox m_boundingBox;
	std::vector<Vector3> m_collisionPoints;

public:
    Wall(Vector3 position, Vector3 size);
    virtual ~Wall() = default;

    virtual void Draw() const = 0;     // Pure virtual
    virtual void DrawDebug();
    virtual void AddCollisionPoint(const Vector3& point);


    // getters for private members
    [[nodiscard]] BoundingBox GetBoundingBox() const { return m_boundingBox; }
    [[nodiscard]] Vector3 GetPosition() const { return m_position; }
    [[nodiscard]] Vector3 GetSize() const { return m_size; }
    
    std::vector<Vector3>& GetCollisionPoints() { return m_collisionPoints; }
};
