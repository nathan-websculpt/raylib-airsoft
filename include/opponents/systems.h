#pragma once
#include <vector>
#include <algorithm>
#include "raylib.h"
#include "components.h"

inline void movementSystem(std::vector<Position>& positions,
                           const std::vector<Movement>& movements,
                           const Vector3& target) {
    for (size_t i = 0; i < positions.size(); ++i) {
        auto movement = Vector3Subtract(target, positions[i].pos);
        movement = Vector3Scale(movement, movements[i].speed);
        positions[i].pos = Vector3Add(positions[i].pos, movement);
    }
}

inline void boundingSystem(std::vector<Position>& positions,
                           std::vector<Bounding>& bounds) {
    for (size_t i = 0; i < positions.size(); ++i) {
        bounds[i].box.min = Vector3Subtract(positions[i].pos, (Vector3){0.25f, 1.25f, 0.25f});
        bounds[i].box.max = Vector3Add(positions[i].pos, (Vector3){0.25f, 2.5f, 0.25f});
    }
}

inline void renderSystem(const std::vector<Position>& positions,
                         const std::vector<Bounding>& bounds,
                         const std::vector<Health>& healths,
                         const std::vector<Render>& renders) {
    for (size_t i = 0; i < positions.size(); ++i) {
        if (healths[i].hp <= 0) continue;
        DrawCube(positions[i].pos, renders[i].size.x, renders[i].size.y, renders[i].size.z, renders[i].color);
        DrawBoundingBox(bounds[i].box, GREEN);
    }
}

inline void damageSystem(std::vector<Health>& healths,
                         const std::vector<Bounding>& bounds,
                         const BoundingBox& projectileBox) {
    for (size_t i = 0; i < healths.size(); ++i) {
        if (CheckCollisionBoxes(projectileBox, bounds[i].box)) {
            healths[i].hp -= 10;
        }
    }
}
