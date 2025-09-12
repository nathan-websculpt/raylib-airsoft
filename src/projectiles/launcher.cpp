#include "../../include/projectiles/launcher.h"
#include "raymath.h"

Launcher::Launcher(const GearConfig& cfg) : config(cfg) {}

void Launcher::update(float dt) {
    timeSinceLastShot += dt;
}

void Launcher::tryFire(Camera& cam, std::vector<std::unique_ptr<BaseProjectile>>& projectiles, int projectileType) {
    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE)) &&
        timeSinceLastShot >= config.fireCooldown) {

        Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, cam.up));
        Vector3 up = Vector3CrossProduct(right, forward);


        Vector3 muzzle = Vector3Add(cam.position, Vector3Scale(right, config.muzzleOffsetRight));
        muzzle = Vector3Add(muzzle, Vector3Scale(up, config.muzzleOffsetUp));

        Vector3 velocity = Vector3Scale(forward, config.projectileSpeed);

        switch (projectileType) {
            case 0: {
                std::unique_ptr<BaseProjectile> proj = std::make_unique<BaseProjectile>(muzzle, velocity, config.projectileRadius, config.projectileColor);
                proj->setGravity(config.gravity);
                projectiles.push_back(std::move(proj));
                break;
            }
            case 1: {
                std::unique_ptr<BaseProjectile> proj = std::make_unique<ProjectileTwo>(muzzle, velocity, config.projectileRadius, RED);
                proj->setGravity(0.0f);
                projectiles.push_back(std::move(proj));
                break;
            }
            case 2: {
                std::unique_ptr<BaseProjectile> proj = std::make_unique<DodgeBall>(muzzle, velocity, config.projectileRadius, DARKGREEN);
                proj->setGravity(config.gravity);
                projectiles.push_back(std::move(proj));
                break;
            }
        }

        timeSinceLastShot = 0.0f;
    }
}

void Launcher::setConfig(const GearConfig& cfg) {
    config = cfg;
}