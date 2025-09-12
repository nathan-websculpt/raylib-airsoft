#pragma once
#include "base_projectile.h"

class ProjectileTwo final : public BaseProjectile {
public:
    using BaseProjectile::BaseProjectile;
    void update(float dt) override;
};