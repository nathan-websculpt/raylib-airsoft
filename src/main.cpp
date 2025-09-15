#include "raylib.h"
#include <memory>
#include <iostream>
#include "../include/core/custom_camera.h"
#include "../include/walls/wall_handler.h"
#include "../include/walls/textured_wall.h"
#include "../include/walls/textured_wall_rec.h"
#include "../include/walls/colored_wall.h"
#include "../include/walls/managed_texture.h"
#include "../include/projectiles/launcher.h"

// g++ -std=c++23 src/main.cpp src/walls/wall.cpp src/walls/wall_handler.cpp src/walls/textured_wall.cpp src/walls/textured_wall_rec.cpp src/walls/colored_wall.cpp src/walls/draw_utils.cpp src/projectiles/gear_config.cpp src/projectiles/launcher.cpp src/projectiles/base_projectile.cpp -o main -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

// release build targeting C++23 with warnings and optimizations
// g++ -std=c++23 -O2 -march=native -flto -Wall -Wextra -Wpedantic src/main.cpp src/walls/wall.cpp src/walls/wall_handler.cpp src/walls/textured_wall.cpp src/walls/textured_wall_rec.cpp src/walls/colored_wall.cpp src/walls/draw_utils.cpp src/projectiles/gear_config.cpp src/projectiles/launcher.cpp src/projectiles/base_projectile.cpp -o main -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

int main(void)
{
    const int screenWidth = 4400;
    const int screenHeight = 2800;
    const std::string configFile = "assets/configs/gear.json";

    InitWindow(screenWidth, screenHeight, "FPS SYSTEM");

    Camera camera { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;                         
    camera.projection = CAMERA_PERSPECTIVE;      

    int cameraMode = CAMERA_FIRST_PERSON;
    DisableCursor();
    SetTargetFPS(60);

    // WALLS
    // RAII Texture
    ManagedTexture brick("assets/models/brick/textures/Brick_Wall_5M_Berlin_yhtvxwB_4K_baseColor.png");

    WallHandler wallHandler;

    // full-textured wall
    wallHandler.AddWall(std::make_unique<TexturedWall>(
        Vector3{0.0f, 2.5f, -8.0f},
        Vector3{8.0f, 5.0f, 1.0f},
        brick
    ));
    
    // sub-rectangle textured wall (atlas example)
    Rectangle brickFace = {
        0.0f,
        0.0f,
        static_cast<float>(brick.get().width) / 2.0f,
        static_cast<float>(brick.get().height) / 2.0f
    };
    wallHandler.AddWall(std::make_unique<TexturedWallRec>(
        Vector3{10.0f, 2.5f, -8.0f},
        Vector3{4.0f, 5.0f, 1.0f},
        brick,
        brickFace
    ));

    // colored walls
    wallHandler.AddWall(std::make_unique<ColoredWall>(Vector3{-16.0f, 2.5f, 0.0f},Vector3{1.0f, 5.0f, 32.0f}, GOLD));
    wallHandler.AddWall(std::make_unique<ColoredWall>(Vector3{16.0f, 2.5f, 0.0f}, Vector3{1.0f, 5.0f, 32.0f}, GOLD));
    wallHandler.AddWall(std::make_unique<ColoredWall>(Vector3{0.0f, 2.5f, 16.0f}, Vector3{32.0f, 5.0f, 1.0f}, GOLD));
    // END: WALLS

    // PROJECTILES
    // load configs and track modification time
    std::map<std::string, GearConfig> configs = LoadGearConfigs(configFile);
    std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(configFile);

    Launcher projectileOne(configs["projectileOne"]);
    Launcher projectileTwo(configs["projectileTwo"]);
    Launcher dodgeBall(configs["dodgeBall"]);

    std::vector<std::unique_ptr<BaseProjectile>> projectiles;
    int projectileType = 0;
    // END: PROJECTILES

    while (!WindowShouldClose())
    {   
        // PROJECTILES
        float dt = GetFrameTime();

        // hot reload check
        std::filesystem::file_time_type currentWriteTime = std::filesystem::last_write_time(configFile);
        if (currentWriteTime != lastWriteTime) {
            try {
                configs = LoadGearConfigs(configFile);
                projectileOne.setConfig(configs["projectileOne"]);
                projectileTwo.setConfig(configs["projectileTwo"]);
                dodgeBall.setConfig(configs["dodgeBall"]);
                lastWriteTime = currentWriteTime;
                std::cout << "Reloaded gear configs!\n";
            } catch (const std::exception& e) {
                std::cerr << "Failed to reload configs: " << e.what() << "\n";
            }
        }

        // TODO: change to enum
        if (IsKeyPressed(KEY_ONE)) projectileType = 0;
        if (IsKeyPressed(KEY_TWO)) projectileType = 1;
        if (IsKeyPressed(KEY_THREE)) projectileType = 2;

        projectileOne.update(dt);
        projectileTwo.update(dt);
        dodgeBall.update(dt);

        if (projectileType == 0) projectileOne.tryFire(camera, projectiles, 0);
        else if (projectileType == 1) projectileTwo.tryFire(camera, projectiles, 1);
        else if (projectileType == 2) dodgeBall.tryFire(camera, projectiles, 2);

        for (std::unique_ptr<BaseProjectile>& p : projectiles) p->update(dt);
        std::erase_if(projectiles, [](const std::unique_ptr<BaseProjectile>& p) { return !p->isAlive(); });
        // END: PROJECTILES


        UpdateCamera(&camera, cameraMode); 
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                //loop through and draw all projectiles' bounding boxes and check for collisions with wall bounding boxes
                for (std::unique_ptr<BaseProjectile>& p : projectiles) {
                    p->draw();

                    // loop all walls
                    for (const auto& wall : wallHandler.GetWalls()) {
                        if (CheckCollisionBoxes(p->GetBoundingBox(), wall->GetBoundingBox())) {
                            // Handle collision
                            float sphereRadius = p->GetBoundingBox().max.x - p->GetBoundingBox().min.x;
                            Vector3 collisionPoint = Vector3Add(p->GetBoundingBox().min, Vector3Multiply((Vector3){0.5f, 0.5f, 0.5f}, Vector3Subtract(p->GetBoundingBox().max, p->GetBoundingBox().min)));

                            // offset the collisionPoint along the ray that it was fired, place the mark on the face of the wall (so it will not be buried in the wall)
                            collisionPoint = Vector3Subtract(collisionPoint, Vector3Scale(p->GetForward(), (sphereRadius * 4)));
                
                            wall->AddCollisionPoint(collisionPoint);

                            p->setIsAlive(false);
                        }
                    }
                }

                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY);
                wallHandler.DrawWalls(true);

            EndMode3D();
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}