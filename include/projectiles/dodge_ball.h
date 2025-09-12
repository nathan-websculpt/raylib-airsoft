#pragma once
#include "base_projectile.h" 


class DodgeBall final : public BaseProjectile {
public:
    using BaseProjectile::BaseProjectile;
    void update(float dt) override;
};