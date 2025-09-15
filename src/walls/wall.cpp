#include "../../include/walls/wall.h"

Wall::Wall(Vector3 position, Vector3 size)
    : m_position(position), m_size(size)
{
    Vector3 min = { position.x - size.x/2, position.y - size.y/2, position.z - size.z/2 };
    Vector3 max = { position.x + size.x/2, position.y + size.y/2, position.z + size.z/2 };
    m_boundingBox = { min, max };
}

void Wall::DrawDebug() {
    DrawBoundingBox(m_boundingBox, RED);

    for (const Vector3& point : Wall::GetCollisionPoints()) {
        DrawSphere(point, 0.1f, BLUE);
    }
}

void Wall::AddCollisionPoint(const Vector3& point) {
    m_collisionPoints.emplace_back(point);
}


