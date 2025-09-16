#pragma once
#include <vector>
#include "raylib.h"
#include "components.h"

// storage for all opponent components
struct OpponentStore {
    std::vector<Position> positions;
    std::vector<Bounding> bounds;
    std::vector<Movement> movements;
    std::vector<Health> healths;
    std::vector<Render> renders;
};

inline void spawnOpponent(OpponentStore& store,
                          Vector3 pos,
                          float speed,
                          int hp,
                          Color color,
                          Vector3 size) {
    store.positions.push_back({pos});
    store.bounds.push_back({}); // updated later by boundingSystem
    store.movements.push_back({speed});
    store.healths.push_back({hp});
    store.renders.push_back({color, size});
}
