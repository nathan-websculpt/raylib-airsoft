#pragma once
#include "gear_config.h"
#include "projectile.h"

class Launcher {
private:
    GearConfig config;
    float timeSinceLastShot {0.0f};
    
public:
    explicit Launcher(const GearConfig& cfg);

    void update(float dt);

    void tryFire(Camera& cam, std::vector<std::unique_ptr<Projectile>>& projectiles, int projectileType);

    void setConfig(const GearConfig& cfg);
};