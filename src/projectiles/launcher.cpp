#include "../../include/projectiles/launcher.h"
#include "raymath.h"

Launcher::Launcher(const GearConfig& cfg) : config(cfg) {}

void Launcher::update(float dt) {
    timeSinceLastShot += dt;
}

void Launcher::tryFire(Camera& cam, std::vector<std::unique_ptr<BaseProjectile>>& projectiles, int projectileType) {
    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE)) && timeSinceLastShot >= config.fireCooldown) {

        Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, cam.up));
        Vector3 up = Vector3CrossProduct(right, forward);

        Vector3 muzzle = Vector3Add(cam.position, Vector3Scale(right, config.muzzleOffsetRight));
        muzzle = Vector3Add(muzzle, Vector3Scale(up, config.muzzleOffsetUp));

        Vector3 velocity = Vector3Scale(forward, config.projectileSpeed);

        std::unique_ptr<BaseProjectile> proj = std::make_unique<BaseProjectile>(muzzle, velocity, config.projectileRadius, forward, config.projectileColor, config.canBounce);
        proj->setGravity(config.gravity);
        projectiles.emplace_back(std::move(proj));

        timeSinceLastShot = 0.0f;
    }
}

void Launcher::setConfig(const GearConfig& cfg) {
    config = cfg;
}