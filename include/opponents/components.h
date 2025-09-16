#pragma once
#include "raylib.h"

struct Position {
    Vector3 pos;
};

struct Bounding {
    BoundingBox box;
};

struct Movement {
    float speed;
};

struct Health {
    int hp;
};

struct Render {
    Color color;
    Vector3 size;
};
