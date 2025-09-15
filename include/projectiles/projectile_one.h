#pragma once
#include "base_projectile.h"

class ProjectileOne final : public BaseProjectile {
public:
    using BaseProjectile::BaseProjectile;
    void update(float dt) override;
};