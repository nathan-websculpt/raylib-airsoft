#### NOTE: This has only been tested on Linux (on wide monitors) - requires raylib

# Compile
```

g++ -std=c++23 main.cpp src/render/draw_utils.cpp src/projectiles/gear_config.cpp src/projectiles/launcher.cpp src/projectiles/projectile.cpp -o main -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

// release build targeting C++23 with warnings and optimizations
g++ -std=c++23 -O2 -march=native -flto -Wall -Wextra -Wpedantic main.cpp src/render/draw_utils.cpp src/projectiles/gear_config.cpp src/projectiles/launcher.cpp src/projectiles/projectile.cpp -o main -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


./main
```

## NOTES

Has been changed from a class-based (polymorphic) system to an ECS system. 