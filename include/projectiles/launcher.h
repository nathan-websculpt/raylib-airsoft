#pragma once
#include "gear_config.h"
#include "projectile_two.h"
#include "dodge_ball.h"

class Launcher {
private:
    GearConfig config;
    float timeSinceLastShot {0.0f};
    
public:
    explicit Launcher(const GearConfig& cfg);

    void update(float dt);

    void tryFire(Camera& cam, std::vector<std::unique_ptr<BaseProjectile>>& projectiles, int projectileType);

    void setConfig(const GearConfig& cfg);
};