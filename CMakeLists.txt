cmake_minimum_required(VERSION 3.20)
project(Asteroids_Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GAME_CORE_SRC
        Vector2.h Vector2.cpp
        Config.h
        GameObject.h GameObject.cpp
        AsteroidType.h
        AsteroidBehavior.h
        NormalBehavior.h NormalBehavior.cpp
        ExplosiveBehavior.h ExplosiveBehavior.cpp
        GravityBehavior.h GravityBehavior.cpp
        Asteroid.h Asteroid.cpp
        AsteroidFactory.h AsteroidFactory.cpp
        AsteroidSpawner.h AsteroidSpawner.cpp
        Projectile.h Projectile.cpp
        ProjectilePool.h ProjectilePool.cpp
        Spaceship.h Spaceship.cpp
        ShieldBonus.h ShieldBonus.cpp
        Command.h Command.cpp
        InputHandler.h InputHandler.cpp
        CollisionSystem.h CollisionSystem.cpp
        GameObserver.h GameObserver.cpp
        ScoreManager.h ScoreManager.cpp
        Renderer.h Renderer.cpp
        GameEngine.h GameEngine.cpp
        GameState.h
        MenuState.h MenuState.cpp
        PlayingState.h PlayingState.cpp
        PausedState.h PausedState.cpp
        GameOverState.h GameOverState.cpp
        VictoryState.h VictoryState.cpp
        Game.h Game.cpp
)

add_executable(Asteroids_Game main.cpp ${GAME_CORE_SRC})

find_package(SFML 3 COMPONENTS Graphics Window System QUIET)
if(SFML_FOUND)
    target_link_libraries(Asteroids_Game PRIVATE SFML::Graphics SFML::Window SFML::System)
else()
    find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
    target_link_libraries(Asteroids_Game PRIVATE sfml-graphics sfml-window sfml-system)
endif()
