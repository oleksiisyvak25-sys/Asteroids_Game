cmake_minimum_required(VERSION 4.3)
project(Asteroids_Game)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Asteroids_Game
        Asteroid.cpp
        Asteroid.h
        AsteroidBehavior.cpp
        AsteroidBehavior.h
        AsteroidFactory.cpp
        AsteroidFactory.h
        AsteroidSpawner.cpp
        AsteroidSpawner.h
        CollisionSystem.cpp
        CollisionSystem.h
        ExplosiveBehavior.cpp
        ExplosiveBehavior.h
        Game.cpp
        Game.h
        GameEngine.cpp
        GameEngine.h
        GameObject.cpp
        GameObject.h
        GameObserver.cpp
        GameObserver.h
        GameOverState.cpp
        GameOverState.h
        GameState.cpp
        GameState.h
        GravityBehavior.cpp
        GravityBehavior.h
        MenuState.cpp
        MenuState.h
        NormalBehavior.cpp
        NormalBehavior.h
        PausedState.cpp
        PausedState.h
        PlayingState.cpp
        PlayingState.h
        Projectile.cpp
        Projectile.h
        ProjectilePool.cpp
        ProjectilePool.h
        Spaceship.cpp
        Spaceship.h
        Vector2.cpp
        Vector2.h)
