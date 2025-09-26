#include "raylib.h"
#include <memory>
#include <iostream>
#include "include/core/custom_camera.h"
#include "include/projectiles/launcher.h"
#include "include/opponents/components.h"
#include "include/opponents/systems.h"
#include "include/opponents/opponents.h"
#include "include/ecs/registry.h"
#include "include/ecs/systems.h"
#include "include/world/room.h"
#include "include/world/hallway.h"
#include "include/world/anchor.h"
#include "include/textures/managed_texture.h"
#include "include/render/draw_utils.h"

// g++ -std=c++23 main.cpp src/render/draw_utils.cpp src/projectiles/gear_config.cpp src/projectiles/launcher.cpp src/projectiles/projectile.cpp -o main -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

// release build targeting C++23 with warnings and optimizations
// g++ -std=c++23 -O2 -march=native -flto -Wall -Wextra -Wpedantic main.cpp src/render/draw_utils.cpp src/projectiles/gear_config.cpp src/projectiles/launcher.cpp src/projectiles/projectile.cpp -o main -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

int main(void)
{
    const int screenWidth = 4400;
    const int screenHeight = 2800;
    const std::string configFile = "assets/configs/gear.json";
    bool isDebug = false;

    InitWindow(screenWidth, screenHeight, "FPS SYSTEM");

    Camera camera { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;                         
    camera.projection = CAMERA_PERSPECTIVE;      

    int cameraMode = CAMERA_FIRST_PERSON;
    DisableCursor(); /////////////////////////
    SetTargetFPS(60);

    OpponentStore opponents;

    // spawn opponents
    spawnOpponent(opponents, {0, 0.1f, 0}, 0.002f, 10, BLUE, {0.5f, 5.0f, 0.5f}); // fast, fragile
    spawnOpponent(opponents, {5, 0.1f, 5}, 0.0005f, 100, DARKGRAY, {1.0f, 6.0f, 1.0f}); // slow, tough

    // WALLS
    auto brick = std::make_shared<ManagedTexture>("assets/models/brick/textures/Brick_Wall_5M_Berlin_yhtvxwB_4K_baseColor.png");
    if (brick->get().id == 0) {
        std::cerr << "DEV: Texture failed to load!\n";
        // TODO:
    }
    
    Registry reg;
    
    // TODO: when I figure out what this will become, I will bring the SystemManager back in...
    TransformSystem transformSystem;
    DrawSystem drawSystem;
    
    // scale factor
    const float S = 20.0f;

    // room and hallway dimensions
    Vector3 roomSize = {10*S, 2.5f*S, 10*S};  // 200 x 50 x 200
    Vector3 hallSize = {4*S, 2.5f*S, 12*S};   // 240 x 50 x 80  (long along Z)

    // spacing for front/back alignment
    float spacing = roomSize.z/2 + hallSize.z/2; // 200/2 + 80/2 = 140

    // place rooms in front and back, hallway in between
    Entity room1 = CreateRoom(reg, { 0, 0, -spacing }, roomSize, brick, std::vector<Wall::Side>{ Wall::Side::Back });  // open back
    Entity room2 = CreateRoom(reg, { 0, 0,  spacing }, roomSize, brick, std::vector<Wall::Side>{ Wall::Side::Front }); // open front    
    Entity hall  = CreateHallway(reg, { 0, 0, 0 }, hallSize, brick);

    // now calc WorldTransforms for all entities and anchors
    transformSystem.update(reg);

    // helper to find anchor on a parent with a specific direction
    // used to locate the specific anchor entities on room1, room2, and hall based on their intended connection directions
    auto findAnchorByDir = [&](Entity parentEntity, Vector3 dir)->Entity {
        if (auto children = reg.get<Children>(parentEntity)) {
            for (Entity child : children->entities) {
                // if the child entity has an anchor component, it normalizes both the given direction and the anchor direction
                if (auto a = reg.get<Anchor>(child)) {
                    // normalize and compare directions
                    Vector3 normalizedDir = Vector3Normalize(dir);
                    Vector3 normalizedAnchorDir = Vector3Normalize(a->direction);
                    
                    float dot = Vector3DotProduct(normalizedAnchorDir, normalizedDir);
                    if (dot > 0.99f) return child; // the directions are close enough
                }
            }
        }
        // if no suitable anchor entity is found, return INVALID_ENTITY
        return INVALID_ENTITY;
    };

    // connect room1's right anchor to hall's left anchor
    Entity r1_right = findAnchorByDir(room1, { 1, 0, 0 });
    Entity hall_left = findAnchorByDir(hall, { -1, 0, 0 });


    // ConnectAnchors is called twice 
    // in order to link room1 to hall and room2 to hall 
    // this snaps positions and carves doorways
    if (r1_right != INVALID_ENTITY && hall_left != INVALID_ENTITY) {
        ConnectAnchors(reg, r1_right, hall_left);
    } else {
        std::cerr << "DEV Warning: missing anchors for room1<->hall connection\n";
    }

    // connect room2's left anchor to hall's right anchor
    Entity r2_left = findAnchorByDir(room2, { -1, 0, 0 });
    Entity hall_right = findAnchorByDir(hall, { 1, 0, 0 });

    if (r2_left != INVALID_ENTITY && hall_right != INVALID_ENTITY) {
        ConnectAnchors(reg, r2_left, hall_right);
    } else {
        std::cerr << "DEV Warning: missing anchors for room2<->hall connection\n";
    }

    // called again to update transforms after the connection adjustments
    transformSystem.update(reg);


    // PROJECTILES
    // load configs and track modification time
    std::map<std::string, GearConfig> configs = LoadGearConfigs(configFile);
    std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(configFile);

    Launcher projectileOne(configs["projectileOne"]);
    Launcher projectileTwo(configs["projectileTwo"]);
    Launcher dodgeBall(configs["dodgeBall"]);

    std::vector<std::unique_ptr<Projectile>> projectiles;
    int projectileType = 0;
    // END: PROJECTILES

    while (!WindowShouldClose())
    {   
        UpdateCamera(&camera, cameraMode); 

        float dt = GetFrameTime();

        // PROJECTILES

        // Update all systems
        // systems.update(dt);

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

        for (std::unique_ptr<Projectile>& p : projectiles) p->update(dt);
        std::erase_if(projectiles, [](const std::unique_ptr<Projectile>& p) { return !p->isAlive(); });
        // END: PROJECTILES


        // WALLS
        // periodic cleanup about every 1000 frames
        static int frameCount = 0;
        if (++frameCount % 1000 == 0) {
            reg.cleanup();
        }
        // transformSystem.update(reg, dt); // currently nothing moves, but system supports it


        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                drawSystem.update(reg, dt); // draw all entities (wall system)

                // check for collisions with projectiles and wall bounding boxes
                for (std::unique_ptr<Projectile>& p : projectiles) {
                    p->draw(isDebug);
                    damageSystem(opponents.healths, opponents.bounds, p->GetBoundingBox()); // check for damage to opponents
                    // (old, when walls were polymorphic class objects)
                    // loop all walls 
                    // for (const auto& wall : wallHandler.GetWalls()) {
                    //     if (CheckCollisionBoxes(p->GetBoundingBox(), wall->GetBoundingBox())) {
                    //         // Handle collision
                    //         float sphereRadius = p->GetBoundingBox().max.x - p->GetBoundingBox().min.x;
                    //         Vector3 collisionPoint = Vector3Add(p->GetBoundingBox().min, Vector3Multiply((Vector3){0.5f, 0.5f, 0.5f}, Vector3Subtract(p->GetBoundingBox().max, p->GetBoundingBox().min)));

                    //         // offset the collisionPoint along the ray that it was fired, place the mark on the face of the wall (so it will not be buried in the wall)
                    //         collisionPoint = Vector3Subtract(collisionPoint, Vector3Scale(p->GetForward(), sphereRadius));
                
                    //         wall->AddCollisionPoint(collisionPoint);

                    //         p->setIsAlive(false);
                    //     }
                    // }
                }

                // OPPONENTS
                movementSystem(opponents.positions, opponents.movements, camera.position);
                boundingSystem(opponents.positions, opponents.bounds);
                renderSystem(opponents.positions, opponents.bounds, opponents.healths, opponents.renders);
                
            EndMode3D();
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}