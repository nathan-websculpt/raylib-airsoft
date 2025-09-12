#pragma once
#include "raylib.h"
#include <fstream>
#include "../nlohmann/json.hpp"

struct GearConfig {
    float projectileSpeed;
    float gravity;
    float muzzleOffsetRight;
    float muzzleOffsetUp;
    float fireCooldown;
    float projectileRadius;
    Color projectileColor;
};

std::map<std::string, GearConfig> LoadGearConfigs(const std::string& filename);
